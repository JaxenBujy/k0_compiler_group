#include "symtab.h"
#include "k0gram.tab.h"
#include "tac.h"
#include <stdio.h>
#include <stdlib.h>

int tempoffset;

void printsymbol(char *s)
{
    printf("Identifier: %s\n", s);
    fflush(stdout);
}

void print_tree(struct tree *root)
{
    print_tree_with_depth(root, 0);
}

void print_tree_with_depth(struct tree *root, int depth)
{
    if (!root)
        return;

    // Print indentation
    for (int i = 0; i < depth; i++)
        printf("  ");

    // Print node type and name
    if (root->leaf != NULL)
        printf("Leaf: %-15s\t", root->leaf->text);
    else
        printf("Node: %-20s\t", root->symbolname);

    // Print labels if they exist
    printf("[");
    if (root->has_first && root->has_follow)
        printf("first=%d follow=%d ", root->first.u.offset, root->follow.u.offset);
    if (root->has_first && !root->has_follow)
        printf("first=%d ", root->first.u.offset);
    if (root->has_true)
        printf("true=%d ", root->onTrue.u.offset);
    if (root->has_false)
        printf("false=%d ", root->onFalse.u.offset);
    if ((!root->has_first && !root->has_follow && !root->has_true && !root->has_false) || (root->has_follow && !root->has_first))
        printf("no label ");
    // Remove trailing space if needed
    printf("\b]");

    // Print production rule and serial
    printf(" (prod=%d, id=%d)", root->prodrule, root->id);

    // Print type info if available
    if (root->type)
        printf(" type=%d", root->type->basetype);

    printf("\n");

    // Print children
    for (int i = 0; i < root->nkids; i++)
        print_tree_with_depth(root->kids[i], depth + 1);
}

void free_tree(struct tree *root)
{
    free_tree_with_depth(root, 0);
}

void free_tree_with_depth(struct tree *root, int depth)
{
    if (!root)
        return;

    // Free children first (post-order)
    for (int i = 0; i < root->nkids; i++)
    {
        free_tree_with_depth(root->kids[i], depth + 1);
    }

    // Free leaf fields if present
    if (root->leaf != NULL)
    {
        free(root->leaf->text);
        free(root->leaf->filename);
        free(root->leaf->sval);
        free(root->leaf);
    }

    free(root->symbolname);
    free(root);
}

void assign_first(struct tree *t)
{
    if (t == NULL)
        return;

    // Post-order: process children first
    for (int i = 0; i < t->nkids; i++)
    {
        if (t->kids[i] != NULL)
            assign_first(t->kids[i]);
    }

    switch (t->prodrule)
    {
    // Statements that generate their own label
    case PR_IF_SIMPLE:
    case PR_IF_ELSE:
    case PR_WHILE:
    case PR_ASSIGNMENT_ASSIGN:
    case PR_ASSIGNMENT_PLUS:
    case PR_ASSIGNMENT_MINUS:
    case PR_FUNCTION_CALL:
    case PR_RELATIONAL_LT:
    case PR_RELATIONAL_GT:
    case PR_RELATIONAL_LTE:
    case PR_RELATIONAL_GTE:
    case PR_EQUALITY_EQ:
    case PR_EQUALITY_NEQ:
    case PR_LOGICAL_AND_RECUR:
    case PR_LOGICAL_OR_RECUR:
    {
        struct addr *l = genlabel();
        t->first = *l;
        t->has_first = 1;
        break;
    }

    // Block: first = first label of its statement_list
    case PR_BLOCK:
    {
        // kids[0] = "{", kids[1] = statement_list, kids[2] = "}"
        struct tree *stmt_list = t->kids[1];
        if (stmt_list != NULL && stmt_list->has_first)
        {
            t->first = stmt_list->first;
            t->has_first = 1;
        }
        break;
    }

    // Statement list: first = first label of first statement
    case PR_STATEMENT_LIST_RECUR:
    {
        // The first statement is the leftmost leaf in the chain
        // If prev_list exists and has a first, use that
        // Otherwise, use current statement's first
        struct tree *prev_list = t->kids[0];
        struct tree *curr_stmt = t->kids[1];

        if (prev_list != NULL && prev_list->has_first)
        {
            t->first = prev_list->first;
            t->has_first = 1;
        }
        else if (curr_stmt != NULL && curr_stmt->has_first)
        {
            t->first = curr_stmt->first;
            t->has_first = 1;
        }
        break;
    }

    // Non-control statement wrapper: first = first label of the expr inside
    case PR_NON_CONTROL_STATEMENT:
    {
        struct tree *expr = t->kids[0];
        if (expr != NULL && expr->has_first)
        {
            t->first = expr->first;
            t->has_first = 1;
        }
        break;
    }
    }
}

void assign_follow(struct tree *t)
{
    if (t == NULL)
        return;

    // Process current node first (top-down)
    switch (t->prodrule)
    {
    case PR_STATEMENT_LIST_RECUR:
    {
        struct tree *prev_list = t->kids[0]; // Previous statements (list)
        struct tree *curr_stmt = t->kids[1]; // Current statement

        if (curr_stmt != NULL)
        {
            // Current statement's follow = parent's follow
            curr_stmt->follow = t->follow;
            curr_stmt->has_follow = 1;
        }

        if (prev_list != NULL)
        {
            // Previous list's follow = current statement's first
            if (curr_stmt != NULL && curr_stmt->has_first)
            {
                prev_list->follow = curr_stmt->first;
                prev_list->has_follow = 1;
            }
            else
            {
                prev_list->follow = t->follow;
                prev_list->has_follow = 1;
            }
        }
        break;
    }

    case PR_BLOCK:
    {
        // Propagate follow to the statement_list inside
        struct tree *stmt_list = t->kids[1];
        if (stmt_list != NULL)
        {
            stmt_list->follow = t->follow;
            stmt_list->has_follow = 1;
        }
        break;
    }

    case PR_IF_SIMPLE:
    {
        // Body (kids[4]) gets the parent's follow
        struct tree *body = t->kids[4];
        if (body != NULL)
        {
            body->follow = t->follow;
            body->has_follow = 1;
        }

        // Set up and process the condition for onTrue and onFalse
        struct tree *condition = t->kids[2];
        if (condition != NULL && body != NULL)
        {
            condition->onTrue = body->first;
            condition->has_true = 1;
            condition->onFalse = t->follow;
            condition->has_false = 1;

            assign_on_true_false(condition);
        }
        break;
    }

    case PR_IF_ELSE:
    {
        // Both branches get parent's follow
        struct tree *then_branch = t->kids[4];
        struct tree *else_branch = t->kids[6];

        if (then_branch != NULL)
        {
            then_branch->follow = t->follow;
            then_branch->has_follow = 1;
        }
        if (else_branch != NULL)
        {
            else_branch->follow = t->follow;
            else_branch->has_follow = 1;
        }

        // Set up and process the condition for onTrue and onFalse
        struct tree *condition = t->kids[2];
        if (condition != NULL && then_branch != NULL && else_branch != NULL)
        {
            condition->onTrue = then_branch->first;
            condition->has_true = 1;
            condition->onFalse = else_branch->first;
            condition->has_false = 1;

            assign_on_true_false(condition);
        }
        break;
    }

    case PR_WHILE:
    {
        // Body jumps back to the loop's first label
        struct tree *body = t->kids[4];
        if (body != NULL)
        {
            body->follow = t->first; // Jump back to condition
            body->has_follow = 1;
        }

        // Set up and process the condition for onTrue and onFalse
        struct tree *condition = t->kids[2];
        if (condition != NULL && body != NULL)
        {
            condition->onTrue = body->first;
            condition->has_true = 1;
            condition->onFalse = t->follow;
            condition->has_false = 1;

            assign_on_true_false(condition);
        }
        break;
    }

    case PR_NON_CONTROL_STATEMENT:
    {
        // Propagate follow to the expression inside
        struct tree *expr = t->kids[0];
        if (expr != NULL)
        {
            expr->follow = t->follow;
            expr->has_follow = 1;
        }
        break;
    }
    default:
    {
        for (int i = 0; i < t->nkids; i++)
        {
            if (t->kids[i] != NULL)
            {
                t->kids[i]->follow = t->follow;
                t->kids[i]->has_follow = 1;
            }
        }
        break;
    }
    }

    // Recurse into children (continue top-down)
    for (int i = 0; i < t->nkids; i++)
    {
        if (t->kids[i] != NULL)
            assign_follow(t->kids[i]);
    }
}

void assign_on_true_false(struct tree *t)
{
    if (t == NULL)
        return;

    // Process current node first (top-down, inherited attributes)
    switch (t->prodrule)
    {
    // Relational expressions: x < y, x > y, x <= y, x >= y, x == y, x != y
    case PR_RELATIONAL_LT:
    case PR_RELATIONAL_GT:
    case PR_RELATIONAL_LTE:
    case PR_RELATIONAL_GTE:
    case PR_EQUALITY_EQ:
    case PR_EQUALITY_NEQ:
    {
        // These are leaf expressions in terms of boolean logic
        // They will generate a conditional jump during codegen
        // Nothing to propagate to children (identifiers are leaves)
        break;
    }

    // Logical AND: left && right
    case PR_LOGICAL_AND_RECUR:
    {
        struct tree *left = t->kids[0];
        struct tree *right = t->kids[2];

        if (left != NULL)
        {
            // If left is false, whole AND is false
            left->onFalse = t->onFalse;
            left->has_false = 1;

            // If left is true, need to check right
            left->onTrue = right->first; // Jump to right's code
            left->has_true = 1;
        }

        if (right != NULL)
        {
            // Right inherits parent's true/false
            right->onTrue = t->onTrue;
            right->has_true = 1;
            right->onFalse = t->onFalse;
            right->has_false = 1;
        }
        break;
    }

    // Logical OR: left || right
    case PR_LOGICAL_OR_RECUR:
    {
        struct tree *left = t->kids[0];
        struct tree *right = t->kids[2];

        if (left != NULL)
        {
            // If left is true, whole OR is true
            left->onTrue = t->onTrue;
            left->has_true = 1;

            // If left is false, need to check right
            left->onFalse = right->first; // Jump to right's code
            left->has_false = 1;
        }

        if (right != NULL)
        {
            // Right inherits parent's true/false
            right->onTrue = t->onTrue;
            right->has_true = 1;
            right->onFalse = t->onFalse;
            right->has_false = 1;
        }
        break;
    }

    // Logical NOT: !expr
    case PR_UNARY_NOT:
    {
        struct tree *expr = t->kids[1];
        if (expr != NULL)
        {
            // Swap true and false
            expr->onTrue = t->onFalse;
            expr->has_true = 1;
            expr->onFalse = t->onTrue;
            expr->has_false = 1;
        }
        break;
    }

    // For all other nodes, just propagate true/false to children
    default:
    {
        for (int i = 0; i < t->nkids; i++)
        {
            if (t->kids[i] != NULL)
            {
                t->kids[i]->onTrue = t->onTrue;
                t->kids[i]->has_true = 1;
                t->kids[i]->onFalse = t->onFalse;
                t->kids[i]->has_false = 1;
            }
        }
        break;
    }
    }

    // Recurse into children (top-down)
    for (int i = 0; i < t->nkids; i++)
    {
        if (t->kids[i] != NULL)
            assign_on_true_false(t->kids[i]);
    }
}

// parent recursive function that takes in a tree and generates three address code
struct instr *codegen(struct tree *t, struct sym_table *scope)
{
    if (t == NULL)
        return NULL;
    struct instr *code = NULL;

    if (t->nkids == 0)
    {
        if (t->leaf == NULL)
            return NULL; // epsilon production

        switch (t->leaf->category)
        {
        case INT:
            t->place = addr_const(t->leaf->ival);
            return NULL;

        case REAL:
        {
            // store as a local temp — doubles need their own slot
            struct addr tmp = new_temp();
            t->place = tmp;
            return NULL;
        }

        case STRING:
        case MULTI_STRING:
        {
            char *s = t->leaf->sval ? t->leaf->sval : "";

            if (!has_interpolation(s)) {
                int idx = addstring(s);
                t->place = addr_string(idx);
                return NULL;
            }

            // build printf format string
            char fmtbuf[1024];
            build_format_string(s, fmtbuf, sizeof(fmtbuf));
            int fmt_idx = addstring(fmtbuf);

            // extract variable names
            char *varnames[16];
            int nvars = extract_interp_vars(s, varnames, 16);

            // PARM the format string first
            struct instr *code = NULL;
            code = append(code, gen(O_PARM, addr_string(fmt_idx), addr_none(), addr_none()));

            // PARM each interpolated variable
            for (int i = 0; i < nvars; i++) {
                struct sym_entry *e = lookup(scope, varnames[i]);
                if (e) {
                    struct addr var_addr;
                    var_addr.region = e->region;
                    var_addr.u.offset = e->offset;
                    code = append(code, gen(O_PARM, var_addr, addr_none(), addr_none()));
                } else {
                    fprintf(stderr, "codegen: interpolated variable '%s' not found\n", varnames[i]);
                }
                free(varnames[i]);
            }

            // CALL printf directly
            struct addr retval = new_temp();
            code = append(code,
                gen(O_CALL, retval, addr_name("printf"), addr_const(nvars + 1)));
            t->place = retval;
            return code;
        }

        case CHAR:
        {
            // chars are just integer values
            t->place = addr_const((int)t->leaf->sval[0]);
            return NULL;
        }

        case K_TRUE:
            t->place = addr_const(1);
            return NULL;

        case K_FALSE:
            t->place = addr_const(0);
            return NULL;

        case K_NULL:
            t->place = addr_const(0);
            return NULL;

        case IDENT:
            t->place = lookup_place(t, scope);
            return NULL;

        default:
            return NULL;
        }
    }

    switch (t->prodrule)
    {
    case PR_WHILE:
        return codegen_while(t, scope);
    case PR_IF_SIMPLE:
        return codegen_if(t, scope);
    case PR_IF_ELSE:
        return codegen_if_else(t, scope);
    case PR_ASSIGNMENT_ASSIGN:
        return codegen_assign(t, scope);
    case PR_ADDITIVE_PLUS:
        return codegen_binop(t, O_ADD, scope);
    case PR_ADDITIVE_MINUS:
        return codegen_binop(t, O_SUB, scope);
    case PR_MULT_MUL:
        return codegen_binop(t, O_MUL, scope);
    case PR_MULT_DIV:
        return codegen_binop(t, O_DIV, scope);
    // boolean expressions
    case PR_RELATIONAL_LT:
        return codegen_relop(t, O_BLT, scope);
    case PR_RELATIONAL_GT:
        return codegen_relop(t, O_BGT, scope);
    case PR_RELATIONAL_LTE:
        return codegen_relop(t, O_BLE, scope);
    case PR_RELATIONAL_GTE:
        return codegen_relop(t, O_BGE, scope);
    case PR_EQUALITY_EQ:
        return codegen_relop(t, O_BEQ, scope);
    case PR_EQUALITY_NEQ:
        return codegen_relop(t, O_BNE, scope);
    // function calls
    case PR_FUNCTION_DECL_TYPED:
    case PR_FUNCTION_DECL_TYPED_NULLABLE:
    {
        char *fname = t->kids[1]->leaf->text;
        struct sym_table *inner = t->type->u.f.st;

        tempoffset = inner->next_offset;

        // Emit O_ASN parm:N → loc:offset for each incoming register argument.
        // This must be done before body codegen so the saves land first.
        struct instr *param_saves = NULL;
        {
            paramlist p = t->type->u.f.parameters;
            int idx = 0;
            while (p && idx < 6)
            {
                struct sym_entry *e = lookup_current(inner, p->name);
                if (e)
                {
                    struct addr dst = {e->region, {.offset = e->offset}};
                    struct addr src = {R_PARM, {.offset = idx}};
                    param_saves = append(param_saves,
                                         gen(O_ASN, dst, src, addr_none()));
                }
                p = p->next;
                idx++;
            }
        }

        // Generate body first so tempoffset reflects ALL allocations,
        // then emit D_PROC with the correct (final) frame size.
        struct instr *body = codegen(t->kids[7], inner);
        int frame = tempoffset;

        struct instr *code = gen(D_PROC, addr_name(fname),
                                 addr_const(0), addr_const(frame));
        code = append(code, param_saves);
        code = append(code, body);
        code = append(code, gen(D_END, addr_name(fname), addr_none(), addr_none()));
        return code;
    }
    case PR_FUNCTION_DECL_UNTYPED:
    {
        char *fname = t->kids[1]->leaf->text;
        struct sym_table *inner = t->type->u.f.st;

        tempoffset = inner->next_offset;

        // Emit parameter-save instructions
        struct instr *param_saves = NULL;
        {
            paramlist p = t->type->u.f.parameters;
            int idx = 0;
            while (p && idx < 6)
            {
                struct sym_entry *e = lookup_current(inner, p->name);
                if (e)
                {
                    struct addr dst = {e->region, {.offset = e->offset}};
                    struct addr src = {R_PARM, {.offset = idx}};
                    param_saves = append(param_saves,
                                         gen(O_ASN, dst, src, addr_none()));
                }
                p = p->next;
                idx++;
            }
        }

        // generate body first so tempoffset reflects all allocations
        struct instr *body = codegen(t->kids[5], inner);
        int frame = tempoffset;

        // now emit proc with the final tempoffset as frame size
        struct instr *code = gen(D_PROC, addr_name(fname),
                                 addr_const(0), addr_const(frame));
        code = append(code, param_saves);
        code = append(code, body);
        code = append(code, gen(D_END, addr_name(fname), addr_none(), addr_none()));
        return code;
    }
    case PR_STATEMENT_RETURN:
    {
        // kids[1] is the return expression
        struct instr *code = codegen(t->kids[1], scope);
        code = append(code, gen(O_RET, t->kids[1]->place, addr_none(), addr_none()));
        return code;
    }

    case PR_FUNCTION_CALL:
    {
        char *fname = t->kids[0]->leaf->text;
        int nargs = 0;
        struct instr *code = codegen_args(t->kids[2], scope, &nargs);

        // if the argument was an interpolated string it already emitted
        // its own PARM/CALL sequence to printf — suppress the outer call
        int already_called = 0;
        for (struct instr *p = code; p; p = p->next)
            if (p->opcode == O_CALL) { already_called = 1; break; }

        if (!already_called) {
            struct addr retval = new_temp();
            code = append(code,
                gen(O_CALL, retval, addr_name(fname), addr_const(nargs)));
            t->place = retval;
        }
        return code;
    }
    case PR_FUN_BODY_VAR_INIT:
    {
        /* val_var IDENT ASSIGN expr SEMICOLON
         * kids: [0]=val_var [1]=IDENT [2]=ASSIGN [3]=expr [4]=SEMICOLON */
        struct instr *code = codegen(t->kids[3], scope);
        struct addr dst = lookup_place(t->kids[1], scope);
        code = append(code, gen(O_ASN, dst, t->kids[3]->place, addr_none()));
        return code;
    }
    case PR_FUN_BODY_VAR_DECL_ASSIGN:
    {
        /* val_var IDENT COLON type ASSIGN expr SEMICOLON
         * kids: [0]=val_var [1]=IDENT [2]=COLON [3]=type [4]=ASSIGN [5]=expr [6]=SEMICOLON */
        struct instr *code = codegen(t->kids[5], scope);
        struct addr dst = lookup_place(t->kids[1], scope);
        code = append(code, gen(O_ASN, dst, t->kids[5]->place, addr_none()));
        return code;
    }
    case PR_FUN_BODY_VAR_DECL_ASSIGN_NULLABLE:
    {
        /* val_var IDENT COLON type NULLABLE ASSIGN expr SEMICOLON
         * kids: [0]=val_var [1]=IDENT [2]=COLON [3]=type [4]=NULLABLE [5]=ASSIGN [6]=expr [7]=SEMICOLON */
        struct instr *code = codegen(t->kids[6], scope);
        struct addr dst = lookup_place(t->kids[1], scope);
        code = append(code, gen(O_ASN, dst, t->kids[6]->place, addr_none()));
        return code;
    }
    case PR_GLOBAL_VAR_INIT:
    case PR_GLOBAL_VAR_DECL:
    case PR_GLOBAL_VAR_DECL_ASSIGN:
    case PR_GLOBAL_VAR_DECL_NULLABLE:
    case PR_GLOBAL_VAR_DECL_ASSIGN_NULLABLE:
        return NULL;
    // default: recurse into children
    default:
        for (int i = 0; i < t->nkids; i++)
            code = append(code, codegen(t->kids[i], scope));
        return code;
    }
}

struct instr *codegen_globals(struct tree *t, struct sym_table *scope)
{
    if (t == NULL)
        return NULL;
    struct instr *code = NULL;

    switch (t->prodrule)
    {
    case PR_GLOBAL_VAR_INIT:
    {
        // VAR IDENT ASSIGN expr SEMICOLON
        // kids[0]=VAR, kids[1]=IDENT, kids[2]=ASSIGN, kids[3]=expr
        struct instr *code = codegen(t->kids[3], scope);
        struct addr dst = lookup_place(t->kids[1], scope);
        code = append(code, gen(O_ASN, dst, t->kids[3]->place, addr_none()));
        return code;
    }
    case PR_GLOBAL_VAR_DECL_ASSIGN:
    {
        // VAR IDENT COLON TYPE ASSIGN expr SEMICOLON
        // kids[0]=VAR, kids[1]=IDENT, kids[2]=COLON, kids[3]=TYPE, kids[4]=ASSIGN, kids[5]=expr
        struct instr *code = codegen(t->kids[5], scope);
        struct addr dst = lookup_place(t->kids[1], scope);
        code = append(code, gen(O_ASN, dst, t->kids[5]->place, addr_none()));
        return code;
    }
    case PR_GLOBAL_VAR_DECL_ASSIGN_NULLABLE:
    {
        // kids[6] is the expr
        struct instr *code = codegen(t->kids[6], scope);
        struct addr dst = lookup_place(t->kids[1], scope);
        code = append(code, gen(O_ASN, dst, t->kids[6]->place, addr_none()));
        return code;
    }
    // skip function declarations - don't recurse into bodies
    case PR_FUNCTION_DECL_TYPED:
    case PR_FUNCTION_DECL_TYPED_NULLABLE:
    case PR_FUNCTION_DECL_UNTYPED:
        return NULL;
    // for everything else recurse
    default:
        for (int i = 0; i < t->nkids; i++)
            code = append(code, codegen_globals(t->kids[i], scope));
        return code;
    }
}

struct instr *codegen_args(struct tree *t, struct sym_table *scope, int *nargs)
{
    if (t == NULL)
        return NULL;
    struct instr *code = NULL;

    if (t->prodrule == PR_CALL_VALUES_RECUR)
    {
        code = codegen_args(t->kids[0], scope, nargs);
        code = append(code, codegen(t->kids[2], scope));

        // check if arg already emitted its own call
        int already_called = 0;
        for (struct instr *p = code; p; p = p->next)
            if (p->opcode == O_CALL) { already_called = 1; break; }

        if (!already_called) {
            code = append(code, gen(O_PARM, t->kids[2]->place, addr_none(), addr_none()));
            (*nargs)++;
        }
    }
    else
    {
        code = codegen(t, scope);

        // check if arg already emitted its own call
        int already_called = 0;
        for (struct instr *p = code; p; p = p->next)
            if (p->opcode == O_CALL) { already_called = 1; break; }

        if (!already_called) {
            code = append(code, gen(O_PARM, t->place, addr_none(), addr_none()));
            (*nargs)++;
        }
    }
    return code;
}

struct instr *codegen_assign(struct tree *t, struct sym_table *scope)
{
    // kids[0] = lhs identifier, kids[2] = rhs expression
    struct instr *code = codegen(t->kids[2], scope);   // eval RHS
    struct addr dst = lookup_place(t->kids[0], scope); // lhs address from symtab
    code = append(code,
                  gen(O_ASN, dst, t->kids[2]->place, addr_none()));
    t->place = dst;
    return code;
}

struct instr *codegen_binop(struct tree *t, int opcode, struct sym_table *scope)
{
    struct instr *code = codegen(t->kids[0], scope); // eval left
    code = append(code, codegen(t->kids[2], scope)); // eval right
    struct addr tmp = new_temp();                    // allocate a temp
    code = append(code,
                  gen(opcode, tmp, t->kids[0]->place, t->kids[2]->place));
    t->place = tmp;
    return code;
}

struct instr *codegen_relop(struct tree *t, int branch_op, struct sym_table *scope)
{
    struct instr *code = codegen(t->kids[0], scope);
    code = append(code, codegen(t->kids[2], scope));

    /*
     * Branch context (inside if/while condition):
     *   emit a conditional branch to onTrue, then an unconditional goto onFalse.
     *
     * Value context (result used as a Boolean variable, e.g. ok = x > 0):
     *   emit O_SCONT so x86gen can materialise 0/1 via SETcc.
     *   Encoding:
     *     dest   = output temporary
     *     src1   = left  operand address
     *     src2   = right operand address
     *   We stuff the branch_op discriminant into a separate field by
     *   temporarily storing it in dest.region while keeping dest.u.offset
     *   as the scratch slot — x86gen reads dest.region == R_CONST as the
     *   "branch_op tag" field.  Actually, cleanest: use a dedicated
     *   3-field instruction:
     *     O_SCONT  tmp , left_operand , right_operand
     *   and carry branch_op in a second O_SCONT instruction as:
     *     O_SCONT  addr_const(branch_op) , addr_none() , addr_none()
     *   which x86gen reads immediately before the main one.
     *
     * Simplest and self-contained: emit one instruction with
     *     dest   = new temp (where 0/1 goes)
     *     src1   = left operand
     *     src2   = right operand
     * and the branch_op baked into the *opcode* field (we map each relop
     * to a unique pseudo-opcode offset so x86gen can recover the SETcc).
     * We re-use the O_BLT..O_BNE range itself: x86gen checks that the
     * opcode is in [O_BLT, O_BNE] AND dest.region != R_LABEL.
     */
    if (t->has_true && t->has_false)
    {
        /* Branch context */
        code = append(code,
                      gen(branch_op, t->onTrue,
                          t->kids[0]->place, t->kids[2]->place));
        code = append(code,
                      gen(O_GOTO, t->onFalse, addr_none(), addr_none()));
    }
    else
    {
        /*
         * Value context.
         * Emit the same branch_op opcode, but with a LOCAL temp as dest
         * instead of a label.  x86gen distinguishes the two cases by
         * checking dest.region:
         *   R_LABEL  → branch (original behaviour)
         *   anything else → SETcc materialise into dest
         */
        struct addr tmp = new_temp();
        t->place = tmp;
        code = append(code,
                      gen(branch_op, tmp,
                          t->kids[0]->place, t->kids[2]->place));
    }
    return code;
}

struct instr *codegen_while(struct tree *t, struct sym_table *scope)
{
    struct tree *cond = t->kids[2];
    struct tree *body = t->kids[4];

    // label marking the top of the loop (t->first)
    struct instr *code = gen(D_LABEL, t->first, addr_none(), addr_none());

    // condition code (uses cond->onTrue / cond->onFalse already set)
    code = append(code, codegen(cond, scope));

    // body (label at body->first, GOTO back to loop top at the end)
    code = append(code, gen(D_LABEL, body->first, addr_none(), addr_none()));
    code = append(code, codegen(body, scope));
    code = append(code, gen(O_GOTO, t->first, addr_none(), addr_none()));

    // exit label (t->follow)
    code = append(code, gen(D_LABEL, t->follow, addr_none(), addr_none()));
    return code;
}

struct instr *codegen_if(struct tree *t, struct sym_table *scope)
{
    struct tree *cond = t->kids[2];
    struct tree *body = t->kids[4];

    struct instr *code = gen(D_LABEL, t->first, addr_none(), addr_none());
    code = append(code, codegen(cond, scope));
    code = append(code, gen(D_LABEL, body->first, addr_none(), addr_none()));
    code = append(code, codegen(body, scope));
    code = append(code, gen(D_LABEL, t->follow, addr_none(), addr_none()));
    return code;
}

struct instr *codegen_if_else(struct tree *t, struct sym_table *scope)
{
    struct tree *cond = t->kids[2];
    struct tree *then_part = t->kids[4];
    struct tree *else_part = t->kids[6];

    struct instr *code = NULL;

    // entry label
    code = gen(D_LABEL, t->first, addr_none(), addr_none());

    // generate condition (uses cond->onTrue / cond->onFalse)
    code = append(code, codegen(cond, scope));

    // THEN branch
    code = append(code, gen(D_LABEL, then_part->first, addr_none(), addr_none()));
    code = append(code, codegen(then_part, scope));

    // jump to follow after THEN (skip ELSE)
    code = append(code, gen(O_GOTO, t->follow, addr_none(), addr_none()));

    // ELSE branch
    code = append(code, gen(D_LABEL, else_part->first, addr_none(), addr_none()));
    code = append(code, codegen(else_part, scope));

    // exit label
    code = append(code, gen(D_LABEL, t->follow, addr_none(), addr_none()));

    return code;
}