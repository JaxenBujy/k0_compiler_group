%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "symtab.h"
    extern int yylex();
    extern int yyerror(const char *s);
    struct tree *alctree(int prod_rule, char *symbol_name, int nkids, struct tree *kids[10], struct token *leaf);
    void print_node(struct tree *t);
    struct tree *root = NULL;
    int serial = 0;
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
%token <treeptr> BYTE_TYPE SHORT_TYPE INT_TYPE LONG_TYPE FLOAT_TYPE DOUBLE_TYPE BOOLEAN_TYPE STRING_TYPE NULL_TYPE
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

/* for more descriptive error messages */
%define parse.lac full
%define parse.error detailed

%%
program /* a program is just a list of top level declarations */
    : top_level_list {root = $$; }
    ;
top_level_list /* a list of top level declarations */
    : top_level_list top_level_decl {struct tree *kids[10] = {$1,$2}; $$ = alctree(PR_TOP_LEVEL_LIST_RECUR, "top_level_list", 2, kids, NULL); }
    | top_level_decl {struct tree *kids[10] = {$1}; $$ = alctree(PR_TOP_LEVEL_LIST_SINGLE, "top_level_list", 1, kids, NULL); }
    ;
top_level_decl /* for now, just a function declaration, but includes class declarations, import statements, etc. Fulfills topLevelObject */
    : function_decl {struct tree *kids[10] = {$1}; $$ = alctree(PR_TOP_LEVEL_DECL_FUNCTION, "top_level_decl", 1, kids, NULL); }
    | importHeader {struct tree *kids[10] = {$1}; $$ = alctree(PR_TOP_LEVEL_DECL_IMPORT, "top_level_decl", 1, kids, NULL); }
    | global_var_decl {struct tree *kids[10] = {$1}; $$ = alctree(PR_TOP_LEVEL_DECL_GLOBAL_VAR_DECL, "top_level_decl", 1, kids, NULL); }
    | global_var_init {struct tree *kids[10] = {$1}; $$ = alctree(PR_TOP_LEVEL_DECL_GLOBAL_VAR_INIT, "top_level_decl", 1, kids, NULL); }
    ;
importHeader /* imports */
    : IMPORT IDENT {struct tree *kids[10] = {$1, $2}; $$ = alctree(PR_IMPORT_HEADER, "importHeader", 2, kids, NULL); }
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
    | NULL_TYPE
    | ARRAY LT type GT {struct tree *kids[10] = {$1, $2, $3, $4}; $$ = alctree(PR_TYPE_ARRAY, "type", 4, kids, NULL); }
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
    : val_var IDENT COLON type SEMICOLON {struct tree *kids[10] = {$1,$2,$3,$4,$5}; $$ = alctree(PR_GLOBAL_VAR_DECL, "global_var_decl", 5, kids, NULL); }
    | val_var IDENT COLON type ASSIGN expr SEMICOLON {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6,$7}; $$ = alctree(PR_GLOBAL_VAR_DECL_ASSIGN, "global_var_decl", 7, kids, NULL); }
    | val_var IDENT COLON type NULLABLE SEMICOLON {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6}; $$ = alctree(PR_GLOBAL_VAR_DECL_NULLABLE, "global_var_decl", 6, kids, NULL); }
    | val_var IDENT COLON type NULLABLE ASSIGN expr SEMICOLON {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6,$7,$8}; $$ = alctree(PR_GLOBAL_VAR_DECL_ASSIGN_NULLABLE, "global_var_decl", 8, kids, NULL); }
    ;
/* variable initializations strictly at the top (global) level. 
k0 allows only simple initializers including int, float and char */
global_var_init
    : val_var IDENT ASSIGN expr SEMICOLON {struct tree *kids[10] = {$1,$2,$3,$4,$5}; $$ = alctree(PR_GLOBAL_VAR_INIT, "global_var_init", 5, kids, NULL); }
    ;
/* function body variable declaration. Acts as typeConstraints. 
In k0, declaration syntax is only allowed for global variables and at the top of the bodies of function definitions*/
fun_body_var_decl 
    : val_var IDENT COLON type SEMICOLON {struct tree *kids[10] = {$1,$2,$3,$4,$5}; $$ = alctree(PR_FUN_BODY_VAR_DECL, "fun_body_var_decl", 5, kids, NULL); }
    | val_var IDENT COLON type ASSIGN expr SEMICOLON {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6,$7}; $$ = alctree(PR_FUN_BODY_VAR_DECL_ASSIGN, "fun_body_var_decl", 7, kids, NULL); }
    | val_var IDENT COLON type NULLABLE SEMICOLON {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6}; $$ = alctree(PR_FUN_BODY_VAR_DECL_NULLABLE, "fun_body_var_decl", 6, kids, NULL); }
    | val_var IDENT COLON type NULLABLE ASSIGN expr SEMICOLON {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6,$7,$8}; $$ = alctree(PR_FUN_BODY_VAR_DECL_ASSIGN_NULLABLE, "fun_body_var_decl", 8, kids, NULL); }
    ;
/* variable initializations strictly at the start of function bodies. 
k0 allows only simple initializers including int, float and char */
fun_body_var_init 
    : val_var IDENT ASSIGN expr SEMICOLON {struct tree *kids[10] = {$1,$2,$3,$4,$5}; $$ = alctree(PR_FUN_BODY_VAR_INIT, "fun_body_var_init", 5, kids, NULL); }
    ;
function_var_decl /* just var_decl without val_var, to be placed within function declarations only */
    : IDENT COLON type {struct tree *kids[10] = {$1,$2,$3}; $$ = alctree(PR_FUNCTION_VAR_DECL, "function_var_decl", 3, kids, NULL); }
    | IDENT COLON type NULLABLE {struct tree *kids[10] = {$1,$2,$3,$4}; $$ = alctree(PR_FUNCTION_VAR_DECL_NULLABLE, "function_var_decl", 4, kids, NULL); }
    ;

/* the basic function declaration of kotlin. Fulfills functionDeclaration. */
/* k0 does not support function bodies that are expressions not enclosed in curly brackets */
/* Kotlin does not do function declaration and definition separately like C. So we can choose to allow declarations like fun main() <no body>, but they seem useless */
function_decl 
    : FUN IDENT LPAREN parameter_list RPAREN COLON type function_body {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6,$7,$8}; $$ = alctree(PR_FUNCTION_DECL_TYPED, "function_decl", 8, kids, NULL); } // fun main(args: Array<String>, arg_count: Int): Int {}
    | FUN IDENT LPAREN parameter_list RPAREN COLON type NULLABLE function_body {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6,$7,$8,$9}; $$ = alctree(PR_FUNCTION_DECL_TYPED_NULLABLE, "function_decl", 9, kids, NULL); } // fun main(args: Array<String>, arg_count: Int): Int? {}
    | FUN IDENT LPAREN parameter_list RPAREN function_body {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6}; $$ = alctree(PR_FUNCTION_DECL_UNTYPED, "function_decl", 6, kids, NULL); } // fun main() {} - the colon and return type may be omitted for functions with no return
    ;
parameter_list /* allows for multiple parameters in a function declaration */
    : function_var_decl
    | parameter_list COMMA function_var_decl {struct tree *kids[10] = {$1,$2,$3}; $$ = alctree(PR_PARAMETER_RECUR, "parameter_list", 3, kids, NULL); }
    | {$$ = NULL; };
    ;

/* k0 allows for var_defs and var_inits at the top of the bodies of function definitions, before the first executable statement */
function_body
    : LBRACE fun_body_var_list statement_list RBRACE {struct tree *kids[10] = {$1,$2,$3,$4}; $$ = alctree(PR_FUNCTION_BODY, "function_body", 4, kids, NULL); }
    ;
fun_body_var_list /* the list of variable defintions/initializations that must go at the start of a function body */
    : fun_body_var_list fun_body_var_decl {struct tree *kids[10] = {$1,$2}; $$ = alctree(PR_FUN_BODY_VAR_LIST_DECL, "fun_body_var_list", 2, kids, NULL); }
    | fun_body_var_list fun_body_var_init {struct tree *kids[10] = {$1,$2}; $$ = alctree(PR_FUN_BODY_VAR_LIST_INIT, "fun_body_var_list", 2, kids, NULL); }
    | {$$ = NULL; };
    ;
statement_list /* a statement list is built of another statement list and statements, fulfills the role of statements in the Kotlin grammar */
    : statement_list statement {struct tree *kids[10] = {$1,$2}; $$ = alctree(PR_STATEMENT_LIST_RECUR, "statement_list", 2, kids, NULL); }
    | {$$ = NULL; };
    ;
statement /* a statement for now is just an expression followed by an optional semicolon, that is how kotlin works */
    : non_control_statement
    | loop_statement
    | if_statement
    | RETURN expr SEMICOLON {struct tree *kids[10] = {$1,$2,$3}; $$ = alctree(PR_STATEMENT_RETURN, "statement", 3, kids, NULL); } /* ignoring SEMICOLON for now, but it still exists as a token */
    ;
non_control_statement
    : expr SEMICOLON {struct tree *kids[10] = {$1,$2}; $$ = alctree(PR_NON_CONTROL_STATEMENT, "non_control_statement", 2, kids, NULL); }
    ;
expr /* for now, an expression is just an assignment with allowable arithmetic and logical algebra. NOTE this gets called before a primary expression in order to keep previous code working */
    : assignment_expr
    ;
logical_or_expr
    : logical_and_expr
    | logical_or_expr OR logical_and_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_LOGICAL_OR_RECUR, "logical_or_expr", 3, kids, NULL); }
    ;
logical_and_expr
    : equality_expr
    | logical_and_expr AND equality_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_LOGICAL_AND_RECUR, "logical_and_expr", 3, kids, NULL); }
    ;
equality_expr
    : relational_expr
    | equality_expr EQ relational_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_EQUALITY_EQ, "equality_expr", 3, kids, NULL); }
    | equality_expr NEQ relational_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_EQUALITY_NEQ, "equality_expr", 3, kids, NULL); }
    ;
relational_expr
    : additive_expr
    | relational_expr LT additive_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_RELATIONAL_LT, "relational_expr", 3, kids, NULL); }
    | relational_expr GT additive_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_RELATIONAL_GT, "relational_expr", 3, kids, NULL); }
    | relational_expr LTE additive_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_RELATIONAL_LTE, "relational_expr", 3, kids, NULL); }
    | relational_expr GTE additive_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_RELATIONAL_GTE, "relational_expr", 3, kids, NULL); }
    ;
assignment_expr /* assignment expression allows for one additive expression or assignment */
    : logical_or_expr
    | IDENT ASSIGN assignment_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_ASSIGNMENT_ASSIGN, "assignment_expr", 3, kids, NULL); }
    | IDENT PLUS_ASSIGN assignment_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_ASSIGNMENT_PLUS, "assignment_expr", 3, kids, NULL); }
    | IDENT MINUS_ASSIGN assignment_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_ASSIGNMENT_MINUS, "assignment_expr", 3, kids, NULL); }
    ;
additive_expr /* left recursion to create correct order of operations */
    : multiplicative_expr
    | additive_expr PLUS multiplicative_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_ADDITIVE_PLUS, "additive_expr", 3, kids, NULL); }
    | additive_expr MINUS multiplicative_expr {struct tree *kids[10] = {$1,$2, $3}; $$ = alctree(PR_ADDITIVE_MINUS, "additive_expr", 3, kids, NULL); }
    ;
multiplicative_expr /* left recursion into a unary expression */
    : unary_expr
    | multiplicative_expr MUL unary_expr {struct tree *kids[10] = {$1,$2,$3}; $$ = alctree(PR_MULT_MUL, "multiplicative_expr", 3, kids, NULL); }
    | multiplicative_expr DIV unary_expr {struct tree *kids[10] = {$1,$2,$3}; $$ = alctree(PR_MULT_DIV, "multiplicative_expr", 3, kids, NULL); }
    | multiplicative_expr MOD unary_expr {struct tree *kids[10] = {$1,$2,$3}; $$ = alctree(PR_MULT_MOD, "multiplicative_expr", 3, kids, NULL); }
    ;
unary_expr /* all unary expressions as well as a primary expression, which will allow for function calls as well as expressions again*/
    : primary_expr
    | MINUS unary_expr {struct tree *kids[10] = {$1,$2}; $$ = alctree(PR_UNARY_MINUS, "unary_expr", 2, kids, NULL); }
    | NOT unary_expr {struct tree *kids[10] = {$1,$2}; $$ = alctree(PR_UNARY_NOT, "unary_expr", 2, kids, NULL); }
    | INC unary_expr {struct tree *kids[10] = {$1,$2}; $$ = alctree(PR_UNARY_INC, "unary_expr", 2, kids, NULL); }
    | DEC unary_expr {struct tree *kids[10] = {$1,$2}; $$ = alctree(PR_UNARY_DEC, "unary_expr", 2, kids, NULL); }
    ;
primary_expr /* refactored expr into primary_expr, that includes everything from before as well as new logic to expressions */
    : function_call
    | IDENT
    | literal
    | LPAREN expr RPAREN {struct tree *kids[10] = {$1,$2,$3}; $$ = alctree(PR_PRIMARY_PAREN, "primary_expr", 3, kids, NULL); }
    | bool_literal
    ;
function_call /* a function call is a form of expression that calls functions */
    : IDENT LPAREN function_call_values_list RPAREN {struct tree *kids[10] = {$1,$2,$3,$4}; $$ = alctree(PR_FUNCTION_CALL, "function_call", 4, kids, NULL); }
    ;
function_call_values_list /* this list of arguments within that function call, acts as functionValueParameters */
    : functionCallVal
    | function_call_values_list COMMA functionCallVal {struct tree *kids[10] = {$1,$2,$3}; $$ = alctree(PR_CALL_VALUES_RECUR, "function_call_values_list", 3, kids, NULL); }
    | {$$ = NULL; };
    ;
functionCallVal
    : literal
    | IDENT
    | LPAREN literal RPAREN DOT function_call {struct tree *kids[10] = {$1,$2,$3,$4,$5}; $$ = alctree(PR_FUNCTION_CALL_OBJECT_CALL, "functionCallVal", 5, kids, NULL);}
    ;
block /* cannot have variable declarations/initiations, since those are only allowed at the global level and top of functions*/
    : LBRACE statement_list RBRACE {struct tree *kids[10] = {$1,$2,$3}; $$ = alctree(PR_BLOCK, "block", 3, kids, NULL); }
    ;
loop_statement /* we are supporting for loops and while loops */
    : for_statement
    | while_statement
    ;
for_statement /* enforced form: for ( x in y..z) {} */
    : FOR LPAREN IDENT IN IDENT RANGE_INCL IDENT RPAREN control_structure_body {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6,$7,$8,$9}; $$ = alctree(PR_FOR_IDENT_IDENT, "for_statement", 9, kids, NULL); } // for ( x in y..z) {}
    | FOR LPAREN IDENT IN literal RANGE_INCL IDENT RPAREN control_structure_body {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6,$7,$8,$9}; $$ = alctree(PR_FOR_LITERAL_IDENT, "for_statement", 9, kids, NULL); } // for ( x in 1..z) {}
    | FOR LPAREN IDENT IN IDENT RANGE_INCL literal RPAREN control_structure_body {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6,$7,$8,$9}; $$ = alctree(PR_FOR_IDENT_LITERAL, "for_statement", 9, kids, NULL); } // for ( x in y..10) {}
    | FOR LPAREN IDENT IN literal RANGE_INCL literal RPAREN control_structure_body {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6,$7,$8,$9}; $$ = alctree(PR_FOR_LITERAL_LITERAL, "for_statement", 9, kids, NULL); } // for ( x in 1..10) {}
    ;
while_statement /* enforced form: while ( expr ) {} */
    : WHILE LPAREN expr RPAREN control_structure_body {struct tree *kids[10] = {$1,$2,$3,$4,$5}; $$ = alctree(PR_WHILE, "while_statement", 5, kids, NULL); }
    ;
control_structure_body /* the body following a control structure like a for, while, if statement can be a block or one statement */
    : block
    | statement
    ;
if_statement /* Allowing if, if else, and if else if*/
    : IF LPAREN expr RPAREN control_structure_body %prec IFX {struct tree *kids[10] = {$1,$2,$3,$4,$5}; $$ = alctree(PR_IF_SIMPLE, "if_statement", 5, kids, NULL); }
    | IF LPAREN expr RPAREN control_structure_body ELSE control_structure_body {struct tree *kids[10] = {$1,$2,$3,$4,$5,$6,$7}; $$ = alctree(PR_IF_ELSE, "if_statement", 7, kids, NULL); }
    ;
%%

struct tree *alctree(int prodrule, char *symbolname, int nkids, struct tree *kids[10], struct token *leaf) {
    struct tree *t = malloc(sizeof(struct tree));
    if (t == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree node\n");
        exit(1);
    }
    t->id = serial++;
    t->prodrule = prodrule;
    t->symbolname = strdup(symbolname);
    t->nkids = nkids;
    t->leaf = leaf;
    t->type = NULL;

    // fill with NULL kids first before inserting real kids so that none of them are garbage
    for (int i = 0; i < 10; i++) {
        t->kids[i] = NULL;
    }
    // fill with real kids
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

