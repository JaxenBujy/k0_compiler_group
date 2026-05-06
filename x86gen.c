/*
 * x86gen.c — x86-64 AT&T assembly generator for k0
 *
 * Translates the flat TAC instruction list into a complete GAS .s file.
 *
 * Design notes
 * ============
 * • All values live in memory (stack slots or .data globals).  Two scratch
 *   registers, %rax and %rcx, are used for every operation.  This is
 *   maximally simple; a register allocator can replace it later.
 *
 * • Frame layout (relative to %rbp after the prologue):
 *     (%rbp)       saved %rbp
 *     -8(%rbp)     loc:0   (first local / parameter)
 *     -16(%rbp)    loc:8
 *     ...
 *     -(N+8)(%rbp) loc:N
 *
 * • Frame size K (from D_PROC src2) is rounded up to the next multiple of
 *   16 bytes to satisfy the System V AMD64 ABI stack-alignment requirement
 *   before any inner callq.
 *
 * • Parameters arrive in the SysV register order (%rdi, %rsi, %rdx, …).
 *   The TAC already contains explicit O_ASN parm:N → loc:M instructions
 *   (emitted by tree.c) that save those registers to their stack slots.
 *
 * • Function calls: O_PARM instructions are buffered; on O_CALL the
 *   buffered operands are loaded into the argument registers in order.
 *
 * • String literals are placed in .rodata with labels .Lstr_N.
 *   Globals are placed in .data as 8-byte zero-initialized quads.
 *
 * • The k0 "main" function calls __init before executing the user body,
 *   initialising any global variables.  k0rt.c provides a weak __init
 *   fallback in case no globals were declared.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tac.h"
#include "x86gen.h"

/* ── SysV AMD64 integer argument registers (in order) ─────────────────────── */
static const char *argregs[6] = {
    "%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"
};

/* ── Pending O_PARM operands waiting for the next O_CALL ─────────────────── */
#define MAX_PARAMS 16
static struct addr param_buf[MAX_PARAMS];
static int        n_pending = 0;

/* ── Running index assigned to successive D_STRINGLIT pseudo-instructions ─── */
static int str_label_idx = 0;

/* =========================================================================
 * Address helpers
 * ========================================================================= */

/*
 * Load the value represented by `a` into the named register.
 * Strings are loaded as a pointer (leaq) rather than a value.
 */
static void load_into(FILE *out, struct addr a, const char *reg)
{
    switch (a.region) {
    case R_CONST:
        fprintf(out, "\tmovq\t$%d, %s\n", a.u.offset, reg);
        break;
    case R_LOCAL:
        fprintf(out, "\tmovq\t-%d(%%rbp), %s\n", a.u.offset + 8, reg);
        break;
    case R_GLOBAL:
        fprintf(out, "\tmovq\t_glob_%d(%%rip), %s\n", a.u.offset, reg);
        break;
    case R_STRING:
        /* Load the address of the string literal (a pointer). */
        fprintf(out, "\tleaq\t.Lstr_%d(%%rip), %s\n", a.u.offset, reg);
        break;
    case R_PARM:
        /* The value is already in an incoming argument register. */
        if (a.u.offset < 6)
            fprintf(out, "\tmovq\t%s, %s\n", argregs[a.u.offset], reg);
        break;
    case R_LABEL:
        /* Labels are jump targets, not data values – should not appear here. */
        fprintf(out, "\t# load_into: unexpected R_LABEL\n");
        break;
    case R_NAME:
        /* Function pointers / external names are not supported yet. */
        fprintf(out, "\t# load_into: R_NAME '%s' not supported as a value\n",
                a.u.name);
        break;
    default:
        break;
    }
}

/*
 * Store the value in `reg` into the memory location described by `a`.
 * Constants, labels, and names are not writable – silently ignored.
 */
static void store_from(FILE *out, const char *reg, struct addr a)
{
    switch (a.region) {
    case R_LOCAL:
        fprintf(out, "\tmovq\t%s, -%d(%%rbp)\n", reg, a.u.offset + 8);
        break;
    case R_GLOBAL:
        fprintf(out, "\tmovq\t%s, _glob_%d(%%rip)\n", reg, a.u.offset);
        break;
    default:
        fprintf(out, "\t# store_from: cannot store into region %d\n", a.region);
        break;
    }
}

/*
 * Emit a re-escaped string character for use inside .asciz "...".
 * Returns the number of bytes written.
 */
static void emit_escaped_char(FILE *out, unsigned char c)
{
    switch (c) {
    case '\n': fputs("\\n",  out); break;
    case '\t': fputs("\\t",  out); break;
    case '\r': fputs("\\r",  out); break;
    case '\\': fputs("\\\\", out); break;
    case '"':  fputs("\\\"", out); break;
    default:
        if (c < 0x20 || c == 0x7f)
            fprintf(out, "\\%03o", c);  /* octal for non-printable */
        else
            fputc(c, out);
        break;
    }
}

/* =========================================================================
 * Main code generator
 * ========================================================================= */

void x86gen(FILE *out, struct instr *code)
{
    n_pending     = 0;
    str_label_idx = 0;

    fprintf(out,
            "# k0 compiler — generated x86-64 AT&T assembly\n"
            "# Assemble and link: gcc <this_file> k0rt.c -o <program>\n\n");

    for (struct instr *p = code; p != NULL; p = p->next) {

        /* ── Pseudo / directive instructions ─────────────────────────── */
        if (p->opcode >= D_GLOB && p->opcode <= D_DATA) {
            switch (p->opcode) {

            /* .string section — placed in read-only data */
            case D_STRINGSEC:
                fprintf(out, "\t.section\t.rodata\n");
                str_label_idx = 0;          /* reset for label assignment */
                break;

            /* One string literal entry; dest.region == R_NAME, dest.u.name
               holds the already-unescaped string content. */
            case D_STRINGLIT:
                fprintf(out, ".Lstr_%d:\n\t.asciz\t\"", str_label_idx++);
                if (p->dest.u.name) {
                    for (const char *s = p->dest.u.name; *s; s++)
                        emit_escaped_char(out, (unsigned char)*s);
                }
                fprintf(out, "\"\n");
                break;

            /* .data section for global variables */
            case D_DATA:
                fprintf(out, "\n\t.section\t.data\n");
                break;

            /* One global variable slot — 8-byte zero-initialised quad */
            case D_GLOB:
                fprintf(out, "_glob_%d:\n\t.quad\t0\n", p->dest.u.offset);
                break;

            /* .code section — switch to .text */
            case D_CODE:
                fprintf(out, "\n\t.section\t.text\n");
                break;

            /* Function prologue
               dest = addr_name(fname)
               src1 = addr_const(0)          (reserved / # of named params)
               src2 = addr_const(framesize)  (bytes needed for locals+temps) */
            case D_PROC: {
                const char *fname   = p->dest.u.name;
                int          fsz    = p->src2.u.offset;

                /* Round fsz up to a multiple of 16 for stack alignment.
                 * After pushq %rbp the stack is 8 bytes past a 16-byte
                 * boundary; subq by a multiple of 16 keeps it that way,
                 * satisfying the SysV requirement before any inner callq. */
                int K = ((fsz + 15) / 16) * 16;
                if (K < 16) K = 16;   /* always allocate at least one slot */

                fprintf(out, "\n\t.globl\t%s\n", fname);
                fprintf(out, "\t.type\t%s, @function\n", fname);
                fprintf(out, "%s:\n", fname);
                fprintf(out, "\tpushq\t%%rbp\n");
                fprintf(out, "\tmovq\t%%rsp, %%rbp\n");
                fprintf(out, "\tsubq\t$%d, %%rsp\n", K);

                /* k0's main must run global initialisers first.
                 * k0rt.c provides a weak __init fallback when none exist. */
                if (strcmp(fname, "main") == 0)
                    fprintf(out, "\tcallq\t__init\n");

                break;
            }

            /* Function epilogue — provides a default return-0 path in case
               the function body already emitted explicit O_RET instructions.
               (The code here is dead after any O_RET, but harmless.) */
            case D_END: {
                const char *fname = p->dest.u.name;
                fprintf(out, "\txorq\t%%rax, %%rax\n");   /* default: return 0 */
                fprintf(out, "\tleave\n");
                fprintf(out, "\tret\n");
                fprintf(out, "\t.size\t%s, .-%s\n", fname, fname);
                break;
            }

            /* Local label  (e.g. .Llab_7:) */
            case D_LABEL:
                fprintf(out, ".Llab_%d:\n", p->dest.u.offset);
                break;

            /* D_LOCAL — locals are on the stack, nothing to emit */
            case D_LOCAL:
                break;

            default:
                fprintf(out, "\t# unhandled pseudo-op %d\n", p->opcode);
                break;
            }
            continue;   /* skip the regular-instruction block */
        }

        /* ── Regular TAC instructions ────────────────────────────────── */
        switch (p->opcode) {

        /* ── Assignment ─────────────────────────────────────────────── */
        case O_ASN:
            /*
             * Special case: src1 is an incoming parameter register.
             * The O_ASN parm:N → loc:M instructions emitted by tree.c
             * save the SysV argument registers to their stack slots.
             */
            if (p->src1.region == R_PARM) {
                int pidx = p->src1.u.offset;
                if (pidx < 6)
                    store_from(out, argregs[pidx], p->dest);
                else
                    fprintf(out, "\t# O_ASN: parm:%d beyond register count\n",
                            pidx);
            } else {
                load_into(out, p->src1, "%rax");
                store_from(out, "%rax", p->dest);
            }
            break;

        /* ── Arithmetic ─────────────────────────────────────────────── */
        case O_ADD:
            load_into(out, p->src1, "%rax");
            load_into(out, p->src2, "%rcx");
            fprintf(out, "\taddq\t%%rcx, %%rax\n");
            store_from(out, "%rax", p->dest);
            break;

        case O_SUB:
            load_into(out, p->src1, "%rax");
            load_into(out, p->src2, "%rcx");
            fprintf(out, "\tsubq\t%%rcx, %%rax\n");
            store_from(out, "%rax", p->dest);
            break;

        case O_MUL:
            load_into(out, p->src1, "%rax");
            load_into(out, p->src2, "%rcx");
            /* imulq %rcx: rdx:rax = rax * rcx; low 64 bits in rax */
            fprintf(out, "\timulq\t%%rcx\n");
            store_from(out, "%rax", p->dest);
            break;

        case O_DIV:
            load_into(out, p->src1, "%rax");
            fprintf(out, "\tcqto\n");            /* sign-extend rax → rdx:rax */
            load_into(out, p->src2, "%rcx");
            fprintf(out, "\tidivq\t%%rcx\n");    /* quotient → %rax */
            store_from(out, "%rax", p->dest);
            break;

        case O_NEG:
            load_into(out, p->src1, "%rax");
            fprintf(out, "\tnegq\t%%rax\n");
            store_from(out, "%rax", p->dest);
            break;

        /* ── Unconditional jump ──────────────────────────────────────── */
        case O_GOTO:
            fprintf(out, "\tjmp\t.Llab_%d\n", p->dest.u.offset);
            break;

        /* ── Conditional branches / compare-and-set ─────────────────── *
         *                                                                 *
         * These opcodes (O_BLT..O_BNE) serve double duty:               *
         *                                                                 *
         *  Branch context  (dest.region == R_LABEL):                    *
         *    dest = jump target label                                     *
         *    src1 = left operand,  src2 = right operand                  *
         *    → cmpq; jCC .Llab_N                                         *
         *    (always followed by an O_GOTO to the false target)          *
         *                                                                 *
         *  Value context   (dest.region != R_LABEL):                    *
         *    dest = output temporary (R_LOCAL / R_GLOBAL)                *
         *    src1 = left operand,  src2 = right operand                  *
         *    → cmpq; SETcc %al; movzbq; store dest                      *
         *    Used when a relop result is assigned to a Boolean variable. */
        case O_BLT: case O_BLE: case O_BGT:
        case O_BGE: case O_BEQ: case O_BNE: {
            /* Map opcode → jump mnemonic and SETcc mnemonic */
            const char *jop, *setcc;
            switch (p->opcode) {
            case O_BLT: jop = "jl";   setcc = "setl";  break;
            case O_BLE: jop = "jle";  setcc = "setle"; break;
            case O_BGT: jop = "jg";   setcc = "setg";  break;
            case O_BGE: jop = "jge";  setcc = "setge"; break;
            case O_BEQ: jop = "je";   setcc = "sete";  break;
            case O_BNE: jop = "jne";  setcc = "setne"; break;
            default:    jop = "jmp";  setcc = "sete";  break;
            }

            load_into(out, p->src1, "%rax");
            load_into(out, p->src2, "%rcx");
            fprintf(out, "\tcmpq\t%%rcx, %%rax\n");

            if (p->dest.region == R_LABEL) {
                /* Branch context */
                fprintf(out, "\t%s\t.Llab_%d\n", jop, p->dest.u.offset);
            } else {
                /* Value context: materialise 0 or 1 */
                fprintf(out, "\t%s\t%%al\n", setcc);
                fprintf(out, "\tmovzbq\t%%al, %%rax\n");
                store_from(out, "%rax", p->dest);
            }
            break;
        }

        /* ── Boolean branches ────────────────────────────────────────── */
        case O_BIF:
            load_into(out, p->src1, "%rax");
            fprintf(out, "\ttestq\t%%rax, %%rax\n");
            fprintf(out, "\tjnz\t.Llab_%d\n", p->dest.u.offset);
            break;

        case O_BNIF:
            load_into(out, p->src1, "%rax");
            fprintf(out, "\ttestq\t%%rax, %%rax\n");
            fprintf(out, "\tjz\t.Llab_%d\n", p->dest.u.offset);
            break;

        /* ── Function call support ───────────────────────────────────── */

        /* Buffer each parameter operand in order. */
        case O_PARM:
            if (n_pending < MAX_PARAMS)
                param_buf[n_pending++] = p->dest;
            else
                fprintf(out, "\t# O_PARM: too many arguments (> %d)\n",
                        MAX_PARAMS);
            break;

        /*
         * O_CALL  dest=retval  src1=name  src2=nargs
         *
         * Load buffered parameters into the SysV argument registers in order,
         * push any extras (beyond 6) right-to-left onto the stack, emit callq,
         * clean up the stack, and store %rax as the return value.
         */
        case O_CALL: {
            const char *fname = p->src1.u.name;
            int nargs = p->src2.u.offset;

            /* Load the first min(nargs,6) params into argument registers. */
            int nreg = (n_pending < 6) ? n_pending : 6;
            for (int i = 0; i < nreg; i++)
                load_into(out, param_buf[i], argregs[i]);

            /* Push additional arguments right-to-left. */
            for (int i = n_pending - 1; i >= 6; i--) {
                load_into(out, param_buf[i], "%rax");
                fprintf(out, "\tpushq\t%%rax\n");
            }

            /*
             * For variadic C functions (printf family), %al must hold the
             * number of vector arguments used (0 for k0's integer/string calls).
             */
            int is_variadic = (strncmp(fname, "printf", 6) == 0 ||
                               strncmp(fname, "scanf",  5) == 0);
            if (is_variadic)
                fprintf(out, "\txorb\t%%al, %%al\n");

            fprintf(out, "\tcallq\t%s\n", fname);

            /* Clean up any stack-passed arguments. */
            int nextra = n_pending - 6;
            if (nextra > 0)
                fprintf(out, "\taddq\t$%d, %%rsp\n", nextra * 8);

            /* Store the return value if the caller uses it. */
            if (p->dest.region != R_NONE)
                store_from(out, "%rax", p->dest);

            (void)nargs;    /* nargs validated by semantic pass */
            n_pending = 0;
            break;
        }

        /* ── Return ──────────────────────────────────────────────────── */
        case O_RET:
            if (p->dest.region != R_NONE)
                load_into(out, p->dest, "%rax");
            else
                fprintf(out, "\txorq\t%%rax, %%rax\n");
            fprintf(out, "\tleave\n");
            fprintf(out, "\tret\n");
            break;

        /* ── Unimplemented / future opcodes ─────────────────────────── */
        case O_ADDR:
        case O_LCONT:
        case O_SCONT:
            fprintf(out, "\t# opcode %d not yet implemented\n", p->opcode);
            break;

        default:
            fprintf(out, "\t# unknown opcode %d\n", p->opcode);
            break;
        }
    }
}
