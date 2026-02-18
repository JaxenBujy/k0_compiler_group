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
%{
    extern int yylex();
    extern int yyerror(char *s);
    #include <stdio.h>
%}
%start program
%%
program
    : top_level_list
    ;
top_level_list
    : top_level_list top_level_decl
    | top_level_decl
    ;
top_level_decl
    : function_decl
    ;
function_decl
    : FUN IDENT LPAREN RPAREN block
    ;
block
    : LBRACE stmt_list RBRACE
    ;
stmt_list
    : stmt_list stmt
    |
    ;
stmt
    : expr SEMICOLON
    | expr
    ;
expr
    : function_call
    { printf("This is an expression\n"); }
    ;
function_call
    : IDENT LPAREN arg_list RPAREN
    { printf("This is a function call\n"); }
arg_list
    : STRING
    |
    ;
    