/* -*-bison-*- */
/* Scanner for subset of Kotlin language "k0" */
%option noyywrap
%option noinput
%option nounput
%{
    #include "k0gram.tab.h"
    #include "tree.h"
    #include "symtab.h"
    int alctoken(int category);
    char *consume_sval(int str_type);
    extern char *filename;
    int lineno = 1; // keep track of line number 
    int multi_line_start; // keep track of when a multi line comment begins to give error if it is unclosed
%}

whitespace      [ \t\r]+
inline_comment  \/\/[^\n]*|\/\*[^\n]\*\/
single_delim        \/\*[^\n]*
hash_line           \#[^\n]*
identifier          [a-zA-Z_][a-zA-Z0-9_]*
illegal_identifier [0-9]+[a-zA-Z_][a-zA-Z0-9_]*
basic_type "Byte"|"Short"|"Int"|"Long"|"Float"|"Double"|"Boolean"|"String"
integer_literal  [0-9]+[1-9]*|0[xX][0-9A-Fa-f]+
real_literal  ([0-9]+\.[0-9]*|\.[0-9]+)
string_literal \"([^\\\"\n]|\\.)*\"
char_literal \'(\\[ntr0b\'\"\\\$]|[^\'\\])\'
multi_string_literal \"\"\"([^\\\"]|\\.)*\"\"\"
%x COMMENT
/* RULES */
%%
 /* COMMENT state for multi-line comments */ 
"/*"    { BEGIN(COMMENT); multi_line_start = lineno; } /* switch to comment state where nothing will be counted */
<COMMENT>"*/"   {BEGIN(INITIAL); } /* switch back to initial flex state */
<COMMENT>(.) {}
<COMMENT>(\n) {lineno++;}
<COMMENT><<EOF>> {return alctoken(UNCLOSED_COMMENT);}

 /* Initial state */
\n		            { lineno++; }
{inline_comment}    {}
{hash_line}         {}
{whitespace}        {}   


 /* K0 Reserved Keywords */
"break"     { return alctoken(BREAK); }
"continue"  { return alctoken(CONTINUE); }
"do"        { return alctoken(DO); }
"else"      { return alctoken(ELSE); }
"false"     { return alctoken(K_FALSE); }
"for"       { return alctoken(FOR); }
"fun"       { return alctoken(FUN); }
"if"        { return alctoken(IF); }
"in"        { return alctoken(IN); }
"null"      { return alctoken(K_NULL); }
"return"    { return alctoken(RETURN); }
"true"      { return alctoken(K_TRUE); }
"val"       { return alctoken(VAL); }
"var"       { return alctoken(VAR); }
"when"      { return alctoken(WHEN); }
"while"     { return alctoken(WHILE); }
"import"    { return alctoken(IMPORT); }

 /* Kotline Reserved Keywords that we are not dealing with, return KEYWORD_ERROR */
"!in"           { return alctoken(KEYWORD_ERROR); }
"!is"           { return alctoken(KEYWORD_ERROR); }
"as"            { return alctoken(KEYWORD_ERROR); }
"as?"           { return alctoken(KEYWORD_ERROR); }
"class"         { return alctoken(KEYWORD_ERROR); }
"is"            { return alctoken(KEYWORD_ERROR); }
"object"        { return alctoken(KEYWORD_ERROR); }
"package"       { return alctoken(KEYWORD_ERROR); }
"super"         { return alctoken(KEYWORD_ERROR); }
"this"          { return alctoken(KEYWORD_ERROR); }
"throw"         { return alctoken(KEYWORD_ERROR); }
"try"           { return alctoken(KEYWORD_ERROR); }
"typealias"     { return alctoken(KEYWORD_ERROR); }
"typeof"        { return alctoken(KEYWORD_ERROR); }
 /* Soft keywords that we are not dealing with */
"by"           { return alctoken(KEYWORD_ERROR); }
"catch"        { return alctoken(KEYWORD_ERROR); }
"constructor"  { return alctoken(KEYWORD_ERROR); }
"delegate"     { return alctoken(KEYWORD_ERROR); }
"dynamic"      { return alctoken(KEYWORD_ERROR); }
"field"        { return alctoken(KEYWORD_ERROR); }
"file"         { return alctoken(KEYWORD_ERROR); }
"finally"      { return alctoken(KEYWORD_ERROR); }
"get"          { return alctoken(KEYWORD_ERROR); }
"init"         { return alctoken(KEYWORD_ERROR); }
"param"        { return alctoken(KEYWORD_ERROR); }
"property"     { return alctoken(KEYWORD_ERROR); }
"receiver"     { return alctoken(KEYWORD_ERROR); }
"set"          { return alctoken(KEYWORD_ERROR); }
"setparam"     { return alctoken(KEYWORD_ERROR); }
"value"        { return alctoken(KEYWORD_ERROR); }
"where"        { return alctoken(KEYWORD_ERROR); }
 /* Kotlin modifier keywords not in k0 */
"abstract"      { return alctoken(KEYWORD_ERROR); }
"actual"        { return alctoken(KEYWORD_ERROR); }
"annotation"    { return alctoken(KEYWORD_ERROR); }
"companion"     { return alctoken(KEYWORD_ERROR); }
"crossinline"   { return alctoken(KEYWORD_ERROR); }
"data"          { return alctoken(KEYWORD_ERROR); }
"enum"          { return alctoken(KEYWORD_ERROR); }
"expect"        { return alctoken(KEYWORD_ERROR); }
"external"      { return alctoken(KEYWORD_ERROR); }
"final"         { return alctoken(KEYWORD_ERROR); }
"infix"         { return alctoken(KEYWORD_ERROR); }
"inline"        { return alctoken(KEYWORD_ERROR); }
"inner"         { return alctoken(KEYWORD_ERROR); }
"internal"      { return alctoken(KEYWORD_ERROR); }
"lateinit"      { return alctoken(KEYWORD_ERROR); }
"noinline"      { return alctoken(KEYWORD_ERROR); }
"open"          { return alctoken(KEYWORD_ERROR); }
"operator"      { return alctoken(KEYWORD_ERROR); }
"out"           { return alctoken(KEYWORD_ERROR); }
"private"       { return alctoken(KEYWORD_ERROR); }
"protected"     { return alctoken(KEYWORD_ERROR); }
"public"        { return alctoken(KEYWORD_ERROR); }
"reified"       { return alctoken(KEYWORD_ERROR); }
"sealed"        { return alctoken(KEYWORD_ERROR); }
"suspend"       { return alctoken(KEYWORD_ERROR); }
"tailrec"       { return alctoken(KEYWORD_ERROR); }
"vararg"        { return alctoken(KEYWORD_ERROR); }
"it"            { return alctoken(KEYWORD_ERROR); }


 /* Operators */
"++"        { alctoken(INC); }
"--"        { return alctoken(DEC); }
"==="       { return alctoken(REQ); }
"!=="       { return alctoken(RNEQ); }
">="        { return alctoken(GTE); }
"<="        { return alctoken(LTE); }
"=="        { return alctoken(EQ); }
"!="        { return alctoken(NEQ); }
"&&"        { return alctoken(AND); }
"||"        { return alctoken(OR); }
"!!"        { return alctoken(NN_ASSERT); }
"?."        { return alctoken(SAFE_CALL); }
"?:"        { return alctoken(ELVIS); }
"?"         { return alctoken(NULLABLE); }
"..<"       { return alctoken(RANGE_EXCL); }
".."        { return alctoken(RANGE_INCL); }
"+="        { return alctoken(PLUS_ASSIGN); }
"-="        { return alctoken(MINUS_ASSIGN); }
"+"         { return alctoken(PLUS); }
"-"         { return alctoken(MINUS); }
"*"         { return alctoken(MUL); }
"/"         { return alctoken(DIV); }
"%"         { return alctoken(MOD); }
"="         { return alctoken(ASSIGN); }
">"         { return alctoken(GT); }
"<"         { return alctoken(LT); }
"!"         { return alctoken(NOT); }
"."         { return alctoken(DOT); }
"("{basic_type}")"   { return alctoken(TYPE_CAST); }
 /* Operators that produce OP_ERROR */
"~"    { return alctoken(OP_ERROR); }
"<<"   { return alctoken(OP_ERROR); }
">>"   { return alctoken(OP_ERROR); }
"&="   { return alctoken(OP_ERROR); }
"|="   { return alctoken(OP_ERROR); }
"^="   { return alctoken(OP_ERROR); }
">>="  { return alctoken(OP_ERROR); }
"<<="  { return alctoken(OP_ERROR); }
"&"    { return alctoken(OP_ERROR); }
"|"    { return alctoken(OP_ERROR); }
"^"    { return alctoken(OP_ERROR); }


 /* Punctuation */
"("     { return alctoken(LPAREN); }
")"     { return alctoken(RPAREN); }
","     { return alctoken(COMMA); }
";"     { return alctoken(SEMICOLON); }
"{"     { return alctoken(LBRACE); }
"}"     { return alctoken(RBRACE); }
":"     { return alctoken(COLON); }
"["     { return alctoken(LSQUARE); }
"]"     { return alctoken(RSQUARE); }

 /* Types */
"Byte"      { return alctoken(BYTE_TYPE); }
"Short"     { return alctoken(SHORT_TYPE); }
"Int"       { return alctoken(INT_TYPE); }
"Long"      { return alctoken(LONG_TYPE); }
"Float"     { return alctoken(FLOAT_TYPE); }
"Double"    { return alctoken(DOUBLE_TYPE); }
"Boolean"   { return alctoken(BOOLEAN_TYPE); }
"String"    { return alctoken(STRING_TYPE); }
"Array"     { return alctoken(ARRAY); }

 /* Literals */
{char_literal}          { return alctoken(CHAR); }
{integer_literal}       { return alctoken(INT); }
{real_literal}          { return alctoken(REAL); }
{string_literal}        { return alctoken(STRING); }
{multi_string_literal}  { return alctoken(MULTI_STRING); }

 /* identifier catchall */
{identifier}            { return alctoken(IDENT); }
{illegal_identifier}    { return alctoken(ILL_IDENT);}

 /* Could not match */
.      { return alctoken(COULD_NOT_MATCH); }

<<EOF>> {
    return alctoken(0);
}
%%

int alctoken(int category)
{
    struct tree *t = malloc(sizeof(struct tree));
    if (t == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree node\n");
        exit(1);
    }

    // initialize kids
    for (int i = 0; i < 10; i++) {
        t->kids[i] = NULL;
    }

    t->id = serial++;
    t->prodrule = category;
    t->symbolname = NULL;
    t->nkids = 0;

    t->leaf = malloc(sizeof(struct token));
    if (t->leaf == NULL) {
        fprintf(stderr, "Failed to allocate memory for token\n");
        exit(1);
    }

    switch (category)
    {
        case KEYWORD_ERROR:
            printf("%s:%d: scanner error: Kotlin reserved keyword: \"%s\" is not a part of k0\n",
                   filename, lineno, yytext);
            exit(1);

        case UNCLOSED_COMMENT:
            printf("%s:%d: scanner error: unclosed multi-line comment\n",
                   filename, multi_line_start);
            exit(1);

        case COULD_NOT_MATCH:
            printf("%s:%d: scanner error: \'%s\' could not be matched\n",
                   filename, lineno, yytext);
            exit(1);

        case ILL_IDENT:
            printf("%s:%d: scanner error: \'%s\' is an illegal identifier\n",
                   filename, lineno, yytext);
            exit(1);

        case OP_ERROR:
            printf("%s:%d: scanner error: \'%s\' is an illegal operator\n",
                   filename, lineno, yytext);
            exit(1);

        default:
            break;
    }

    // fill token fields
    t->leaf->category = category;
    t->leaf->text = strdup(yytext);
    t->leaf->lineno = lineno;
    t->leaf->filename = strdup(filename);

    t->leaf->ival = 0;
    t->leaf->dval = 0.0;
    t->leaf->sval = NULL;

    switch (category)
    {
        case INT:
            t->leaf->ival = atoi(yytext);
            break;

        case REAL:
            t->leaf->dval = atof(yytext);
            break;

        case STRING:
        case MULTI_STRING:
            t->leaf->sval = consume_sval(category);
            if (t->leaf->sval == NULL) {
                printf("%s:%d: scanner error: unsupported escape sequence in %s\n",
                       filename, lineno, yytext);
                exit(1);
            }
            break;
    }

    yylval.treeptr = t;
    return category;
}

char *consume_sval(int str_type)
{
    // allocate worst-case size (raw string length including escapes and quotes)
    char *tmp = (char *)malloc(yyleng + 1);
    char *start = tmp;

    // pointer to iterate over the raw lexer text
    char *raw_text = yytext + 1;     // skip initial quote
    char *end = yytext + yyleng - 1; // stop before final quote

    if (str_type == MULTI_STRING)
    {
        raw_text = yytext + 3;
        end = yytext + yyleng - 3;
    }

    // iterate through raw text, consuming escapes and copying characters
    while (raw_text < end)
    {
        // encountered a backslash, meaning an escape sequence
        if (*raw_text == '\\')
        {
            raw_text++; // move to escaped character

            // check what the escaped character is
            switch (*raw_text)
            {
            case 'n': // consume newline
                *tmp = '\n';
                break;
            case 't': // consume tab
                *tmp = '\t';
                break;
            case '\\': // consume literal backslash
                *tmp = '\\';
                break;
            case '\'': // consume apostrophe
                *tmp = '\'';
                break;
            case '\"': // consume quote
                *tmp = '\"';
                break;
            case 'r': // consume carriage return
                *tmp = '\r';
                break;
            case 'b': // consume backspace
                *tmp = '\b';
                break;
            default:
                // unsupported escape sequence
                free(start);
                return NULL;
            }
        }
        else
        {
            // regular character, copy as-is
            *tmp = *raw_text;
        }

        tmp++;
        raw_text++;
    }

    // null-terminate the final string value
    *tmp = '\0';

    return start;
}