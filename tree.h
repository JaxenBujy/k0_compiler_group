#include <stdio.h>
#include <stdlib.h>

struct tree
{
    int prodrule;
    char *symbolname;
    int nkids;
    struct tree *kids[10]; /* if nkids >0 */
    struct token *leaf;    /* if nkids == 0; NULL for ε productions */
};

struct token
{
    int category;   /* the integer code returned by yylex */
    char *text;     /* the actual string (lexeme) matched */
    int lineno;     /* the line number on which the token occurs */
    char *filename; /* the source file in which the token occurs */
    int ival;       /* for integer constants, store binary value here */
    double dval;    /* for real constants, store binary value here */
    char *sval;     /* for string constants, malloc space, de-escape, store the string (less quotes and after escapes) here */
};

void print_tree(struct tree *root);
void print_tree_with_depth(struct tree *root, int depth);
void free_tree(struct tree *root);
void free_tree_with_depth(struct tree *root, int depth);

enum ProductionRule {

    /* program */
    PR_PROGRAM = 100,

    /* top_level_list */
    PR_TOP_LEVEL_LIST_RECUR,
    PR_TOP_LEVEL_LIST_SINGLE,

    /* top_level_decl */
    PR_TOP_LEVEL_DECL_FUNCTION,
    PR_TOP_LEVEL_DECL_IMPORT,
    PR_TOP_LEVEL_DECL_GLOBAL_VAR_DECL,
    PR_TOP_LEVEL_DECL_GLOBAL_VAR_INIT,

    /* importHeader */
    PR_IMPORT_HEADER,

    /* type */
    PR_TYPE_BYTE,
    PR_TYPE_SHORT,
    PR_TYPE_INT,
    PR_TYPE_LONG,
    PR_TYPE_FLOAT,
    PR_TYPE_DOUBLE,
    PR_TYPE_BOOLEAN,
    PR_TYPE_STRING,
    PR_TYPE_ARRAY,

    /* val_var */
    PR_VAL,
    PR_VAR,

    /* literal */
    PR_LITERAL_INT,
    PR_LITERAL_REAL,
    PR_LITERAL_STRING,
    PR_LITERAL_MULTI_STRING,
    PR_LITERAL_CHAR,

    /* bool_literal */
    PR_BOOL_TRUE,
    PR_BOOL_FALSE,

    /* global_var_decl */
    PR_GLOBAL_VAR_DECL_SIMPLE,
    PR_GLOBAL_VAR_DECL_LITERAL_INIT,
    PR_GLOBAL_VAR_DECL_IDENT_INIT,

    /* global_var_init */
    PR_GLOBAL_VAR_INIT_INT,
    PR_GLOBAL_VAR_INIT_REAL,
    PR_GLOBAL_VAR_INIT_CHAR,

    /* fun_body_var_decl */
    PR_FUN_BODY_VAR_DECL_SIMPLE,
    PR_FUN_BODY_VAR_DECL_LITERAL_INIT,
    PR_FUN_BODY_VAR_DECL_IDENT_INIT,

    /* fun_body_var_init */
    PR_FUN_BODY_VAR_INIT_INT,
    PR_FUN_BODY_VAR_INIT_REAL,
    PR_FUN_BODY_VAR_INIT_CHAR,

    /* function_var_decl */
    PR_FUNCTION_VAR_DECL,

    /* function_decl */
    PR_FUNCTION_DECL_TYPED,
    PR_FUNCTION_DECL_UNTYPED,
    PR_FUNCTION_DECL_SEMICOLON,

    /* parameter_list */
    PR_PARAMETER_SINGLE,
    PR_PARAMETER_RECUR,
    PR_PARAMETER_EMPTY,

    /* function_body */
    PR_FUNCTION_BODY,

    /* fun_body_var_list */
    PR_FUN_BODY_VAR_LIST_DECL,
    PR_FUN_BODY_VAR_LIST_INIT,
    PR_FUN_BODY_VAR_LIST_EMPTY,

    /* statement_list */
    PR_STATEMENT_LIST_RECUR,
    PR_STATEMENT_LIST_EMPTY,

    /* statement */
    PR_STATEMENT_NON_CONTROL,
    PR_STATEMENT_LOOP,
    PR_STATEMENT_IF,
    PR_STATEMENT_RETURN,

    /* non_control_statement */
    PR_NON_CONTROL_STATEMENT,

    /* expr */
    PR_EXPR,

    /* logical_or_expr */
    PR_LOGICAL_OR_SINGLE,
    PR_LOGICAL_OR_RECUR,

    /* logical_and_expr */
    PR_LOGICAL_AND_SINGLE,
    PR_LOGICAL_AND_RECUR,

    /* equality_expr */
    PR_EQUALITY_SINGLE,
    PR_EQUALITY_EQ,
    PR_EQUALITY_NEQ,

    /* relational_expr */
    PR_RELATIONAL_SINGLE,
    PR_RELATIONAL_LT,
    PR_RELATIONAL_GT,
    PR_RELATIONAL_LTE,
    PR_RELATIONAL_GTE,

    /* assignment_expr */
    PR_ASSIGNMENT_SINGLE,
    PR_ASSIGNMENT_ASSIGN,
    PR_ASSIGNMENT_PLUS,
    PR_ASSIGNMENT_MINUS,

    /* additive_expr */
    PR_ADDITIVE_SINGLE,
    PR_ADDITIVE_PLUS,
    PR_ADDITIVE_MINUS,

    /* multiplicative_expr */
    PR_MULT_SINGLE,
    PR_MULT_MUL,
    PR_MULT_DIV,
    PR_MULT_MOD,

    /* unary_expr */
    PR_UNARY_PRIMARY,
    PR_UNARY_MINUS,
    PR_UNARY_NOT,
    PR_UNARY_INC,
    PR_UNARY_DEC,

    /* primary_expr */
    PR_PRIMARY_FUNCTION_CALL,
    PR_PRIMARY_IDENT,
    PR_PRIMARY_LITERAL,
    PR_PRIMARY_PAREN,
    PR_PRIMARY_BOOL,

    /* function_call */
    PR_FUNCTION_CALL,

    /* function_call_values_list */
    PR_CALL_VALUES_SINGLE,
    PR_CALL_VALUES_RECUR,
    PR_CALL_VALUES_EMPTY,

    /* functionCallVal */
    PR_CALL_VAL_LITERAL,
    PR_CALL_VAL_IDENT,

    /* block */
    PR_BLOCK,

    /* loop_statement */
    PR_LOOP_FOR,
    PR_LOOP_WHILE,

    /* for_statement */
    PR_FOR_IDENT_IDENT,
    PR_FOR_LITERAL_IDENT,
    PR_FOR_IDENT_LITERAL,
    PR_FOR_LITERAL_LITERAL,

    /* while_statement */
    PR_WHILE,

    /* control_structure_body */
    PR_CONTROL_BODY_BLOCK,
    PR_CONTROL_BODY_STATEMENT,

    /* if_statement */
    PR_IF_SIMPLE,
    PR_IF_ELSE
};
