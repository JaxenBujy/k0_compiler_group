#include "symtab.h"
#include "k0gram.tab.h"
#include "tac.h"
#include <stdio.h>
#include <stdlib.h>

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
    if (root->has_first)
        printf("first=%d ", root->first.u.offset);
    if (root->has_follow)
        printf("follow=%d ", root->follow.u.offset);
    if (root->has_true)
        printf("true=%d ", root->onTrue.u.offset);
    if (root->has_false)
        printf("false=%d ", root->onFalse.u.offset);
    if (!root->has_first && !root->has_follow && !root->has_true && !root->has_false)
        printf("no label");
    // Remove trailing space if needed
    printf("]");

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

    switch (t->prodrule)
    {
    case PR_WHILE:
        return codegen_while(t, scope);
    case PR_IF_SIMPLE:
        return codegen_if(t, scope);
    case PR_IF_ELSE:
        return codegen_if_else(t);
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
    // default: recurse into children
    default:
        for (int i = 0; i < t->nkids; i++)
            code = append(code, codegen(t->kids[i], scope));
        return code;
    }
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
    // branch to onTrue if condition holds, else fall through to onFalse
    code = append(code,
                  gen(branch_op, t->onTrue, t->kids[0]->place, t->kids[2]->place));
    code = append(code,
                  gen(O_GOTO, t->onFalse, addr_none(), addr_none()));
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