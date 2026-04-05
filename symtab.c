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
            typeptr t = malloc(sizeof(*t));
            t->basetype = FUNC_TYPE;
            insert(global, name, t);
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

void insert(struct sym_table *st, char *name, typeptr t)
{
    /*if (t)
    {
        printf("%s ", name);
        printf("%d\n", t->basetype);
    }*/
    int i = hash(st, name);

    struct sym_entry *e = malloc(sizeof(struct sym_entry));
    e->name = strdup(name);
    e->type = t;
    e->next = st->tbl[i];
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

int infer_type(struct token *node)
{
    switch (node->category)
    {
    case INT:
    {
        return INT_TYPE;
    }
    default:
        return NULL_TYPE;
    }
}

void build_symtab(struct tree *node, struct sym_table *current, int *symtab_err_flag, char *filename)
{
    if (!node)
        return;

    switch (node->prodrule)
    {
    // variable declaration
    // val x: Int;
    case PR_GLOBAL_VAR_DECL:
    case PR_FUN_BODY_VAR_DECL:
    {
        typeptr t = malloc(sizeof(*t));
        t->basetype = node->kids[3]->leaf->category;

        char *name = node->kids[1]->leaf->text; // finding where IDENT name lives within global variable declarations kids array

        if (lookup_current(current, name))
        { // so if the name appears twice as a declaration in current symbol table, break out for now
            fprintf(stderr, "%s:%d: semantic error: redeclaration of variable %s\n", filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, t); // else insert into current symbol table
        }
        break;
    }
    // variable declaration with assignment
    // var x: Int = 5;
    case PR_GLOBAL_VAR_DECL_ASSIGN:
    case PR_FUN_BODY_VAR_DECL_ASSIGN:
    {
        // immediately handle type checking
        if (node->kids[3]->leaf->category == INT_TYPE && node->kids[5]->leaf->category != INT)
        {
            fprintf(stderr, "%s:%d: semantic error: type mismatch of variable %s\n", filename, node->kids[1]->leaf->lineno, node->kids[1]->leaf->text);
            *symtab_err_flag = 1;
        }

        typeptr t = malloc(sizeof(*t));

        t->basetype = node->kids[3]->leaf->category; // IDENT category

        char *name = node->kids[1]->leaf->text; // finding where IDENT name lives within global variable declarations kids array

        if (lookup_current(current, name))
        { // so if the name appears twice as a declaration in current symbol table, break out for now
            fprintf(stderr, "%s:%d: semantic error: redeclaration of variable %s\n", filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, t); // else insert into current symbol table
        }
        break;
    }
    // variable initialization (inferred)
    // val x = 6;
    case PR_GLOBAL_VAR_INIT:
    case PR_FUN_BODY_VAR_INIT:
    {
        typeptr t = malloc(sizeof(*t));

        t->basetype = infer_type(node->kids[3]->leaf);

        char *name = node->kids[1]->leaf->text; // finding where IDENT name lives within global variable declarations kids array

        if (lookup_current(current, name))
        { // so if the name appears twice as a declaration in current symbol table, break out for now
            fprintf(stderr, "%s:%d: semantic error: redeclaration of variable %s\n", filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, t); // else insert into current symbol table
        }
        break;
    }

    // Typed Function Declarations
    // fun foo(<parameter list>): Int {}
    case PR_FUNCTION_DECL_TYPED:
    {
        char *name = node->kids[1]->leaf->text;

        if (lookup_current(current, name))
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of function %s\n",
                    filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
            return;
        }

        // Build function type
        typeptr t = malloc(sizeof(*t));
        t->basetype = FUNC_TYPE;

        t->u.f.name = name;
        t->u.f.defined = 1;

        t->u.f.returntype = malloc(sizeof(*(t->u.f.returntype)));
        t->u.f.returntype->basetype = node->kids[6]->leaf->category;

        t->u.f.nparams = 0;
        t->u.f.parameters = NULL;  // will fill later

        // insert function into current scope FIRST
        insert(current, name, t);

        // Create new scope for function
        struct sym_table *new_scope = mksymtab(16);
        new_scope->parent = current;
        new_scope->sibling = current->child;
        current->child = new_scope;

        // scope name
        char *buf = malloc(strlen("func ") + strlen(name) + 1);
        strcpy(buf, "func ");
        strcat(buf, name);
        new_scope->scope_name = buf;

        // link scope to function type
        t->u.f.st = new_scope;

        // Build params and insert into function scope 
        t->u.f.parameters = build_and_insert_params( node->kids[3], new_scope, &t->u.f.nparams, symtab_err_flag, filename);

        // Traverse function body
        build_symtab(node->kids[7], new_scope, symtab_err_flag, filename);

        return;
    }
    // Untyped Function Declarations
    // fun foo(<parameter list>) {}
    case PR_FUNCTION_DECL_UNTYPED:
    {
        char *name = node->kids[1]->leaf->text;

        if (lookup_current(current, name))
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of function %s\n",
                    filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
            return;
        }

        // Build type
        typeptr t = malloc(sizeof(*t));
        t->basetype = FUNC_TYPE;

        t->u.f.name = name;
        t->u.f.defined = 1;

        // no return type -> NONE_TYPE
        t->u.f.returntype = malloc(sizeof(*(t->u.f.returntype)));
        t->u.f.returntype->basetype = NONE_TYPE;

        t->u.f.nparams = 0;

        // insert function into current scope
        insert(current, name, t);

        // Create new scope for function
        struct sym_table *new_scope = mksymtab(16);
        new_scope->parent = current;
        new_scope->sibling = current->child;
        current->child = new_scope;

        // scope name
        char *buf = malloc(strlen("func ") + strlen(name) + 1);
        strcpy(buf, "func ");
        strcat(buf, name);
        new_scope->scope_name = buf;

        // link scope to function type
        t->u.f.st = new_scope;

        // Build params and insert into function scope 
        t->u.f.parameters = build_and_insert_params( node->kids[3], new_scope, &t->u.f.nparams, symtab_err_flag, filename);

        // Traverse function body
        build_symtab(node->kids[7], new_scope, symtab_err_flag, filename);

        return;
    }
    // Strictly a function declaration
    // fun foo(<parameter list>);
    case PR_FUNCTION_DECL_SEMICOLON:
    {
        char *name = node->kids[1]->leaf->text;

        if (lookup_current(current, name))
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of function %s\n",
                    filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
            return;
        }

        // Build type 
        typeptr t = malloc(sizeof(*t));
        t->basetype = FUNC_TYPE;

        t->u.f.name = name;
        t->u.f.defined = 0; // prototype only

        t->u.f.returntype = malloc(sizeof(*(t->u.f.returntype)));
        t->u.f.returntype->basetype = NONE_TYPE;

        t->u.f.nparams = 0;

        // Only build param list
        t->u.f.parameters = build_param_list_only(
            node->kids[3],
            &t->u.f.nparams
        );

        t->u.f.st = NULL; // no scope for prototype

        insert(current, name, t);

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
    /*if (node->leaf && node->leaf->category == IDENT)
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
    }*/

    for (int i = 0; i < node->nkids; i++)
        build_symtab(node->kids[i], current, symtab_err_flag, filename);
}

void print_scope(struct sym_table *st, int level)
{
    printf("--- Level %d, Symbol table for: %s ---\n", level, st->scope_name);

    for (int i = 0; i < st->nBuckets; i++)
    {
        struct sym_entry *e = st->tbl[i];

        while (e)
        {

            if (e->type)
            {
                switch(e->type->basetype)
                {
                    case BYTE_TYPE:
                        printf("  %s, type: BYTE\n", e->name);
                        break;
                    case SHORT_TYPE:
                        printf("  %s, type: SHORT\n", e->name);
                        break;
                    case INT_TYPE:
                        printf("  %s, type: INT\n", e->name);
                        break;
                    case LONG_TYPE:
                        printf("  %s, type: LONG\n", e->name);
                        break;
                    case FLOAT_TYPE:
                        printf("  %s, type: FLOAT\n", e->name);
                        break;
                    case DOUBLE_TYPE:
                        printf("  %s, type: DOUBLE\n", e->name);
                        break;
                    case BOOLEAN_TYPE:
                        printf("  %s, type: BOOLEAN\n", e->name);
                        break;
                    case STRING_TYPE:
                        printf("  %s, type: STRING\n", e->name);
                        break;
                    case NONE_TYPE:
                        printf("  %s, type: NONE\n", e->name);
                        break;
                    case FUNC_TYPE:
                        printf("  %s\n", e->name);
                        break;
                    default:
                        printf("  %s, type: %d\n", e->name, e->type->basetype);
                        break;
                }
                
            }

            else
                printf("  %s\n", e->name);
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

paramlist build_and_insert_params(struct tree *node, struct sym_table *st, int *count, int *symtab_err_flag, char *filename)
{
    if (!node)
        return NULL;

    paramlist head = NULL;
    paramlist tail = NULL;

    if (node->prodrule == PR_FUNCTION_VAR_DECL)
    {
        char *name = node->kids[0]->leaf->text;

        // Duplicate check
        if (lookup_current(st, name))
        {
            fprintf(stderr, "%s:%d: semantic error: duplicate parameter %s\n",
                    filename,
                    node->kids[0]->leaf->lineno,
                    name);

            *symtab_err_flag = 1;
            return NULL;  // don't add duplicate to list
        }

        // Build type
        typeptr t = malloc(sizeof(*t));
        t->basetype = node->kids[2]->leaf->category;

        // Insert into symbol table
        insert(st, name, t);

        // Build param list node
        struct param *p = malloc(sizeof(struct param));
        p->name = strdup(name);
        p->type = t;
        p->next = NULL;

        (*count)++;

        return p;
    }

    // Recursive traversal
    for (int i = 0; i < node->nkids; i++)
    {
        paramlist sub = build_and_insert_params(
            node->kids[i],
            st,
            count,
            symtab_err_flag,
            filename
        );

        if (!sub)
            continue;

        if (!head)
        {
            head = sub;
            tail = sub;
        }
        else
        {
            tail->next = sub;
        }

        // Move tail to end
        while (tail->next)
            tail = tail->next;
    }

    return head;
}

paramlist build_param_list_only(struct tree *node, int *count)
{
    if (!node)
        return NULL;

    paramlist head = NULL;
    paramlist tail = NULL;

    // Base case: parameter declaration
    if (node->prodrule == PR_FUNCTION_VAR_DECL)
    {
        struct param *p = malloc(sizeof(struct param));

        char *name = node->kids[0]->leaf->text;

        typeptr t = malloc(sizeof(*t));
        t->basetype = node->kids[2]->leaf->category;

        p->name = strdup(name);
        p->type = t;
        p->next = NULL;

        (*count)++;

        return p;
    }

    // Recursive traversal for parameter lists
    for (int i = 0; i < node->nkids; i++)
    {
        paramlist sub = build_param_list_only(node->kids[i], count);

        if (!sub)
            continue;

        if (!head)
        {
            head = sub;
            tail = sub;
        }
        else
        {
            tail->next = sub;
        }

        // Move tail to end of list
        while (tail->next)
            tail = tail->next;
    }

    return head;
}