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
%right ASSIGN PLUS_ASSIGN MINUS_ASSIGN
%left PLUS MINUS
%left MUL DIV MOD
%right NOT
%right INC DEC
%nonassoc IFX
%nonassoc ELSE
%union {
    struct tree *treeptr;
}
%{
    extern int yylex();
    extern int yyerror(char *s);
    #include <stdio.h>
    #include "tree.h"
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
top_level_decl /* for now, just a function declaration, but includes class declarations, import statements, etc. Fulfills topLevelObject */
    : function_decl
    | importHeader
    | global_var_decl | global_var_init // able to declare and initalize variables at a top (global) level
    ;
importHeader /* imports */
    : IMPORT IDENT
    ;
type /* basic types supported in k0 */
    : BYTE_TYPE 
    | SHORT_TYPE 
    | INT_TYPE 
    | LONG_TYPE 
    | FLOAT_TYPE 
    | DOUBLE_TYPE 
    | BOOLEAN_TYPE 
    | STRING_TYPE
    | ARRAY LT type GT
    ;
val_var /* keywords val or var to be used in variable declaration/initialization */
    : VAL
    | VAR
    ;
literal /* literals */
    : INT 
    | REAL 
    | STRING 
    | MULTI_STRING 
    | CHAR
    ;
bool_literal
    : K_TRUE
    | K_FALSE
    ;
/* Variable declaration/initialization
/* global variable declaration. Acts as typeConstraints. 
In k0, declaration syntax is only allowed for global variables and at the top of the bodies of function definitions */
global_var_decl 
    : val_var IDENT COLON type
    | val_var IDENT COLON type ASSIGN literal
    | val_var IDENT COLON type ASSIGN IDENT
    ;
/* variable initializations strictly at the top (global) level. 
k0 allows only simple initializers including int, float and char */
global_var_init
    : val_var IDENT ASSIGN INT
    | val_var IDENT ASSIGN REAL
    | val_var IDENT ASSIGN CHAR
    ;
/* function body variable declaration. Acts as typeConstraints. 
In k0, declaration syntax is only allowed for global variables and at the top of the bodies of function definitions*/
fun_body_var_decl 
    : val_var IDENT COLON type
    | val_var IDENT COLON type ASSIGN literal
    | val_var IDENT COLON type ASSIGN IDENT
    ;
/* variable initializations strictly at the start of function bodies. 
k0 allows only simple initializers including int, float and char */
fun_body_var_init 
    : val_var IDENT ASSIGN INT
    | val_var IDENT ASSIGN REAL
    | val_var IDENT ASSIGN CHAR
    ;
function_var_decl /* just var_decl without val_var, to be place within function declarations only */
    : IDENT COLON type
    ;

/* the basic function declaration of kotlin. Fulfills functionDeclaration. */
/* k0 does not support function bodies that are expressions not enclosed in curly brackets */
/* Kotlin does not do function declaration and definition separately like C. So we can choose to allow declarations like fun main() <no body>, but they seem useless */
function_decl 
    : FUN IDENT LPAREN parameter_list RPAREN COLON type function_body_with_return // fun main(args: Array<String>, arg_count: Int): Int {}
    | FUN IDENT LPAREN parameter_list RPAREN function_body // fun main() {} - the colon and return type may be omitted for functions with no return
    | FUN IDENT LPAREN parameter_list RPAREN // fun main() <no body> - a strict declaration that is useless other than the programmer wanting to strictly declare a function
    ;
parameter_list /* allows for multiple parameters in a function declaration */
    : function_var_decl
    | parameter_list COMMA function_var_decl
    |
    ;

/* k0 allows for var_defs and var_inits at the top of the bodies of function definitions, before the first executable statement */
function_body
    : LBRACE fun_body_var_list statement_list RBRACE
    ;
function_body_with_return
    : LBRACE fun_body_var_list statement_list RETURN expr RBRACE
    ;
fun_body_var_list /* the list of variable defintions/initializations that must go at the start of a function body */
    : fun_body_var_list fun_body_var_decl
    | fun_body_var_list fun_body_var_init
    |
    ;
statement_list /* a statement list is built of another statement list and statements, fulfills the role of statements in the Kotlin grammar */
    : statement_list statement
    |
    ;
statement /* a statement for now is just an expression followed by an optional semicolon, that is how kotlin works */
    : non_control_statement
    | loop_statement
    | if_statement
    ;
non_control_statement
    : expr opt_semicolon
    ;
opt_semicolon
    : SEMICOLON
    | 
    ;
expr /* for now, an expression is just an assignment with allowable arithmetic. NOTE this gets called before a primary expression in order to keep previous code working */
    : assignment_expr
    ;
assignment_expr /* assignment expression allows for one additive expression or assignment */
    : additive_expr
    | IDENT ASSIGN assignment_expr
    | IDENT PLUS_ASSIGN assignment_expr
    | IDENT MINUS_ASSIGN assignment_expr
    ;
additive_expr /* left recursion to create correct order of operations */
    : mulitplicative_expr
    | additive_expr PLUS mulitplicative_expr
    | additive_expr MINUS mulitplicative_expr
    ;
mulitplicative_expr /* left recursion into a unary expression */
    : unary_expr
    | mulitplicative_expr MUL unary_expr
    | mulitplicative_expr DIV unary_expr
    | mulitplicative_expr MOD unary_expr
    ;
unary_expr /* all unary expressions as well as a primary expression, which will allow for function calls as well as expressions again*/
    : primary_expr
    | MINUS unary_expr
    | NOT unary_expr
    | INC unary_expr
    | DEC unary_expr
    ;
primary_expr /* refactored expr into primary_expr, that includes everything from before as well as new logic to expressions */
    : function_call
    | IDENT
    | literal
    | LPAREN expr RPAREN
    | bool_literal
    ;
function_call /* a function call is a form of expression that calls functions */
    : IDENT LPAREN function_call_values_list RPAREN
    ;
function_call_values_list /* this list of arguments within that function call, acts as functionValueParameters */
    : functionCallVal
    | function_call_values_list COMMA functionCallVal
    |
    ;
functionCallVal
    : literal
    | IDENT
    ;
block /* cannot have variable declarations/initiations, since those are only allowed at the global level and top of functions*/
    : LBRACE statement_list RBRACE
    ;
loop_statement /* we are supporting for loops and while loops */
    : for_statement
    | while_statement
    ;
for_statement /* enforced form: for ( x in y..z) {} */
    : FOR LPAREN IDENT IN IDENT RANGE_INCL IDENT RPAREN control_structure_body // for ( x in y..z) {}
    | FOR LPAREN IDENT IN literal RANGE_INCL IDENT RPAREN control_structure_body // for ( x in 1..z) {}
    | FOR LPAREN IDENT IN IDENT RANGE_INCL literal RPAREN control_structure_body // for ( x in y..10) {}
    | FOR LPAREN IDENT IN literal RANGE_INCL literal RPAREN control_structure_body // for ( x in 1..10) {}
    ;
while_statement /* enforced form: while ( expr ) {} */
    : WHILE LPAREN expr RPAREN control_structure_body
    ;
control_structure_body /* the body following a control structure like a for, while, if statement can be a block or one statement */
    : block
    | statement
    ;
if_statement /* Allowing if, if else, and if else if*/
    : IF LPAREN expr RPAREN control_structure_body %prec IFX
    | IF LPAREN expr RPAREN control_structure_body ELSE control_structure_body
    ;
%%