// tac.c - TAC builder
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "tree.h"
#include "symtab.h"

extern int tempoffset;

// region / opcode name tables
char *regionnames[] = {
    "global", "loc", "class", "lab", "const", "", "none", "str"};
char *regionname(int i) { return regionnames[i - R_GLOBAL]; }

char *opcodenames[] = {
    "ADD", "SUB", "MUL", "DIV", "NEG", "ASN", "ADDR",
    "LCONT", "SCONT", "GOTO", "BLT", "BLE", "BGT", "BGE",
    "BEQ", "BNE", "BIF", "BNIF", "PARAM", "CALL", "RETURN"};
char *opcodename(int i) { return opcodenames[i - O_ADD]; }

// D_GLOB=3051 … D_CODE=3059
char *pseudonames[] = {
    "glob", "proc", "loc", "lab", "end", "prot",
    "string", "stringlit", "code"};
char *pseudoname(int i) { return pseudonames[i - D_GLOB]; }

// string literal table
#define MAX_STRINGS 64
char *stringliterals[MAX_STRINGS];
int stringcount = 0;

// Intern a string literal.  Returns its 0-based index
int addstring(const char *s)
{
        if (stringcount >= MAX_STRINGS)
        {
                fprintf(stderr, "string table overflow\n");
                exit(1);
        }
        stringliterals[stringcount] = strdup(s);
        return stringcount++;
}

// Total byte size of all interned strings, rounded up to a multiple of 8.
int stringsectionsize(void)
{
        return stringcount * 8; // one 8-byte slot per string entry
}

//  Build the pseudo-instruction sequence for the .string section:
// D_STRINGSEC  dest = addr_const(size)
// D_STRINGLIT  dest = addr_name(text) one per literal
// D_CODE marks start of code
struct instr *gen_stringsection(void)
{
        struct addr none = {R_NONE, {0}};

        // .string <size>
        struct addr szaddr = {R_CONST, {.offset = stringsectionsize()}};
        struct instr *head = gen(D_STRINGSEC, szaddr, none, none);

        // one D_STRINGLIT per entry
        for (int i = 0; i < stringcount; i++)
        {
                struct addr lit;
                lit.region = R_NAME;
                lit.u.name = stringliterals[i];
                head = append(head, gen(D_STRINGLIT, lit, none, none));
        }

        // .code
        head = append(head, gen(D_CODE, none, none, none));
        return head;
}

// label counter
int labelcounter;

struct addr *genlabel(void)
{
        struct addr *a = malloc(sizeof(struct addr));
        a->region = R_LABEL;
        a->u.offset = labelcounter++;
        printf("generated a label %d\n", a->u.offset);
        return a;
}

// instruction constructors
struct instr *gen(int op, struct addr a1, struct addr a2, struct addr a3)
{
        struct instr *rv = malloc(sizeof(struct instr));
        if (!rv)
        {
                fprintf(stderr, "out of memory\n");
                exit(4);
        }
        rv->opcode = op;
        rv->dest = a1;
        rv->src1 = a2;
        rv->src2 = a3;
        rv->next = NULL;
        return rv;
}

struct instr *copylist(struct instr *l)
{
        if (!l)
                return NULL;
        struct instr *c = gen(l->opcode, l->dest, l->src1, l->src2);
        c->next = copylist(l->next);
        return c;
}

struct instr *append(struct instr *l1, struct instr *l2)
{
        if (!l1)
                return l2;
        struct instr *p = l1;
        while (p->next)
                p = p->next;
        p->next = l2;
        return l1;
}

struct instr *concat(struct instr *l1, struct instr *l2)
{
        return append(copylist(l1), l2);
}

// address helpers
struct addr addr_loc(int offset)
{
        struct addr a = {R_LOCAL, {.offset = offset}};
        return a;
}
struct addr addr_const(int val)
{
        struct addr a = {R_CONST, {.offset = val}};
        return a;
}
struct addr addr_name(char *name)
{
        struct addr a;
        a.region = R_NAME;
        a.u.name = name;
        return a;
}
struct addr addr_none(void)
{
        struct addr a = {R_NONE, {.offset = 0}};
        return a;
}
// New: reference a string literal by its table index
struct addr addr_string(int idx)
{
        struct addr a = {R_STRING, {.offset = idx}};
        return a;
}

// address printer
void print_addr(struct addr a)
{
        switch (a.region)
        {
        case R_LOCAL:
                printf("loc:%d", a.u.offset);
                break;
        case R_GLOBAL:
                printf("glob:%d", a.u.offset);
                break;
        case R_CONST:
                printf("const:%d", a.u.offset);
                break;
        case R_LABEL:
                printf("lab:%d", a.u.offset);
                break;
        case R_STRING:
                printf("str:%d", a.u.offset);
                break; // ← new
        case R_NAME:
                printf("%s", a.u.name);
                break;
        case R_NONE:
                break;
        default:
                printf("?:%d", a.u.offset);
                break;
        }
}

// TAC printer
void tacprint(struct instr *code)
{
        for (struct instr *p = code; p != NULL; p = p->next)
        {

                // pseudo/declaration instructions
                if (p->opcode >= D_GLOB && p->opcode <= D_CODE)
                {
                        switch (p->opcode)
                        {

                        case D_STRINGSEC:
                                printf(".string %d\n", p->dest.u.offset);
                                break;

                        case D_STRINGLIT:
                                printf("\t%s\n", p->dest.u.name);
                                break;

                        case D_CODE:
                                printf(".code\n");
                                break;

                        case D_PROC:
                                printf("proc ");
                                print_addr(p->dest);
                                printf(",%d,%d\n", p->src1.u.offset, p->src2.u.offset);
                                break;

                        case D_LOCAL:
                                printf("\t.local\t");
                                print_addr(p->dest);
                                printf("\n");
                                break;

                        case D_LABEL:
                                printf("lab%d:\n", p->dest.u.offset);
                                break;

                        case D_END:
                                printf("end\t");
                                print_addr(p->dest);
                                printf("\n");
                                break;

                        case D_GLOB:
                                printf(".glob\t");
                                print_addr(p->dest);
                                printf("\n");
                                break;

                        default:
                                printf("%s\t", pseudoname(p->opcode));
                                print_addr(p->dest);
                                printf("\n");
                        }
                        continue;
                }

                // real instructions
                printf("\t%-6s\t", opcodename(p->opcode));

                switch (p->opcode)
                {
                case O_ADD:
                case O_SUB:
                case O_MUL:
                case O_DIV:
                        print_addr(p->dest);
                        printf(",");
                        print_addr(p->src1);
                        printf(",");
                        print_addr(p->src2);
                        break;

                case O_NEG:
                case O_ASN:
                        print_addr(p->dest);
                        printf(",");
                        print_addr(p->src1);
                        break;

                case O_GOTO:
                        print_addr(p->dest);
                        break;

                case O_BLT:
                case O_BLE:
                case O_BGT:
                case O_BGE:
                case O_BEQ:
                case O_BNE:
                        print_addr(p->dest);
                        printf(",");
                        print_addr(p->src1);
                        printf(",");
                        print_addr(p->src2);
                        break;

                case O_BIF:
                case O_BNIF:
                        print_addr(p->dest);
                        printf(",");
                        print_addr(p->src1);
                        break;

                case O_PARM:
                        print_addr(p->dest);
                        break;

                case O_CALL:
                        print_addr(p->src1);              // function name
                        printf(",%d,", p->src2.u.offset); // #params
                        print_addr(p->dest);              // return loc
                        break;

                case O_RET:
                        if (p->dest.region != R_NONE)
                                print_addr(p->dest);
                        break;

                default:
                        print_addr(p->dest);
                        printf(",");
                        print_addr(p->src1);
                        printf(",");
                        print_addr(p->src2);
                }
                printf("\n");
        }
}

struct addr new_temp(void)
{
        printf("tempoffset: %d\n", tempoffset);
        struct addr a;
        a.region = R_LOCAL;
        a.u.offset = tempoffset;
        tempoffset += 8; // advance by one 8-byte slot
        return a;
}

void dump_scope(struct sym_table *st)
{
    fprintf(stderr, "=== scope: %s ===\n", st->scope_name);
    for (int i = 0; i < st->nBuckets; i++) {
        struct sym_entry *e = st->tbl[i];
        while (e) {
            fprintf(stderr, "  entry: '%s'\n", e->name);
            e = e->next;
        }
    }
}

struct addr lookup_place(struct tree *t, struct sym_table *scope)
{
    //fprintf(stderr, "lookup_place: scope is '%s'\n", scope ? scope->scope_name : "NULL");
    //fprintf(stderr, "lookup_place: leaf is %s\n", t->leaf ? t->leaf->text : "NULL");
    if (t->leaf == NULL) {
        fprintf(stderr, "lookup_place: expected a leaf node\n");
        return addr_none();
    }

    //printf("current leaf %s is category %d\n", t->leaf->text, t->leaf->category);

    // use the symbol entry already attached during the semantic pass
    struct sym_entry *e = t->symbol;
    //printf("current token %s has prodrule %d\n", t->symbolname, t->prodrule);

    // fallback to lookup if symbol wasn't attached
    if (e == NULL)
    {
        fprintf(stderr, "lookup_place: calling lookup with scope '%s'\n", scope ? scope->scope_name : "NULL");
        e = lookup(scope, t->leaf->text);
    }

    if (e == NULL) {
        fprintf(stderr, "lookup_place: '%s' not found\n", t->leaf->text);
        return addr_none();
    }

    struct addr a;
    a.region = e->region;
    a.u.offset = e->offset;
    return a;
}
