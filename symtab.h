#include "tree.h"
#include "type.h"

struct sym_table *mksymtab(int size);
struct sym_table *mksymtab_global(int size);
int hash(struct sym_table *st, char *s);
void printsyms(struct tree *t);
void insert(struct sym_table *st, char *name, typeptr t, int is_mutable, int is_nullable);
struct sym_entry *lookup(struct sym_table *st, char *name);
struct sym_entry *lookup_current(struct sym_table *st, char *name);
void build_symtab(struct tree *node, struct sym_table *current, int *symtab_err_flag, char *filename);
void print_scope(struct sym_table *st, int level);
void print_symtab(struct sym_table *st, int level);
int infer_type(struct token *node);
paramlist build_and_insert_params(struct tree *node, struct sym_table *st, int *count, int *symtab_err_flag, char *filename);
paramlist build_param_list_only(struct tree *node, int *count);
typeptr typecheck_expr(struct tree *node, struct sym_table *current, int *err, char *filename);
typeptr check_function_call(struct tree *node, struct sym_table *current, int *err, char *filename);
int type_equal(typeptr a, typeptr b);
int is_numeric_type(int bt);
int is_assignable(typeptr dst, typeptr src);
typeptr get_bool_typeptr(void);
void make_non_nullable(struct sym_entry *e);
void make_nullable(struct sym_entry *e);
int is_nullable_expr(struct tree *node, struct sym_table *st);
typeptr type_from_ast_node(struct tree *type_node);
void typecheck_ast(struct tree *node, struct sym_table *current, int *err, char *filename, typeptr expected_return_type, int func_returns_nullable);

struct sym_entry
{
    char *name;             // identifier name
    typeptr type;           // full type information
    int is_mutable;         // 0 = val (immutable), 1 = var (mutable)
    int is_nullable;        // 0 = no ? (not nullable), 1 = ? (nullable)
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