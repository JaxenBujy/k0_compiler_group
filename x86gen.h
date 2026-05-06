/*
 * x86gen.h — x86-64 AT&T assembly generator for k0 TAC
 *
 * Translates the linked list of TAC instructions produced by codegen()
 * into x86-64 assembly suitable for GAS (GNU assembler) on Linux.
 *
 * Calling convention: System V AMD64 ABI
 *   - First six integer/pointer args in %rdi, %rsi, %rdx, %rcx, %r8, %r9
 *   - Return value in %rax
 *   - %rbp used as frame pointer; locals at -(offset+8)(%rbp)
 *
 * Address mapping from TAC regions:
 *   R_LOCAL  offset N  →  -(N+8)(%rbp)
 *   R_GLOBAL offset N  →  _glob_N(%rip)
 *   R_CONST  val    N  →  $N  (immediate)
 *   R_LABEL  index  N  →  .Llab_N  (local label)
 *   R_STRING index  N  →  .Lstr_N(%rip)  (string literal address)
 *   R_NAME   name   s  →  s   (external / function name)
 *   R_PARM   index  N  →  argregs[N] (%rdi, %rsi, …)
 *
 * Usage:
 *   FILE *asm_out = fopen("program.s", "w");
 *   x86gen(asm_out, tac_code);
 *   fclose(asm_out);
 *
 * Link the output with k0rt.c to resolve the k0 runtime functions
 * (println, print, readln, …):
 *   gcc program.s k0rt.c -o program
 */
#ifndef X86GEN_H
#define X86GEN_H

#include <stdio.h>
#include "tac.h"

/* Generate x86-64 AT&T assembly from a TAC instruction list.
 * Writes a complete, self-contained .s file to `out`. */
void x86gen(FILE *out, struct instr *code);

#endif /* X86GEN_H */
