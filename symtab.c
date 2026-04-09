#include "symtab.h"
#include "k0gram.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// predefined functions in k0.
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

struct sym_table *mksymtab_global(int size)
{
    struct sym_table *global = mksymtab(size);
    global->scope_name = strdup("global");

    int n = sizeof(predefined_functions) / sizeof(predefined_functions[0]);

    for (int i = 0; i < n; i++)
    {
        char *name = predefined_functions[i];

        if (!lookup_current(global, name))
        {
            typeptr t = malloc(sizeof(*t));
            memset(t, 0, sizeof(*t));

            // set defaults that may or may not get overwritten
            t->basetype = FUNC_TYPE;
            t->u.f.name = strdup(name);
            t->u.f.defined = 1;
            t->u.f.returntype = malloc(sizeof(*(t->u.f.returntype)));
            t->u.f.nparams = 0;
            t->u.f.parameters = NULL;
            t->u.f.st = NULL;

            // assign return types, these could change
            // hardcoding print and println to both accept one string argument for now
            if (!strcmp(name, "print") || !strcmp(name, "println"))
            {
                t->u.f.returntype->basetype = NONE_TYPE;

                t->u.f.nparams = 1;

                // build single parameter: String
                paramlist p = malloc(sizeof(struct param));

                p->name = strdup("s"); // arbitrary name
                p->type = malloc(sizeof(*(p->type)));
                p->type->basetype = STRING_TYPE;

                p->next = NULL;

                t->u.f.parameters = p;
            }
            else if (!strcmp(name, "readln"))
            {
                t->u.f.returntype->basetype = STRING_TYPE;
            }
            else if (!strcmp(name, "get"))
            {
                t->u.f.returntype->basetype = STRING_TYPE;
            }
            else if (!strcmp(name, "equals"))
            {
                t->u.f.returntype->basetype = BOOL_TYPE;
            }
            else if (!strcmp(name, "length"))
            {
                t->u.f.returntype->basetype = INT_TYPE;
            }
            else if (!strcmp(name, "toString") || !strcmp(name, "valueOf"))
            {
                t->u.f.returntype->basetype = STRING_TYPE;
            }
            else if (!strcmp(name, "substring"))
            {
                t->u.f.returntype->basetype = STRING_TYPE;
            }
            else if (!strcmp(name, "nextInt"))
            {
                t->u.f.returntype->basetype = INT_TYPE;
            }
            else if (!strcmp(name, "abs") || !strcmp(name, "max") || !strcmp(name, "min"))
            {
                t->u.f.returntype->basetype = INT_TYPE;
            }
            else if (!strcmp(name, "pow") || !strcmp(name, "cos") ||
                     !strcmp(name, "sin") || !strcmp(name, "tan"))
            {
                t->u.f.returntype->basetype = FLOAT_TYPE;
            }
            else
            {
                t->u.f.returntype->basetype = NONE_TYPE; // fallback if we don't know it
            }

            insert(global, name, t, 0, 0);
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

void insert(struct sym_table *st, char *name, typeptr t, int is_mutable, int is_nullable)
{
    int i = hash(st, name);

    struct sym_entry *e = malloc(sizeof(struct sym_entry));
    e->name = strdup(name);
    e->type = t;
    e->is_mutable = is_mutable;
    e->is_nullable = is_nullable;
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

        char *name = node->kids[1]->leaf->text;

        int is_mutable = (node->kids[0]->leaf->category == VAR);
        int is_nullable = 0; // we know it's not nullable

        if (lookup_current(current, name))
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of variable %s\n",
                    filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, t, is_mutable, is_nullable);
        }
        break;
    }
    // variable declaration with assignment
    // var x: Int = 5;
    case PR_GLOBAL_VAR_DECL_ASSIGN:
    case PR_FUN_BODY_VAR_DECL_ASSIGN:
    {
        // immediately handle type checking
        typeptr declared = malloc(sizeof(*declared));
        declared->basetype = node->kids[3]->leaf->category;

        typeptr expr_t = typecheck_expr(node->kids[5], current,
                                        symtab_err_flag, filename);

        if (!type_equal(declared, expr_t))
        {
            fprintf(stderr, "%s:%d: semantic error: type mismatch of variable %s\n",
                    filename,
                    node->kids[1]->leaf->lineno,
                    node->kids[1]->leaf->text);
            *symtab_err_flag = 1;
        }

        typeptr t = malloc(sizeof(*t));

        t->basetype = declared->basetype; // passed type checks, so make the basetype the declared type

        char *name = node->kids[1]->leaf->text;
        int is_mutable = (node->kids[0]->leaf->category == VAR);
        int is_nullable = 0; // we know it's not nullable

        if (lookup_current(current, name))
        { // so if the name appears twice as a declaration in current symbol table, break out for now
            fprintf(stderr, "%s:%d: semantic error: redeclaration of variable %s\n", filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, t, is_mutable, is_nullable); // else insert into current symbol table
        }
        break;
    }
    // nullable variable declaration
    // val x: Int?;
    case PR_GLOBAL_VAR_DECL_NULLABLE:
    case PR_FUN_BODY_VAR_DECL_NULLABLE:
    {
        typeptr t = malloc(sizeof(*t));
        t->basetype = node->kids[3]->leaf->category;

        char *name = node->kids[1]->leaf->text;

        // assign mutability and nullability
        int is_mutable = (node->kids[0]->leaf->category == VAR);
        int is_nullable = 1;

        if (lookup_current(current, name))
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of variable %s\n",
                    filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, t, is_mutable, is_nullable);
        }
        break;
    }
    // nullable variable declaration with assignment
    // var x: Int? = 5;
    case PR_GLOBAL_VAR_DECL_ASSIGN_NULLABLE:
    case PR_FUN_BODY_VAR_DECL_ASSIGN_NULLABLE:
    {
        typeptr declared = malloc(sizeof(*declared));
        declared->basetype = node->kids[3]->leaf->category;

        typeptr expr_t = typecheck_expr(node->kids[6], current,
                                        symtab_err_flag, filename);

        /* allow null if you support it later */
        if (expr_t && !type_equal(declared, expr_t))
        {
            fprintf(stderr, "%s:%d: semantic error: type mismatch of variable %s\n",
                    filename,
                    node->kids[1]->leaf->lineno,
                    node->kids[1]->leaf->text);
            *symtab_err_flag = 1;
        }

        typeptr t = malloc(sizeof(*t));

        t->basetype = declared->basetype;

        char *name = node->kids[1]->leaf->text;
        int is_mutable = (node->kids[0]->leaf->category == VAR);
        int is_nullable = 1;

        if (lookup_current(current, name))
        { // so if the name appears twice as a declaration in current symbol table, break out for now
            fprintf(stderr, "%s:%d: semantic error: redeclaration of variable %s\n", filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, t, is_mutable, is_nullable); // else insert into current symbol table
        }
        break;
    }
    // variable initialization (inferred)
    // val x = 6;
    case PR_GLOBAL_VAR_INIT:
    case PR_FUN_BODY_VAR_INIT:
    {
        typeptr expr_t = typecheck_expr(node->kids[3], current,
                                        symtab_err_flag, filename);

        if (!expr_t)
        {
            *symtab_err_flag = 1;
            break;
        }

        // only INT, CHAR, DOUBLE/REAL allowed
        int bt = expr_t->basetype;
        if (bt != INT_TYPE && bt != CHAR_TYPE && bt != DOUBLE_TYPE)
        {
            fprintf(stderr,
                    "%s:%d: semantic error: inferred type not allowed for %s\n",
                    filename,
                    node->kids[1]->leaf->lineno,
                    node->kids[1]->leaf->text);
            *symtab_err_flag = 1;

            /* still assign something to avoid crashing later */
            bt = NONE_TYPE;
        }

        typeptr t = malloc(sizeof(*t));
        t->basetype = bt;

        char *name = node->kids[1]->leaf->text;
        int is_mutable = (node->kids[0]->leaf->category == VAR);
        int is_nullable = 0;

        if (lookup_current(current, name))
        {
            fprintf(stderr,
                    "%s:%d: semantic error: redeclaration of variable %s\n",
                    filename,
                    node->kids[1]->leaf->lineno,
                    name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, t, is_mutable, is_nullable);
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
        t->u.f.parameters = NULL; // will fill later

        // insert function into current scope
        insert(current, name, t, 0, 0);

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
        t->u.f.parameters = build_and_insert_params(node->kids[3], new_scope, &t->u.f.nparams, symtab_err_flag, filename);

        // Traverse function body
        build_symtab(node->kids[7], new_scope, symtab_err_flag, filename);

        return;
    }
    case PR_FUNCTION_DECL_TYPED_NULLABLE:
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
        t->u.f.parameters = NULL; // will fill later

        // insert function into current scope FIRST
        // return type is nullable, so send 1 for nullable
        insert(current, name, t, 0, 1);

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
        t->u.f.parameters = build_and_insert_params(node->kids[3], new_scope, &t->u.f.nparams, symtab_err_flag, filename);

        // Traverse function body
        build_symtab(node->kids[8], new_scope, symtab_err_flag, filename);

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
        insert(current, name, t, 0, 0);

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
        t->u.f.parameters = build_and_insert_params(node->kids[3], new_scope, &t->u.f.nparams, symtab_err_flag, filename);

        // Traverse function body
        build_symtab(node->kids[7], new_scope, symtab_err_flag, filename);

        return;
    }

    // Assignment and arithmetic assignment
    case PR_ASSIGNMENT_ASSIGN:
    case PR_ASSIGNMENT_PLUS:
    case PR_ASSIGNMENT_MINUS:
    {
        char *name = node->kids[0]->leaf->text;

        // lookup if variable exists
        struct sym_entry *e = lookup(current, name);
        if (!e)
        {
            fprintf(stderr, "%s:%d: semantic error: undeclared variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        // it exists, lookup if it is mutable
        if (!e->is_mutable)
        {
            fprintf(stderr, "%s:%d: semantic error: attempt to modify immutable variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        // it is mutable and exists, so type check the right hand side
        typeptr rhs = typecheck_expr(node->kids[2], current, symtab_err_flag, filename);

        if (rhs && !type_equal(e->type, rhs))
        {
            fprintf(stderr, "%s:%d: semantic error: type mismatch in assignment to %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        break;
    }

    // Function call
    case PR_FUNCTION_CALL:
    {

        typeptr t = check_function_call(node, current, symtab_err_flag, filename);
        (void)t; // all error messages and actions happen in check_function_call, so just doing something with t here to avoid unused warning

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
    printf("\n--- Level %d, Symbol table for: %s ---\n", level, st->scope_name);

    for (int i = 0; i < st->nBuckets; i++)
    {
        struct sym_entry *e = st->tbl[i];

        while (e)
        {
            if (e->type)
            {
                // Function case
                if (e->type->basetype == FUNC_TYPE)
                {
                    printf("  func %s\n", e->name);
                }
                else
                {
                    // Variable case
                    const char *mut = e->is_mutable ? "var" : "val";
                    const char *nullable = e->is_nullable ? "?" : "";

                    switch (e->type->basetype)
                    {
                    case BYTE_TYPE:
                        printf("  %s %s, type: Byte%s\n", mut, e->name, nullable);
                        break;
                    case SHORT_TYPE:
                        printf("  %s %s, type: Short%s\n", mut, e->name, nullable);
                        break;
                    case INT_TYPE:
                        printf("  %s %s, type: Int%s\n", mut, e->name, nullable);
                        break;
                    case LONG_TYPE:
                        printf("  %s %s, type: Long%s\n", mut, e->name, nullable);
                        break;
                    case FLOAT_TYPE:
                        printf("  %s %s, type: Float%s\n", mut, e->name, nullable);
                        break;
                    case DOUBLE_TYPE:
                        printf("  %s %s, type: Double%s\n", mut, e->name, nullable);
                        break;
                    case BOOLEAN_TYPE:
                        printf("  %s %s, type: Boolean%s\n", mut, e->name, nullable);
                        break;
                    case STRING_TYPE:
                        printf("  %s %s, type: String%s\n", mut, e->name, nullable);
                        break;
                    case CHAR_TYPE:
                        printf("  %s %s, type: Char%s\n", mut, e->name, nullable);
                        break;
                    case NONE_TYPE:
                        printf("  %s %s, type: NONE%s\n", mut, e->name, nullable);
                        break;
                    default:
                        printf("  %s %s, type: %d%s\n", mut, e->name, e->type->basetype, nullable);
                        break;
                    }
                }
            }
            else
            {
                // predefined without types
                printf("  %s\n", e->name);
            }

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
            return NULL; // don't add duplicate to list
        }

        // Build type
        typeptr t = malloc(sizeof(*t));
        t->basetype = node->kids[2]->leaf->category;

        // Insert into symbol table
        insert(st, name, t, 0, 0);

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
            filename);

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

typeptr typecheck_expr(struct tree *node, struct sym_table *current,
                       int *err, char *filename)
{
    if (!node)
        return NULL;

    switch (node->prodrule)
    {

    // literals
    case INT:
    case REAL:
    case STRING:
    case MULTI_STRING:
    case CHAR:
    case K_TRUE:
    case K_FALSE:
    {
        return node->leaf->type;
    }

    // identifiers
    case IDENT:
    {
        char *name = node->leaf->text;
        struct sym_entry *e = lookup(current, name);

        if (!e)
        {
            fprintf(stderr, "%s:%d: undeclared variable %s\n",
                    filename, node->leaf->lineno, name);
            *err = 1;
            return NULL;
        }

        node->symbol = e;
        return e->type;
    }

    // parentheses: ( expr )
    case PR_PRIMARY_PAREN:
        return typecheck_expr(node->kids[1], current, err, filename);

    // arithmetic
    case PR_ADDITIVE_PLUS:
    case PR_ADDITIVE_MINUS:
    case PR_MULT_MUL:
    case PR_MULT_DIV:
    case PR_MULT_MOD:
    {
        typeptr left = typecheck_expr(node->kids[0], current, err, filename);
        typeptr right = typecheck_expr(node->kids[2], current, err, filename);

        if (!left || !right)
            return NULL;

        if (!type_equal(left, right))
        {
            fprintf(stderr, "%s:%d: type mismatch in binary expression\n",
                    filename, node->kids[1]->leaf->lineno);
            *err = 1;
            return NULL;
        }

        return left;
    }

    // unary
    case PR_UNARY_MINUS:
        return typecheck_expr(node->kids[1], current, err, filename);

    case PR_UNARY_NOT:
    {
        typeptr t = typecheck_expr(node->kids[1], current, err, filename);

        if (!t)
            return NULL;

        if (t->basetype != BOOL_TYPE)
        {
            fprintf(stderr, "%s:%d: ! requires boolean\n",
                    filename, node->kids[0]->leaf->lineno);
            *err = 1;
            return NULL;
        }

        return t;
    }

    // function call
    case PR_FUNCTION_CALL:
    {
        char *name = node->kids[0]->leaf->text;
        struct sym_entry *e = lookup(current, name);

        if (!e || e->type->basetype != FUNC_TYPE)
        {
            fprintf(stderr, "%s:%d: invalid function call %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *err = 1;
            return NULL;
        }

        // on succes, will return return type of the function, else returns NULL
        return check_function_call(node, current, err, filename);
    }
    }
    return NULL;
}

int type_equal(typeptr a, typeptr b)
{
    if (!a || !b)
        return 0;
    return a->basetype == b->basetype;
}

typeptr check_function_call(struct tree *node, struct sym_table *current, int *err, char *filename)
{
    if (!node)
        return NULL;

    char *name = node->kids[0]->leaf->text;

    struct sym_entry *e = lookup(current, name);
    if (!e || e->type->basetype != FUNC_TYPE)
    {
        fprintf(stderr, "%s:%d: semantic error: invalid function call %s\n",
                filename, node->kids[0]->leaf->lineno, name);
        *err = 1;
        return NULL;
    }

    typeptr ftype = e->type;
    paramlist param = ftype->u.f.parameters; // parameters noted in the declaration of the function from earlier
    struct tree *arglist = node->kids[2];    // current arg list we are looking at in the function call

    int arg_index = 1;

    // Traverse param list and provided args
    while (param || arglist)
    {
        // Too many arguments
        if (!param)
        {
            fprintf(stderr,
                    "%s:%d: semantic error: too many arguments in call to %s\n",
                    filename,
                    node->kids[0]->leaf->lineno,
                    name);
            *err = 1;
            break;
        }

        // Too few arguments
        if (!arglist)
        {
            fprintf(stderr,
                    "%s:%d: semantic error: too few arguments in call to %s\n",
                    filename,
                    node->kids[0]->leaf->lineno,
                    name);
            *err = 1;
            break;
        }

        struct tree *arg_node;

        // unwrap argument list node
        if (arglist->prodrule == PR_CALL_VALUES_RECUR)
        {
            arg_node = arglist->kids[2]; // rightmost argument
            arglist = arglist->kids[0];  // continue left
        }
        else
        {
            arg_node = arglist; // single argument
            arglist = NULL;
        }

        typeptr arg_t = typecheck_expr(arg_node, current, err, filename);

        if (!arg_t || !type_equal(arg_t, param->type))
        {
            fprintf(stderr,
                    "%s:%d: semantic error: argument %d of %s has wrong type\n",
                    filename,
                    node->kids[0]->leaf->lineno,
                    arg_index,
                    name);
            *err = 1;
        }

        param = param->next;
        arg_index++;
    }

    return ftype->u.f.returntype;
}