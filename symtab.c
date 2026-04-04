#include "symtab.h"
#include "k0gram.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// predefined functions in k0. In comments, before the . is the expected type that the method gets called on
// asking Dr. Zhan about this stuff, particularly if we need to support object-oriented-like method calls e.g "hello world".length vs. length("hello world")
// for symbol tables, all that matters is that we have the names defined here so that user does not get an error for using them
char *predefined_functions[] = {
    "print",     // print with no newline
    "println",   // print with newline
    "readln",    // reads a line of input from specified input stream
    "get",       // String.get
    "equals",    // String.equals
    "length",    // String.length
    "toString",  // String.toString
    "valueOf",   // String.valueOf
    "substring", // String.substring
    "nextInt",   // Random.nextInt
    "abs",       // Math.abs
    "max",       // Math.max
    "min",       // Math.min
    "pow",       // Math.pow
    "cos",       // Math.cos
    "sin",       // Math.sin
    "tan"        // Math.tan
};

struct sym_table *mksymtab(int size)
{
    struct sym_table *t = malloc(sizeof(struct sym_table));

    t->nBuckets = size;
    t->nEntries = 0;
    t->parent = NULL;

    t->child = NULL;
    t->sibling = NULL;
    t->scope_name = NULL;

    t->tbl = calloc(size, sizeof(struct sym_entry *));

    return t;
}

// strictly for globally defined functions, including the predefined functions specified in k0 specification.
struct sym_table *mksymtab_global(int size)
{
    struct sym_table *global = mksymtab(size);

    global->scope_name = strdup("global");

    // insert predefined functions
    int n = sizeof(predefined_functions) / sizeof(predefined_functions[0]);
    for (int i = 0; i < n; i++)
    {
        char *name = predefined_functions[i];

        // avoid accidental duplicates
        if (!lookup_current(global, name))
        {
            insert(global, name);
        }
    }

    return global;
}

int hash(struct sym_table *st, char *s)
{
    register int h = 0;
    register char c;
    while ((c = *s++))
    {
        h += c & 0377;
        h *= 37;
    }
    if (h < 0)
        h = -h;
    return h % st->nBuckets;
}

void insert(struct sym_table *st, char *name)
{
    int i = hash(st, name);

    struct sym_entry *e = malloc(sizeof(struct sym_entry));
    e->name = strdup(name);

    e->next = st->tbl[i];
    e->type = malloc(sizeof(typeptr));
    e->type->basetype = 100;
    st->tbl[i] = e;

    st->nEntries++;
}

struct sym_entry *lookup(struct sym_table *st, char *name)
{
    for (; st != NULL; st = st->parent)
    {
        int i = hash(st, name);
        struct sym_entry *e = st->tbl[i];

        while (e)
        {
            if (strcmp(e->name, name) == 0)
                return e;
            e = e->next;
        }
    }
    return NULL;
}

struct sym_entry *lookup_current(struct sym_table *st, char *name)
{
    int i = hash(st, name);
    struct sym_entry *e = st->tbl[i];

    while (e)
    {
        if (strcmp(e->name, name) == 0)
            return e;
        e = e->next;
    }
    return NULL;
}

void build_symtab(struct tree *node, struct sym_table *current, int *symtab_err_flag, char *filename)
{
    if (!node)
        return;

    switch (node->prodrule)
    {
    // Global variables
    case PR_GLOBAL_VAR_DECL_SIMPLE:
    case PR_GLOBAL_VAR_DECL_LITERAL_INIT:
    case PR_GLOBAL_VAR_INIT_INT:
    {
        char *name = node->kids[1]->leaf->text; // finding where IDENT name lives within global variable declarations kids array

        if (lookup_current(current, name))
        { // so if the name appears twice as a declaration in current symbol table, break out for now
            fprintf(stderr, "%s:%d: semantic error: redeclaration of %s\n", filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name); // else insert into current symbol table
        }
        break;
    }

    // Function Declarations
    case PR_FUNCTION_DECL_TYPED:
    case PR_FUNCTION_DECL_UNTYPED:
    case PR_FUNCTION_DECL_SEMICOLON:
    {
        char *name = node->kids[1]->leaf->text; // finding where IDENT lives within function declarations kids array

        // insert function into global scope
        if (lookup_current(current, name))
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of function %s\n", filename, node->kids[1]->leaf->lineno, name); // break out
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name); // insert into current symbol table
        }

        // create function scope
        struct sym_table *new_scope = mksymtab(16); // create new scope
        new_scope->parent = current;                // track new_scopes parent to the current
        new_scope->sibling = current->child;        // track the new scopes sibling to the child of current
        current->child = new_scope;                 // set new scope as current child

        // set current symtables scope name, since we know it's a function declaration, we say func <name>
        char *buf = malloc(strlen("func ") + strlen(name) + 1);
        strcpy(buf, "func ");
        strcat(buf, name);
        new_scope->scope_name = buf;

        // insert parameters as they are a special case, they are being included in that functions scope, not the parents scope
        struct tree *params = node->kids[3];
        insert_parameters(params, new_scope, symtab_err_flag, filename);

        // traverse body
        for (int i = 0; i < node->nkids; i++)
            build_symtab(node->kids[i], new_scope, symtab_err_flag, filename);

        return;
    }

    // Scope blocks. Commenting out since hw4 specifies they do not need to be supported
    // case PR_BLOCK:
    // {
    //     struct sym_table *new_scope = mksymtab(16); // create new scope
    //     new_scope->parent = current;                // track new_scopes parent to the current
    //     new_scope->sibling = current->child;        // track the new scopes sibling to the child of current
    //     current->child = new_scope;                 // set new scope as current child

    //     // kids[1] = statement_list, so just recurse over this statement list and let the rest of the cases handle it
    //     build_symtab(node->kids[1], new_scope, symtab_err_flag);
    //     return;
    // }

    // Function body variable declarations
    case PR_FUN_BODY_VAR_DECL_SIMPLE:
    case PR_FUN_BODY_VAR_DECL_LITERAL_INIT:
    case PR_FUN_BODY_VAR_INIT_INT:
    {
        char *name = node->kids[1]->leaf->text;

        if (lookup_current(current, name))
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of %s\n", filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name);
        }
        break;
    }

    // Assignment and arithmetic assignment
    case PR_ASSIGNMENT_ASSIGN:
    case PR_ASSIGNMENT_PLUS:
    case PR_ASSIGNMENT_MINUS:
    {
        char *name = node->kids[0]->leaf->text;

        struct sym_entry *e = lookup(current, name);
        if (!e)
        {
            fprintf(stderr, "%s:%d: semantic error: undeclared variable %s\n", filename, node->kids[0]->leaf->lineno, name); // look up variable and ensure it has been defined before
            *symtab_err_flag = 1;
        }
        else
        {
            node->kids[0]->symbol = e; // assign symbol info to AST node
        }
        break;
    }

    // Function call
    case PR_FUNCTION_CALL:
    {
        char *name = node->kids[0]->leaf->text;

        struct sym_entry *e = lookup(current, name);
        if (!e)
        {
            fprintf(stderr, "%s:%d: semantic error: undeclared function %s\n", filename, node->kids[0]->leaf->lineno, name); // look up function and ensure it has been defined before
            *symtab_err_flag = 1;
        }
        else
        {
            node->kids[0]->symbol = e; // assign symbol info to AST node
        }
        break;
    }
    }

    // Leaf Identifiers
    if (node->leaf && node->leaf->category == IDENT)
    {
        char *name = node->leaf->text;

        struct sym_entry *e = lookup(current, name);
        if (!e)
        {
            fprintf(stderr, "%s:%d: semantic error: undeclared variable %s\n", filename, node->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            node->symbol = e;
        }
    }

    for (int i = 0; i < node->nkids; i++)
        build_symtab(node->kids[i], current, symtab_err_flag, filename);
}

void insert_parameters(struct tree *node, struct sym_table *st, int *symtab_err_flag, char *filename)
{
    if (!node)
        return;

    if (node->prodrule == PR_FUNCTION_VAR_DECL)
    {
        char *name = node->kids[0]->leaf->text;

        if (lookup_current(st, name))
        {
            fprintf(stderr, "%s:%d: semantic error: duplicate parameter %s\n", filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(st, name);
        }
        return;
    }

    // recursive list
    for (int i = 0; i < node->nkids; i++)
        insert_parameters(node->kids[i], st, symtab_err_flag, filename);
}

void print_scope(struct sym_table *st, int level)
{
    printf("Level %d, Symbol table for %s:\n", level, st->scope_name);

    for (int i = 0; i < st->nBuckets; i++)
    {
        struct sym_entry *e = st->tbl[i];

        while (e)
        {
            printf("  %s, type: %d\n", e->name, e->type->basetype);
            e = e->next;
        }
    }
}

void print_symtab(struct sym_table *st, int level)
{
    if (!st)
        return;

    // print current scope
    print_scope(st, level);

    // print all children
    struct sym_table *child = st->child;
    while (child)
    {
        print_symtab(child, level + 1);
        child = child->sibling;
    }
}