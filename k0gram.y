%token BREAK CONTINUE DO ELSE K_FALSE FOR FUN IF IN K_NULL RETURN K_TRUE VAL VAR WHEN WHILE IMPORT
%token OP_ERROR ILL_IDENT UNCLOSED_COMMENT COULD_NOT_MATCH KEYWORD_ERROR
%token ASSIGN PLUS_ASSIGN MINUS_ASSIGN
%token PLUS MINUS MUL DIV MOD
%token INC DEC
%token NEG
%token EQ NEQ GT LT GTE LTE
%token REQ RNEQ
%token AND OR NOT
%token NN_ASSERT
%token LSQUARE DOT
%token SAFE_CALL
%token ELVIS
%token NULLABLE
%token RANGE_INCL RANGE_EXCL
%token TYPE_CAST
%token LPAREN RPAREN COMMA SEMICOLON LBRACE RBRACE COLON RSQUARE
%token INT REAL STRING MULTI_STRING CHAR
%token IDENT
%token BYTE_TYPE SHORT_TYPE INT_TYPE LONG_TYPE FLOAT_TYPE DOUBLE_TYPE BOOLEAN_TYPE STRING_TYPE
%token ARRAY
%{
    extern int yylex();
    extern int yyerror(char *s);
    #include <stdio.h>
%}
%start program
%%
program /* a program is just a list of top level declarations */
    : top_level_list
    ;
top_level_list /* a list of top level declarations */
    : top_level_list top_level_decl
    | top_level_decl
    ;
top_level_decl /* for now, just a function declaration, but includes class declarations, import statements, etc */
    : function_decl
    | importHeader
    ;
importHeader: /* imports */
    IMPORT IDENT
    ;
type: /* basic types supported in k0 */
    BYTE_TYPE | SHORT_TYPE | INT_TYPE | LONG_TYPE | FLOAT_TYPE | DOUBLE_TYPE | BOOLEAN_TYPE | STRING_TYPE | "String" {return STRING_TYPE; }
    ;
array_decl:
    ARRAY '<' type '>'
    ;
var_decl:
    IDENT COLON array_decl
    | IDENT COLON type
    ;
function_decl /* the basic function declaration of kotlin */
    : FUN IDENT LPAREN RPAREN block // fun main() {}
    | FUN IDENT LPAREN RPAREN COLON type block // fun main(): int {}
    | FUN IDENT LPAREN RPAREN // fun main() - has no block after it, just a function declaration
    | FUN IDENT LPAREN var_decl RPAREN COLON type block // fun main(args: Array<String>) {}
    ;
block /* a block is full of statements in a statement list */
    : LBRACE stmt_list RBRACE
    ;
stmt_list /* a statement list is built of another statement list and statements, or just statements */
    : stmt_list stmt
    |
    ;
stmt /* a statement for now is just an expression followed by an optional semicolon, that is how kotlin works */
    : expr SEMICOLON
    | expr
    ;
expr /* for now, an expression is just a function call, but it includes literals, assignments, etc. */
    : function_call
    { printf("This is an expression\n"); }
    ;
function_call /* a function call is a form of expression that calls functions */
    : IDENT LPAREN arg_list RPAREN
    { printf("This is a function call\n"); }
arg_list /* this list of arguments within that function call, just string for now */
    : STRING
    ;