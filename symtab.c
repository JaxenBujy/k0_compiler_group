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
    "tan",       // Math.tan
    "arrayOf"};

// initializes and allocates a new symbol table
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

// returns the global symbol table of any k0 program, primarily involving all the prebuilt functions in k0
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
                t->u.f.returntype->basetype = BOOLEAN_TYPE;
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
            else if (!strcmp(name, "arrayOf"))
            {
                t->u.f.returntype->basetype = NONE_TYPE; // placeholder since the return type gets assigned later
                // arrayOf accepts any number of arguments of the same type.
                // We leave nparams = 0 and parameters = NULL
            }
            else
            {
                t->u.f.returntype->basetype = NONE_TYPE; // fallback if we don't know it
            }

            insert(global, name, t, 0, 0, 0);
        }
    }

    return global;
}

// hash function for placement into buckets
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

// insert a symbol into the current scope
void insert(struct sym_table *st, char *name, typeptr t, int is_mutable, int is_nullable, int is_declared)
{
    int i = hash(st, name);

    struct sym_entry *e = malloc(sizeof(struct sym_entry));
    e->name = strdup(name);
    e->type = t;
    e->is_mutable = is_mutable;
    e->is_nullable = is_nullable;
    e->is_declared = is_declared;
    e->next = st->tbl[i];
    st->tbl[i] = e;

    st->nEntries++;
}

// lookup a symbol in any scope
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

// lookup a symbol in the current scope
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

// recursively traverses through the AST, generating the symbol table.
// prints some error messages relating to obvious redeclarations and undefined variables
// delegates much type checking to typecheckexpr for nested cases
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
        struct tree *type_node = node->kids[3];
        typeptr t = type_from_ast_node(type_node);

        char *name = node->kids[1]->leaf->text;
        int is_mutable = (node->kids[0]->leaf->category == VAR);
        int is_declared = 1;
        int is_nullable = 0;

        if (lookup_current(current, name))
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of variable %s\n",
                    filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, t, is_mutable, is_nullable, is_declared);
        }
        break;
    }
    // variable declaration with assignment
    // var x: Int = 5;
    case PR_GLOBAL_VAR_DECL_ASSIGN:
    case PR_FUN_BODY_VAR_DECL_ASSIGN:
    {
        struct tree *type_node = node->kids[3];
        typeptr declared = type_from_ast_node(type_node);

        typeptr expr_t = typecheck_expr(node->kids[5], current, symtab_err_flag, filename);

        if (expr_t && expr_t->basetype == NULL_TYPE)
        {
            fprintf(stderr, "%s:%d: semantic error: cannot assign null to non-nullable variable %s\n",
                    filename, node->kids[1]->leaf->lineno, node->kids[1]->leaf->text);
            *symtab_err_flag = 1;
        }
        else if (expr_t && !is_assignable(declared, expr_t, node->kids[5]))
        {
            fprintf(stderr, "%s:%d: semantic error: type mismatch in variable declaration of %s\n",
                    filename, node->kids[1]->leaf->lineno, node->kids[1]->leaf->text);
            *symtab_err_flag = 1;
        }

        // Use the declared type for insertion (already built)
        char *name = node->kids[1]->leaf->text;
        int is_mutable = (node->kids[0]->leaf->category == VAR);
        int is_declared = 0; // not just strictly declared, but also defined, so turn off
        int is_nullable = 0;

        if (lookup_current(current, name))
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of variable %s\n",
                    filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, declared, is_mutable, is_nullable, is_declared);
        }
        break;
    }
    // nullable variable declaration
    // val x: Int?;
    case PR_GLOBAL_VAR_DECL_NULLABLE:
    case PR_FUN_BODY_VAR_DECL_NULLABLE:
    {
        struct tree *type_node = node->kids[3];
        typeptr t = type_from_ast_node(type_node);

        char *name = node->kids[1]->leaf->text;
        int is_mutable = (node->kids[0]->leaf->category == VAR);
        int is_declared = 1; // strictly declared for right now, so turn on
        int is_nullable = 1;

        if (lookup_current(current, name))
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of variable %s\n",
                    filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, t, is_mutable, is_nullable, is_declared);
        }
        break;
    }
    // nullable variable declaration with assignment
    // var x: Int? = 5;
    case PR_GLOBAL_VAR_DECL_ASSIGN_NULLABLE:
    case PR_FUN_BODY_VAR_DECL_ASSIGN_NULLABLE:
    {
        struct tree *type_node = node->kids[3];
        typeptr declared = type_from_ast_node(type_node);

        typeptr expr_t = typecheck_expr(node->kids[6], current, symtab_err_flag, filename);

        if (expr_t && expr_t->basetype == NULL_TYPE)
        {
            // null allowed
        }
        else if (expr_t && !is_assignable(declared, expr_t, node->kids[6]))
        {
            fprintf(stderr, "%s:%d: semantic error: type mismatch in nullable variable declaration of %s\n",
                    filename, node->kids[1]->leaf->lineno, node->kids[1]->leaf->text);
            *symtab_err_flag = 1;
        }

        char *name = node->kids[1]->leaf->text;
        int is_mutable = (node->kids[0]->leaf->category == VAR);
        int is_declared = 0; // 0 because it is not strictly declared, it is also defined
        int is_nullable = 1;

        if (lookup_current(current, name))
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of variable %s\n",
                    filename, node->kids[1]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        else
        {
            insert(current, name, declared, is_mutable, is_nullable, is_declared);
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
        if (bt != INT_TYPE && bt != CHAR_TYPE && bt != DOUBLE_TYPE && bt != ARRAY_TYPE)
        {
            fprintf(stderr,
                    "%s:%d: semantic error: inferred type not allowed for %s\n",
                    filename,
                    node->kids[1]->leaf->lineno,
                    node->kids[1]->leaf->text);
            *symtab_err_flag = 1;

            // still assign something to avoid crashing later
            bt = NONE_TYPE;
        }

        typeptr t = malloc(sizeof(*t));
        t->basetype = bt;

        char *name = node->kids[1]->leaf->text;
        int is_mutable = (node->kids[0]->leaf->category == VAR);
        int is_declared = 0;
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
            insert(current, name, t, is_mutable, is_nullable, is_declared);
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
        memset(t, 0, sizeof(*t));
        t->basetype = FUNC_TYPE;
        t->u.f.name = name;
        t->u.f.defined = 1;

        // Return type (may be array)
        struct tree *ret_type_node = node->kids[6];
        t->u.f.returntype = type_from_ast_node(ret_type_node);
        t->u.f.nparams = 0;
        t->u.f.parameters = NULL;

        insert(current, name, t, 0, 0, 0);

        // Create function scope
        struct sym_table *new_scope = mksymtab(16);
        new_scope->parent = current;
        new_scope->sibling = current->child;
        current->child = new_scope;

        char *buf = malloc(strlen("func ") + strlen(name) + 1);
        strcpy(buf, "func ");
        strcat(buf, name);
        new_scope->scope_name = buf;
        t->u.f.st = new_scope;

        t->u.f.parameters = build_and_insert_params(node->kids[3], new_scope, &t->u.f.nparams, symtab_err_flag, filename);

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

        typeptr t = malloc(sizeof(*t));
        memset(t, 0, sizeof(*t));
        t->basetype = FUNC_TYPE;
        t->u.f.name = name;
        t->u.f.defined = 1;

        struct tree *ret_type_node = node->kids[6];
        t->u.f.returntype = type_from_ast_node(ret_type_node);
        t->u.f.nparams = 0;
        t->u.f.parameters = NULL;

        // Return type is nullable, so pass is_nullable = 1
        insert(current, name, t, 0, 1, 0);

        struct sym_table *new_scope = mksymtab(16);
        new_scope->parent = current;
        new_scope->sibling = current->child;
        current->child = new_scope;

        char *buf = malloc(strlen("func ") + strlen(name) + 1);
        strcpy(buf, "func ");
        strcat(buf, name);
        new_scope->scope_name = buf;
        t->u.f.st = new_scope;

        t->u.f.parameters = build_and_insert_params(node->kids[3], new_scope, &t->u.f.nparams, symtab_err_flag, filename);

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

        typeptr t = malloc(sizeof(*t));
        memset(t, 0, sizeof(*t));
        t->basetype = FUNC_TYPE;
        t->u.f.name = name;
        t->u.f.defined = 1;

        t->u.f.returntype = malloc(sizeof(*(t->u.f.returntype)));
        t->u.f.returntype->basetype = NONE_TYPE;
        t->u.f.nparams = 0;

        insert(current, name, t, 0, 0, 0);

        struct sym_table *new_scope = mksymtab(16);
        new_scope->parent = current;
        new_scope->sibling = current->child;
        current->child = new_scope;

        char *buf = malloc(strlen("func ") + strlen(name) + 1);
        strcpy(buf, "func ");
        strcat(buf, name);
        new_scope->scope_name = buf;
        t->u.f.st = new_scope;

        t->u.f.parameters = build_and_insert_params(node->kids[3], new_scope, &t->u.f.nparams, symtab_err_flag, filename);

        build_symtab(node->kids[5], new_scope, symtab_err_flag, filename);
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

        // it exists, lookup if it is mutable and has already been given a value (!e->is_declared)
        if (!e->is_mutable && !e->is_declared)
        {
            fprintf(stderr, "%s:%d: semantic error: attempt to modify immutable variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        // it is mutable and exists, so type check the right hand side
        typeptr rhs = typecheck_expr(node->kids[2], current, symtab_err_flag, filename);

        // Handle null assignment specially
        if (rhs && rhs->basetype == NULL_TYPE)
        {
            if (!e->is_nullable)
            {
                fprintf(stderr, "%s:%d: semantic error: assigning null to non-nullable variable %s\n",
                        filename, node->kids[0]->leaf->lineno, name);
                *symtab_err_flag = 1;
            }
            // else assignment of null to nullable variable is OK
        }
        else if (rhs && !is_assignable(e->type, rhs, node->kids[2])) // can tighten this to type_equal if it causes issues later
        {
            fprintf(stderr, "%s:%d: semantic error: type mismatch in assignment to %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }

        // Nullability check for non-null expressions (already handled null case above)
        if (rhs && rhs->basetype != NULL_TYPE && !e->is_nullable && is_nullable_expr(node->kids[2], current))
        {
            fprintf(stderr, "%s:%d: semantic error: assigning nullable value to non-nullable variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
        }
        e->is_declared = 0; // by making it here, variable was assigned something and therefore not strictly declared anymore
        break;
    }
    case PR_UNARY_INC:
    {
        // must be an identifier (or l-value)
        char *name = node->kids[1]->leaf->text;

        struct sym_entry *e = lookup(current, name);
        if (!e)
        {
            fprintf(stderr, "%s:%d: semantic error: undeclared variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        if (!e->is_mutable)
        {
            fprintf(stderr, "%s:%d: semantic error: cannot increment immutable variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        if (!is_numeric_type(e->type->basetype))
        {
            fprintf(stderr, "%s:%d: semantic error: ++ requires numeric type for %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }
        if (e->is_nullable)
        {
            fprintf(stderr, "%s:%d: semantic error: cannot increment nullable variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        node->type = e->type;
        break;
    }
    case PR_POST_FIX_INC:
    {
        // must be an identifier (or l-value)
        char *name = node->kids[0]->leaf->text;

        struct sym_entry *e = lookup(current, name);
        if (!e)
        {
            fprintf(stderr, "%s:%d: semantic error: undeclared variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        if (!e->is_mutable)
        {
            fprintf(stderr, "%s:%d: semantic error: cannot increment immutable variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        if (!is_numeric_type(e->type->basetype))
        {
            fprintf(stderr, "%s:%d: semantic error: ++ requires numeric type for %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }
        if (e->is_nullable)
        {
            fprintf(stderr, "%s:%d: semantic error: cannot increment nullable variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        node->type = e->type;
        break;
    }
    case PR_UNARY_DEC:
    {
        // must be an identifier (or l-value)
        char *name = node->kids[1]->leaf->text;

        struct sym_entry *e = lookup(current, name);
        if (!e)
        {
            fprintf(stderr, "%s:%d: semantic error: undeclared variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        if (!e->is_mutable)
        {
            fprintf(stderr, "%s:%d: semantic error: cannot decrement immutable variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        if (!is_numeric_type(e->type->basetype))
        {
            fprintf(stderr, "%s:%d: semantic error: -- requires numeric type for %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        if (e->is_nullable)
        {
            fprintf(stderr, "%s:%d: semantic error: cannot decrement nullable variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        node->type = e->type;
        break;
    }
    case PR_POST_FIX_DEC:
    {
        // must be an identifier (or l-value)
        char *name = node->kids[0]->leaf->text;

        struct sym_entry *e = lookup(current, name);
        if (!e)
        {
            fprintf(stderr, "%s:%d: semantic error: undeclared variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        if (!e->is_mutable)
        {
            fprintf(stderr, "%s:%d: semantic error: cannot decrement immutable variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        if (!is_numeric_type(e->type->basetype))
        {
            fprintf(stderr, "%s:%d: semantic error: -- requires numeric type for %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        if (e->is_nullable)
        {
            fprintf(stderr, "%s:%d: semantic error: cannot decrement nullable variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }

        node->type = e->type;
        break;
    }
    case PR_POST_FIX_NN_ASSERT: // !! makes expression non-nullable
    {
        char *name = node->kids[0]->leaf->text;
        struct sym_entry *e = lookup(current, name);
        if (!e)
        {
            fprintf(stderr, "%s:%d: semantic error: undeclared variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }
        make_non_nullable(e);
        break;
    }
    case PR_POST_FIX_NULLABLE: // ? makes expression nullable
    {
        char *name = node->kids[0]->leaf->text;
        struct sym_entry *e = lookup(current, name);
        if (!e)
        {
            fprintf(stderr, "%s:%d: semantic error: undeclared variable %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *symtab_err_flag = 1;
            break;
        }
        make_nullable(e);
        break;
    }
    case PR_POST_FIX_INDEX:
    {
        typeptr t = typecheck_expr(node, current, symtab_err_flag, filename);
        (void)t;
        break;
    }
    case PR_IF_SIMPLE:
    case PR_IF_ELSE:
    case PR_WHILE:
    {
        // The condition is the third child (index 2): IF LPAREN expr RPAREN, WHILE LPAREN expr RPAREN
        struct tree *cond = node->kids[2];
        typeptr cond_type = typecheck_expr(cond, current, symtab_err_flag, filename);
        if (cond_type && cond_type->basetype != BOOLEAN_TYPE)
        {
            fprintf(stderr, "%s:%d: semantic error: if condition must be Boolean\n",
                    filename, cond->leaf ? cond->leaf->lineno : node->kids[0]->leaf->lineno);
            *symtab_err_flag = 1;
        }
        // Nullability check for operands inside the condition is already performed
        // by typecheck_expr when it processes relational/equality operators.
        break;
    }
    // for loops
    case PR_FOR_IDENT_IDENT:
    case PR_FOR_LITERAL_IDENT:
    case PR_FOR_IDENT_LITERAL:
    case PR_FOR_LITERAL_LITERAL:
    {
        // Loop variable name (third child, index 2)
        struct tree *loop_var_node = node->kids[2];
        char *var_name = loop_var_node->leaf->text;
        struct sym_entry *e = lookup_current(current, var_name);

        // Start expression (fifth child, index 4)
        struct tree *start_node = node->kids[4];
        // End expression (seventh child, index 6)
        struct tree *end_node = node->kids[6];

        // If loop variable has been
        if (e)
        {
            fprintf(stderr, "%s:%d: semantic error: redeclaration of loop variable %s\n",
                    filename, loop_var_node->leaf->lineno, var_name);
            *symtab_err_flag = 1;
            // Continue checking anyway
        }

        // Type‑check start and end expressions
        typeptr start_type = NULL;
        typeptr end_type = NULL;

        // Determine start expression type
        if (node->prodrule == PR_FOR_IDENT_IDENT || node->prodrule == PR_FOR_IDENT_LITERAL)
        {
            // start is an IDENT
            char *start_name = start_node->leaf->text;
            struct sym_entry *start_e = lookup(current, start_name);
            if (!start_e)
            {
                fprintf(stderr, "%s:%d: semantic error: undeclared variable %s in for-loop range\n",
                        filename, start_node->leaf->lineno, start_name);
                *symtab_err_flag = 1;
            }
            else
            {
                start_type = start_e->type;
                if (start_e->is_nullable)
                {
                    fprintf(stderr, "%s:%d: semantic error: nullable variable %s not allowed in for-loop range\n",
                            filename, start_node->leaf->lineno, start_name);
                    *symtab_err_flag = 1;
                }
            }
        }
        else
        {
            // start is a literal
            start_type = typecheck_expr(start_node, current, symtab_err_flag, filename);
        }

        // Determine end expression type
        if (node->prodrule == PR_FOR_IDENT_IDENT || node->prodrule == PR_FOR_LITERAL_IDENT)
        {
            // end is an IDENT
            char *end_name = end_node->leaf->text;
            struct sym_entry *end_e = lookup(current, end_name);
            if (!end_e)
            {
                fprintf(stderr, "%s:%d: semantic error: undeclared variable %s in for‑loop range\n",
                        filename, end_node->leaf->lineno, end_name);
                *symtab_err_flag = 1;
            }
            else
            {
                end_type = end_e->type;
                if (end_e->is_nullable)
                {
                    fprintf(stderr, "%s:%d: semantic error: nullable variable %s not allowed in for‑loop range\n",
                            filename, end_node->leaf->lineno, end_name);
                    *symtab_err_flag = 1;
                }
            }
        }
        else
        {
            // end is a literal
            end_type = typecheck_expr(end_node, current, symtab_err_flag, filename);
        }

        // Verify both types exist and are numeric
        if (start_type && end_type)
        {
            if (!is_whole_number_type(start_type->basetype) || !is_whole_number_type(end_type->basetype))
            {
                fprintf(stderr, "%s:%d: semantic error: for-loop range must be whole number numeric types\n",
                        filename, node->kids[0]->leaf->lineno);
                *symtab_err_flag = 1;
            }
            else if (!type_equal(start_type, end_type))
            {
                fprintf(stderr, "%s:%d: semantic error: for-loop range bounds must have the same type\n",
                        filename, node->kids[0]->leaf->lineno);
                *symtab_err_flag = 1;
            }
            else
            {
                // All checks passed – declare the loop variable
                typeptr var_type = malloc(sizeof(*var_type));
                memset(var_type, 0, sizeof(*var_type));
                var_type->basetype = start_type->basetype; // infer from range type

                // Loop variable is implicitly val (immutable) and non‑nullable
                insert(current, var_name, var_type, 0, 0, 0);
            }
        }

        // The control_structure_body will be visited by the recursion at the end
        break;
    }
    // any fallthrough single line expressions like functions calls
    case PR_NON_CONTROL_STATEMENT:
        typecheck_expr(node->kids[0], current, symtab_err_flag, filename);
        break;
    }

    for (int i = 0; i < node->nkids; i++)
        build_symtab(node->kids[i], current, symtab_err_flag, filename);
}

// helper for printing a given symbol table scope st
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
                    case ARRAY_TYPE:
                        printf("  %s %s, type: Array%s\n", mut, e->name, nullable);
                        break;
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

// first function to call when printing the symbol table
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

// build a linked list of parameters to be placed in a function's parameter list
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
        typeptr t = type_from_ast_node(node->kids[2]);

        // Insert into symbol table
        insert(st, name, t, 0, 0, 0);

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

// recursive function that checks for nested expressions and ultimately returns the type of the expression
// primarily prints errors relating to operator misuse and type mismatches
typeptr typecheck_expr(struct tree *node, struct sym_table *current, int *err, char *filename)
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

    case K_NULL:
    {
        typeptr null_typeptr = NULL;
        if (!null_typeptr)
        {
            null_typeptr = malloc(sizeof(*null_typeptr));
            null_typeptr->basetype = NULL_TYPE;
        }
        return null_typeptr;
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

        if (!type_equal(left, right) || !is_numeric_type(left->basetype) || !is_numeric_type(right->basetype))
        {
            fprintf(stderr, "%s:%d: semantic error: type mismatch in binary expression\n",
                    filename, node->kids[1]->leaf->lineno);
            *err = 1;
            return NULL;
        }
        if (is_nullable_expr(node->kids[0], current) || is_nullable_expr(node->kids[2], current))
        {
            fprintf(stderr, "%s:%d: semantic error: nullable operands not allowed in arithmetic expression\n",
                    filename, node->kids[1]->leaf->lineno);
            *err = 1;
            return NULL;
        }

        return left;
    }
    // Relational expressions: expr < expr, expr > expr, etc.
    case PR_RELATIONAL_LT:
    case PR_RELATIONAL_GT:
    case PR_RELATIONAL_LTE:
    case PR_RELATIONAL_GTE:
    {
        typeptr left = typecheck_expr(node->kids[0], current, err, filename);
        typeptr right = typecheck_expr(node->kids[2], current, err, filename);
        if (!left || !right)
            return NULL;

        // In k0, relational operators require numeric types (Int, Double, etc.)
        int left_bt = left->basetype;
        int right_bt = right->basetype;
        if (!is_numeric_type(left_bt) || !is_numeric_type(right_bt) ||
            !type_equal(left, right))
        {
            fprintf(stderr, "%s:%d: relational operator requires numeric operands of the same type\n",
                    filename, node->kids[1]->leaf->lineno);
            *err = 1;
            return NULL;
        }
        if (is_nullable_expr(node->kids[0], current) || is_nullable_expr(node->kids[2], current))
        {
            fprintf(stderr, "%s:%d: semantic error: nullable operands not allowed in relational expression\n",
                    filename, node->kids[1]->leaf->lineno);
            *err = 1;
            return NULL;
        }
        // Result is always Boolean
        return get_bool_typeptr();
    }

    // Equality operators (==, !=) – allow any types and allow null
    case PR_EQUALITY_EQ:
    case PR_EQUALITY_NEQ:
    case PR_EQUALITY_RNEQ:
    case PR_EQUALITY_REQ:
    {
        typeptr left = typecheck_expr(node->kids[0], current, err, filename);
        typeptr right = typecheck_expr(node->kids[2], current, err, filename);
        if (!left || !right)
            return NULL;

        // Allow any operand types, including null; no numeric requirement.
        // However, if both are non‑null, they must have the same type (except null).
        if (left->basetype != NULL_TYPE && right->basetype != NULL_TYPE &&
            !type_equal(left, right))
        {
            fprintf(stderr, "%s:%d: equality operator requires operands of the same type\n",
                    filename, node->kids[1]->leaf->lineno);
            *err = 1;
            return NULL;
        }
        // Result is Boolean
        return get_bool_typeptr();
    }

    case PR_LOGICAL_OR_RECUR:
    case PR_LOGICAL_AND_RECUR:
    {
        typeptr left = typecheck_expr(node->kids[0], current, err, filename);
        typeptr right = typecheck_expr(node->kids[2], current, err, filename);
        if (!left || !right)
            return NULL;

        if (left->basetype != BOOLEAN_TYPE || right->basetype != BOOLEAN_TYPE)
        {
            fprintf(stderr, "%s:%d: logical operator requires Boolean operands\n",
                    filename, node->kids[1]->leaf->lineno);
            *err = 1;
            return NULL;
        }
        if (is_nullable_expr(node->kids[0], current) || is_nullable_expr(node->kids[2], current))
        {
            fprintf(stderr, "%s:%d: semantic error: nullable operands not allowed in logical expression\n",
                    filename, node->kids[1]->leaf->lineno);
            *err = 1;
            return NULL;
        }
        return get_bool_typeptr();
    }

    // unary
    case PR_UNARY_MINUS:
    {
        typeptr t = typecheck_expr(node->kids[1], current, err, filename);
        if (!t)
            return NULL;
        if (!is_numeric_type(t->basetype))
        {
            fprintf(stderr, "%s:%d: unary minus requires numeric operand\n",
                    filename, node->kids[0]->leaf->lineno);
            *err = 1;
            return NULL;
        }
        if (is_nullable_expr(node->kids[1], current))
        {
            fprintf(stderr, "%s:%d: semantic error: nullable operand not allowed in unary minus\n",
                    filename, node->kids[0]->leaf->lineno);
            *err = 1;
            return NULL;
        }
        return t;
    }

    case PR_UNARY_NOT:
    {
        typeptr t = typecheck_expr(node->kids[1], current, err, filename);

        if (!t)
            return NULL;

        if (t->basetype != BOOLEAN_TYPE)
        {
            fprintf(stderr, "%s:%d: ! requires boolean\n",
                    filename, node->kids[0]->leaf->lineno);
            *err = 1;
            return NULL;
        }

        if (is_nullable_expr(node->kids[1], current))
        {
            fprintf(stderr, "%s:%d: semantic error: nullable operand not allowed in unary not\n",
                    filename, node->kids[0]->leaf->lineno);
            *err = 1;
            return NULL;
        }
        return t;
    }

    case PR_POST_FIX_INDEX:
    {
        char *name = node->kids[0]->leaf->text;
        struct sym_entry *e = lookup(current, name);
        if (!e)
        {
            fprintf(stderr, "%s:%d: semantic error: array %s is undeclared\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *err = 1;
            return NULL;
        }
        if (node->kids[2]->leaf->type->basetype != INT_TYPE)
        {
            fprintf(stderr, "%s:%d: semantic error: subscript needs integer %s\n",
                    filename, node->kids[0]->leaf->lineno, name);
            *err = 1;
            return NULL;
        }
        return e->type->u.a.elemtype;
    }

    // function call
    case PR_FUNCTION_CALL:
    {
        return check_function_call(node, current, err, filename);
    }
    }
    return NULL;
}

// returns 1 if two types are equal and 0 if they are not. Used for stiffer type checking
int type_equal(typeptr a, typeptr b)
{
    if (!a || !b)
        return 0;
    if (a->basetype != b->basetype)
        return 0;

    if (a->basetype == ARRAY_TYPE)
    {
        return type_equal(a->u.a.elemtype, b->u.a.elemtype);
    }
    return 1;
}

// check a function call against its declaration, ensuring parameter amounts and types are correct
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
    if (strcmp(name, "arrayOf") == 0)
    {
        typeptr elem_type = NULL;
        struct tree *arglist = node->kids[2];
        int arg_index = 0;

        while (arglist)
        {
            struct tree *arg_node;
            if (arglist->prodrule == PR_CALL_VALUES_RECUR)
            {
                arg_node = arglist->kids[2];
                arglist = arglist->kids[0];
            }
            else
            {
                arg_node = arglist;
                arglist = NULL;
            }

            typeptr arg_t = typecheck_expr(arg_node, current, err, filename);
            if (!arg_t)
            {
                *err = 1;
                return NULL;
            }

            // Disallow null literals as array elements (unless you want Array<Null>)
            if (arg_t->basetype == K0_NULL_TYPE)
            {
                fprintf(stderr, "%s:%d: semantic error: arrayOf cannot contain null literal\n",
                        filename, node->kids[0]->leaf->lineno);
                *err = 1;
                return NULL;
            }

            if (elem_type == NULL)
            {
                elem_type = arg_t; // first argument sets expected element type
            }
            else if (!type_equal(elem_type, arg_t))
            {
                fprintf(stderr, "%s:%d: semantic error: all arguments to arrayOf must have the same type\n",
                        filename, node->kids[0]->leaf->lineno);
                *err = 1;
                return NULL;
            }
            arg_index++;
        }

        // Build the array type: Array<elem_type>
        typeptr array_type = malloc(sizeof(*array_type));
        memset(array_type, 0, sizeof(*array_type));
        array_type->basetype = ARRAY_TYPE;
        array_type->u.a.elemtype = elem_type;

        return array_type;
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
        if (arg_t && is_nullable_expr(arg_node, current))
        {
            fprintf(stderr, "%s:%d: semantic error: argument %d of %s is nullable but parameter expects non-nullable\n",
                    filename, node->kids[0]->leaf->lineno, arg_index, name);
            *err = 1;
        }

        param = param->next;
        arg_index++;
    }

    return ftype->u.f.returntype;
}

// checks if a base type is numeric
int is_numeric_type(int bt)
{
    return bt == INT_TYPE || bt == DOUBLE_TYPE || bt == FLOAT_TYPE || bt == LONG_TYPE || bt == SHORT_TYPE || bt == BYTE_TYPE;
}

// Returns 1 if the base type is a whole number numeric type
int is_whole_number_type(int bt)
{
    return bt == INT_TYPE || bt == LONG_TYPE || bt == SHORT_TYPE || bt == BYTE_TYPE;
}

// Check if a value of type src can be assigned to a variable of type dst
int is_assignable(typeptr dst, typeptr src, struct tree *src_node)
{
    if (!dst || !src)
        return 0;
    if (type_equal(dst, src))
        return 1;
    if (src->basetype == NULL_TYPE)
        return 0;

    if (is_numeric_type(dst->basetype) && is_numeric_type(src->basetype))
    {
        // Allow integer literal to any whole number destination
        if (src_node && src_node->prodrule == INT && is_whole_number_type(dst->basetype))
        {
            return 1;
        }
        // Floating destination can be any numeric source
        if (dst->basetype == FLOAT_TYPE || dst->basetype == DOUBLE_TYPE)
        {
            return 1;
        }
        // Whole number destination: whole number source with widening
        if (is_whole_number_type(dst->basetype) && is_whole_number_type(src->basetype))
        {
            return number_rank(src->basetype) <= number_rank(dst->basetype);
        }
        return 0;
    }

    return 0;
}

// helper that returns a rank of the given numeric basetype for use in determining if a type is assignable to another
int number_rank(int bt)
{
    switch (bt)
    {
    case BYTE_TYPE:
        return 1;
    case SHORT_TYPE:
        return 2;
    case INT_TYPE:
        return 3;
    case LONG_TYPE:
        return 4;
    default:
        return 0;
    }
}

// returns a bool typeptr
typeptr get_bool_typeptr(void)
{
    static typeptr bool_ptr = NULL;
    if (!bool_ptr)
    {
        bool_ptr = malloc(sizeof(*bool_ptr));
        bool_ptr->basetype = BOOLEAN_TYPE;
    }
    return bool_ptr;
}

// make a symbol non nullable
void make_non_nullable(struct sym_entry *e)
{
    e->is_nullable = 0;
    return;
}

// make a symbol nullable
void make_nullable(struct sym_entry *e)
{
    e->is_nullable = 1;
    return;
}

// Returns 1 if the expression is nullable, 0 otherwise.
int is_nullable_expr(struct tree *node, struct sym_table *st)
{
    if (!node)
        return 0;

    switch (node->prodrule)
    {
    // Literals that are never null
    case INT:
    case REAL:
    case STRING:
    case MULTI_STRING:
    case CHAR:
    case K_TRUE:
    case K_FALSE:
        return 0;

    // null literal
    case K_NULL:
        return 1;

    // Identifier: use stored nullability
    case IDENT:
    {
        struct sym_entry *e = lookup(st, node->leaf->text);
        return e ? e->is_nullable : 0;
    }

    // Parentheses: nullability of inner expression
    case PR_PRIMARY_PAREN:
        return is_nullable_expr(node->kids[1], st);

    // Unary operators: propagate nullability of operand
    case PR_UNARY_MINUS:
    case PR_UNARY_NOT:
        return is_nullable_expr(node->kids[1], st);

    // Binary operators: nullable if either operand is nullable
    case PR_ADDITIVE_PLUS:
    case PR_ADDITIVE_MINUS:
    case PR_MULT_MUL:
    case PR_MULT_DIV:
    case PR_MULT_MOD:
    case PR_RELATIONAL_LT:
    case PR_RELATIONAL_GT:
    case PR_RELATIONAL_LTE:
    case PR_RELATIONAL_GTE:
    case PR_EQUALITY_EQ:
    case PR_EQUALITY_NEQ:
    case PR_EQUALITY_RNEQ:
    case PR_EQUALITY_REQ:
    case PR_LOGICAL_AND_RECUR:
    case PR_LOGICAL_OR_RECUR:
        return is_nullable_expr(node->kids[0], st) ||
               is_nullable_expr(node->kids[2], st);

    // Function call: use the function symbol's nullability flag
    case PR_FUNCTION_CALL:
    {
        char *fname = node->kids[0]->leaf->text;
        struct sym_entry *e = lookup(st, fname);
        return e ? e->is_nullable : 0;
    }

    // !! (non‑null assertion) makes expression non‑nullable
    case PR_POST_FIX_NN_ASSERT:
        return 0;

    // ? (nullable operator) makes it nullable
    case PR_POST_FIX_NULLABLE:
        return 1;

    // For any other production, assume non‑nullable as a safe default
    default:
        return 0;
    }
}

// mainly for differentiating between array and normal types since array grammar rule requires one more depth of recursion
typeptr type_from_ast_node(struct tree *type_node)
{
    if (type_node->leaf)
    {
        typeptr t = malloc(sizeof(*t));
        memset(t, 0, sizeof(*t));
        t->basetype = type_node->leaf->category;
        return t;
    }
    if (type_node->prodrule == PR_TYPE_ARRAY)
    {
        typeptr t = malloc(sizeof(*t));
        memset(t, 0, sizeof(*t));
        t->basetype = ARRAY_TYPE;
        t->u.a.elemtype = type_from_ast_node(type_node->kids[2]);
        return t;
    }
    return NULL;
}

// Recursively frees a type structure and all its nested components.
void free_type(typeptr t)
{
    if (!t)
        return;

    switch (t->basetype)
    {
    case FUNC_TYPE:
        free(t->u.f.name);
        free_type(t->u.f.returntype);
        free_paramlist(t->u.f.parameters);
        // Do not free t->u.f.st here, the symbol table is freed separately
        break;
    case ARRAY_TYPE:
        free_type(t->u.a.elemtype);
        break;
    default:
        // Simple types have no nested allocations
        break;
    }
    free(t);
}

// Recursively frees a parameter list.
void free_paramlist(paramlist p)
{
    while (p)
    {
        paramlist next = p->next;
        free(p->name);
        free_type(p->type);
        free(p);
        p = next;
    }
}

// Frees a single symbol table entry and its associated type.
void free_sym_entry(struct sym_entry *e)
{
    if (!e)
        return;
    free(e->name);
    free_type(e->type);
    free(e);
}

// Recursively frees an entire symbol table hierarchy.
// Does NOT free the parent pointer (parent is freed by its owner).
// Frees all children and siblings, all entries, and the table itself.
void free_symtab(struct sym_table *st)
{
    if (!st)
        return;

    // Free all entries in this table
    for (int i = 0; i < st->nBuckets; i++)
    {
        struct sym_entry *e = st->tbl[i];
        while (e)
        {
            struct sym_entry *next = e->next;
            free_sym_entry(e);
            e = next;
        }
    }
    free(st->tbl);
    free(st->scope_name);

    // Recursively free child scopes
    struct sym_table *child = st->child;
    while (child)
    {
        struct sym_table *next = child->sibling;
        free_symtab(child);
        child = next;
    }

    // Finally free the table structure itself
    free(st);
}