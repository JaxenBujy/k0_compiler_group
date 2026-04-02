#include "tree.h"

struct sym_table *mksymtab(int size);
int hash(struct sym_table *st, char *s);
void printsyms(struct tree *t);
void insert(struct sym_table *st, char *name);
struct sym_entry *lookup(struct sym_table *st, char *name);
struct sym_entry *lookup_current(struct sym_table *st, char *name);
void build_symtab(struct tree *node, struct sym_table *current, int *symtab_err_flag);
void insert_parameters(struct tree *node, struct sym_table *st, int *symtab_err_flag);
void print_scope(struct sym_table *st, int level);
void print_symtab(struct sym_table *st, int level);

struct sym_entry
{
    char *name;             // identifier name
    struct sym_entry *next; // next entry in same bucket
};

struct sym_table
{
    int nBuckets;
    int nEntries;
    char *scope_name; // name like package main or func f

    struct sym_table *parent;

    struct sym_table *child;   // first child scope
    struct sym_table *sibling; // next scope at same level

    struct sym_entry **tbl;
};