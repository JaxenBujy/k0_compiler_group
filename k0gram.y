%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "tree.h"
    extern int yylex();
    extern int yyerror(char *s);
    struct tree *alctree(int prod_rule, char *symbol_name, int nkids, struct tree *kids[10], struct token *leaf);
    void print_node(struct tree *t);
    struct tree *root = NULL;
%}

%union {
    struct tree *treeptr;
}

/* Keywords */
%token <treeptr> BREAK CONTINUE DO ELSE K_FALSE FOR FUN IF IN K_NULL RETURN K_TRUE VAL VAR WHEN WHILE IMPORT

/* Error tokens */
%token <treeptr> OP_ERROR ILL_IDENT UNCLOSED_COMMENT COULD_NOT_MATCH KEYWORD_ERROR

/* Assignments */
%token <treeptr> ASSIGN PLUS_ASSIGN MINUS_ASSIGN

/* Arithmetic */
%token <treeptr> PLUS MINUS MUL DIV MOD

/* Inc/dec */
%token <treeptr> INC DEC

/* Unary */
%token <treeptr> NEG

/* Comparisons */
%token <treeptr> EQ NEQ GT LT GTE LTE
%token <treeptr> REQ RNEQ

/* Logic */
%token <treeptr> AND OR NOT

/* Assertions */
%token <treeptr> NN_ASSERT

/* Punctuation / access */
%token <treeptr> LSQUARE DOT
%token <treeptr> SAFE_CALL
%token <treeptr> ELVIS
%token <treeptr> NULLABLE
%token <treeptr> RANGE_INCL RANGE_EXCL
%token <treeptr> TYPE_CAST

/* Delimiters */
%token <treeptr> LPAREN RPAREN COMMA SEMICOLON LBRACE RBRACE COLON RSQUARE

/* Literals */
%token <treeptr> INT REAL STRING MULTI_STRING CHAR

/* Identifiers */
%token <treeptr> IDENT

/* Types */
%token <treeptr> BYTE_TYPE SHORT_TYPE INT_TYPE LONG_TYPE FLOAT_TYPE DOUBLE_TYPE BOOLEAN_TYPE STRING_TYPE
%token <treeptr> ARRAY

/* Production Rules */
%type <treeptr> program
%type <treeptr> top_level_list
%type <treeptr> top_level_decl
%type <treeptr> importHeader
%type <treeptr> type
%type <treeptr> val_var
%type <treeptr> literal
%type <treeptr> bool_literal
%type <treeptr> global_var_decl
%type <treeptr> global_var_init
%type <treeptr> fun_body_var_decl
%type <treeptr> fun_body_var_init
%type <treeptr> function_var_decl
%type <treeptr> function_decl
%type <treeptr> parameter_list
%type <treeptr> function_body
%type <treeptr> fun_body_var_list
%type <treeptr> statement_list
%type <treeptr> statement
%type <treeptr> non_control_statement
%type <treeptr> expr
%type <treeptr> logical_or_expr
%type <treeptr> logical_and_expr
%type <treeptr> equality_expr
%type <treeptr> relational_expr
%type <treeptr> assignment_expr
%type <treeptr> additive_expr
%type <treeptr> multiplicative_expr
%type <treeptr> unary_expr
%type <treeptr> primary_expr
%type <treeptr> function_call
%type <treeptr> function_call_values_list
%type <treeptr> functionCallVal
%type <treeptr> block
%type <treeptr> loop_statement
%type <treeptr> for_statement
%type <treeptr> while_statement
%type <treeptr> control_structure_body
%type <treeptr> if_statement

/* Precedence */
%right ASSIGN PLUS_ASSIGN MINUS_ASSIGN
%left PLUS MINUS
%left MUL DIV MOD
%right NOT
%right INC DEC
%nonassoc IFX
%nonassoc ELSE
%start program
%%
program /* a program is just a list of top level declarations */
    : top_level_list {struct tree *kids[10] = {$1}; $$ = alctree(0, "program", 1, kids, NULL); }
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
    : INT {print_node($1);};
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
    : val_var IDENT COLON type SEMICOLON
    | val_var IDENT COLON type ASSIGN literal SEMICOLON
    | val_var IDENT COLON type ASSIGN IDENT SEMICOLON
    ;
/* variable initializations strictly at the top (global) level. 
k0 allows only simple initializers including int, float and char */
global_var_init
    : val_var IDENT ASSIGN INT SEMICOLON
    | val_var IDENT ASSIGN REAL SEMICOLON
    | val_var IDENT ASSIGN CHAR SEMICOLON
    ;
/* function body variable declaration. Acts as typeConstraints. 
In k0, declaration syntax is only allowed for global variables and at the top of the bodies of function definitions*/
fun_body_var_decl 
    : val_var IDENT COLON type SEMICOLON
    | val_var IDENT COLON type ASSIGN literal SEMICOLON
    | val_var IDENT COLON type ASSIGN IDENT SEMICOLON
    ;
/* variable initializations strictly at the start of function bodies. 
k0 allows only simple initializers including int, float and char */
fun_body_var_init 
    : val_var IDENT ASSIGN INT SEMICOLON
    | val_var IDENT ASSIGN REAL SEMICOLON
    | val_var IDENT ASSIGN CHAR SEMICOLON
    ;
function_var_decl /* just var_decl without val_var, to be placed within function declarations only */
    : IDENT COLON type
    ;

/* the basic function declaration of kotlin. Fulfills functionDeclaration. */
/* k0 does not support function bodies that are expressions not enclosed in curly brackets */
/* Kotlin does not do function declaration and definition separately like C. So we can choose to allow declarations like fun main() <no body>, but they seem useless */
function_decl 
    : FUN IDENT LPAREN parameter_list RPAREN COLON type function_body // fun main(args: Array<String>, arg_count: Int): Int {}
    | FUN IDENT LPAREN parameter_list RPAREN function_body // fun main() {} - the colon and return type may be omitted for functions with no return
    | FUN IDENT LPAREN parameter_list RPAREN SEMICOLON // fun main() <no body> - a strict declaration that is useless other than the programmer wanting to strictly declare a function
    ;
parameter_list /* allows for multiple parameters in a function declaration */
    : function_var_decl
    | parameter_list COMMA function_var_decl
    | {$$ = NULL; };
    ;

/* k0 allows for var_defs and var_inits at the top of the bodies of function definitions, before the first executable statement */
function_body
    : LBRACE fun_body_var_list statement_list RBRACE
    ;
fun_body_var_list /* the list of variable defintions/initializations that must go at the start of a function body */
    : fun_body_var_list fun_body_var_decl
    | fun_body_var_list fun_body_var_init
    | {$$ = NULL; };
    ;
statement_list /* a statement list is built of another statement list and statements, fulfills the role of statements in the Kotlin grammar */
    : statement_list statement
    | {$$ = NULL; };
    ;
statement /* a statement for now is just an expression followed by an optional semicolon, that is how kotlin works */
    : non_control_statement
    | loop_statement
    | if_statement
    | RETURN expr SEMICOLON
    ;
non_control_statement
    : expr SEMICOLON
    ;
expr /* for now, an expression is just an assignment with allowable arithmetic and logical algebra. NOTE this gets called before a primary expression in order to keep previous code working */
    : assignment_expr
    ;
logical_or_expr
    : logical_and_expr
    | logical_or_expr OR logical_and_expr
    ;
logical_and_expr
    : equality_expr
    | logical_and_expr AND equality_expr
    ;
equality_expr
    : relational_expr
    | equality_expr EQ relational_expr
    | equality_expr NEQ relational_expr
    ;
relational_expr
    : additive_expr
    | relational_expr LT additive_expr
    | relational_expr GT additive_expr
    | relational_expr LTE additive_expr
    | relational_expr GTE additive_expr
    ;
assignment_expr /* assignment expression allows for one additive expression or assignment */
    : logical_or_expr
    | IDENT ASSIGN assignment_expr
    | IDENT PLUS_ASSIGN assignment_expr
    | IDENT MINUS_ASSIGN assignment_expr
    ;
additive_expr /* left recursion to create correct order of operations */
    : multiplicative_expr
    | additive_expr PLUS multiplicative_expr
    | additive_expr MINUS multiplicative_expr
    ;
multiplicative_expr /* left recursion into a unary expression */
    : unary_expr
    | multiplicative_expr MUL unary_expr
    | multiplicative_expr DIV unary_expr
    | multiplicative_expr MOD unary_expr
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
    | {$$ = NULL; };
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
struct tree *alctree(int prodrule, char *symbolname, int nkids, struct tree *kids[10], struct token *leaf) {
    struct tree *t = malloc(sizeof(struct tree));
    if (t == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree node\n");
        exit(1);
    }
    t->prodrule = prodrule;
    t->symbolname = symbolname;
    t->nkids = nkids;
    t->leaf = leaf;

    for (int i = 0; i < nkids; i++) {
        t->kids[i] = kids[i];
    }
    return t;
}
void print_node(struct tree *t) {
    printf("Prodrule: %d\nSymbol Name: %s\nNumber Kids: %d\n", t->prodrule, t->symbolname, t->nkids);
    if (t->leaf != NULL) {
        printf("Category: %d\nText: %s\nLine Number: %d\nFilename: %s\nival: %d\ndval: %f\nsval: %s\n\n",
        t->leaf->category, t->leaf->text, t->leaf->lineno, t->leaf->filename, t->leaf->ival, t->leaf->dval, t->leaf->sval);
    }
}