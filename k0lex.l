/* -*-bison-*- */
/* Scanner for subset of Kotlin language "k0" */
%option noyywrap
%option noinput
%option nounput
%{
    #include "k0gram.tab.h"
    #include "tree.h"
    int alctoken(int category);
    char *consume_sval(int str_type);
    extern char *filename;
    int lineno = 1; // keep track of line number 
    int multi_line_start; // keep track of when a multi line comment begins to give error if it is unclosed
%}

whitespace  [ \t\r]+
inline_comment  \/\/[^\n]*|\/\*[^\n]\*\/
single_delim \/\*[^\n]*
hash_line  \#[^\n]*
identifier  [a-zA-Z_][a-zA-Z0-9_]*
illegal_identifier [0-9]+[a-zA-Z_][a-zA-Z0-9_]*
basic_type "byte"|"short"|"int"|"long"|"float"|"double"|"boolean"|"string"
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
<COMMENT><<EOF>> {return UNCLOSED_COMMENT;}

 /* Initial state */
\n		            { lineno++; }
{inline_comment}    {}
{hash_line}         {}
{whitespace}        {}   


 /* K0 Reserved Keywords */
"break"     { return BREAK; }
"continue"  { return CONTINUE; }
"do"        { return DO; }
"else"      { return ELSE; }
"false"     { return K_FALSE; }
"for"       { return FOR; }
"fun"       { return FUN; }
"if"        { return IF; }
"in"        { return IN; }
"null"      { return K_NULL; }
"return"    { return RETURN; }
"true"      { return K_TRUE; }
"val"       { return VAL; }
"var"       { return VAR; }
"when"      { return WHEN; }
"while"     { return WHILE; }
"import"    { return IMPORT; }

 /* Kotline Reserved Keywords that we are not dealing with, return KEYWORD_ERROR */
"!in"           { return KEYWORD_ERROR; }
"!is"           { return KEYWORD_ERROR; }
"as"            { return KEYWORD_ERROR; }
"as?"           { return KEYWORD_ERROR; }
"class"         { return KEYWORD_ERROR; }
"is"            { return KEYWORD_ERROR; }
"object"        { return KEYWORD_ERROR; }
"package"       { return KEYWORD_ERROR; }
"super"         { return KEYWORD_ERROR; }
"this"          { return KEYWORD_ERROR; }
"throw"         { return KEYWORD_ERROR; }
"try"           { return KEYWORD_ERROR; }
"typealias"     { return KEYWORD_ERROR; }
"typeof"        { return KEYWORD_ERROR; }
 /* Soft keywords that we are not dealing with */
"by"           { return KEYWORD_ERROR; }
"catch"        { return KEYWORD_ERROR; }
"constructor"  { return KEYWORD_ERROR; }
"delegate"     { return KEYWORD_ERROR; }
"dynamic"      { return KEYWORD_ERROR; }
"field"        { return KEYWORD_ERROR; }
"file"         { return KEYWORD_ERROR; }
"finally"      { return KEYWORD_ERROR; }
"get"          { return KEYWORD_ERROR; }
"init"         { return KEYWORD_ERROR; }
"param"        { return KEYWORD_ERROR; }
"property"     { return KEYWORD_ERROR; }
"receiver"     { return KEYWORD_ERROR; }
"set"          { return KEYWORD_ERROR; }
"setparam"     { return KEYWORD_ERROR; }
"value"        { return KEYWORD_ERROR; }
"where"        { return KEYWORD_ERROR; }
 /* Kotlin modifier keywords not in k0 */
"abstract"      { return KEYWORD_ERROR; }
"actual"        { return KEYWORD_ERROR; }
"annotation"    { return KEYWORD_ERROR; }
"companion"     { return KEYWORD_ERROR; }
"crossinline"   { return KEYWORD_ERROR; }
"data"          { return KEYWORD_ERROR; }
"enum"          { return KEYWORD_ERROR; }
"expect"        { return KEYWORD_ERROR; }
"external"      { return KEYWORD_ERROR; }
"final"         { return KEYWORD_ERROR; }
"infix"         { return KEYWORD_ERROR; }
"inline"        { return KEYWORD_ERROR; }
"inner"         { return KEYWORD_ERROR; }
"internal"      { return KEYWORD_ERROR; }
"lateinit"      { return KEYWORD_ERROR; }
"noinline"      { return KEYWORD_ERROR; }
"open"          { return KEYWORD_ERROR; }
"operator"      { return KEYWORD_ERROR; }
"out"           { return KEYWORD_ERROR; }
"private"       { return KEYWORD_ERROR; }
"protected"     { return KEYWORD_ERROR; }
"public"        { return KEYWORD_ERROR; }
"reified"       { return KEYWORD_ERROR; }
"sealed"        { return KEYWORD_ERROR; }
"suspend"       { return KEYWORD_ERROR; }
"tailrec"       { return KEYWORD_ERROR; }
"vararg"        { return KEYWORD_ERROR; }
"it"            { return KEYWORD_ERROR; }


 /* Operators */
"++"        { alctoken(INC); }
"--"        { return DEC; }
"==="       { return REQ; }
"!=="       { return RNEQ; }
">="        { return GTE; }
"<="        { return LTE; }
"=="        { return EQ; }
"!="        { return NEQ; }
"&&"        { return AND; }
"||"        { return OR; }
"!!"        { return NN_ASSERT; }
"?."        { return SAFE_CALL; }
"?:"        { return ELVIS; }
"?"         { return NULLABLE; }
"..<"       { return RANGE_EXCL; }
".."        { return RANGE_INCL; }
"+="        { return PLUS_ASSIGN; }
"-="        { return MINUS_ASSIGN; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MUL; }
"/"         { return DIV; }
"%"         { return MOD; }
"="         { return ASSIGN; }
">"         { return GT; }
"<"         { return LT; }
"!"         { return NOT; }
"."         { return DOT; }
"("{basic_type}")"   { return TYPE_CAST; }
 /* Operators that produce OP_ERROR */
"~"    { return OP_ERROR; }
"<<"   { return OP_ERROR; }
">>"   { return OP_ERROR; }
"&="   { return OP_ERROR; }
"|="   { return OP_ERROR; }
"^="   { return OP_ERROR; }
">>="  { return OP_ERROR; }
"<<="  { return OP_ERROR; }
"&"    { return OP_ERROR; }
"|"    { return OP_ERROR; }
"^"    { return OP_ERROR; }


 /* Punctuation */
"("     { return LPAREN; }
")"     { return RPAREN; }
","     { return COMMA; }
";"     { return SEMICOLON; }
"{"     { return LBRACE; }
"}"     { return RBRACE; }
":"     { return COLON; }
"["     { return LSQUARE; }
"]"     { return RSQUARE; }

 /* Types */
"Byte"      { return BYTE_TYPE; }
"Short"     { return SHORT_TYPE; }
"Int"       { return INT_TYPE; }
"Long"      { return LONG_TYPE; }
"Float"     { return FLOAT_TYPE; }
"Double"    { return DOUBLE_TYPE; }
"Boolean"   { return BOOLEAN_TYPE; }
"String"    { return STRING_TYPE; }
"Array"     { return ARRAY; }

 /* Literals */
{char_literal}          { return CHAR; }
{integer_literal}       { return alctoken(INT); }
{real_literal}          { return REAL; }
{string_literal}        { return STRING; }
{multi_string_literal}  { return MULTI_STRING; }

 /* identifier catchall */
{identifier}            { return IDENT; }
{illegal_identifier}    { return ILL_IDENT;}

 /* Could not match */
.      { return COULD_NOT_MATCH; }

<<EOF>> {
    return 0;
}
%%

int alctoken(int category)
{
    struct tree *t = malloc(sizeof(struct tree));

    t->prodrule = category;
    t->symbolname = NULL;
    t->nkids = 0;

    t->leaf = malloc(sizeof(struct token));
    t->leaf->category = category;

    t->kids[0] = NULL; // optional cleanup

    yylval.treeptr = t;

    switch (category)
    {
    case KEYWORD_ERROR:
        printf("%s:%d: scanner error: Kotlin reserved keyword: \"%s\" is not a part of k0\n", filename, lineno, yytext);
        exit(1);
    case UNCLOSED_COMMENT:
        printf("%s:%d: scanner error: unclosed multi-line comment\n", filename, multi_line_start);
        exit(1);
    case COULD_NOT_MATCH:
        printf("%s:%d: scanner error: \'%s\' could not be matched\n", filename, lineno, yytext);
        exit(1);
    case ILL_IDENT:
        printf("%s:%d: scanner error: \'%s\' is an illegal identifier\n", filename, lineno, yytext);
        exit(1);
    case OP_ERROR:
        printf("%s:%d: scanner error: \'%s\' is an illegal operator\n", filename, lineno, yytext);
        exit(1);
    default: 
        break;
    }

    // set general fields
    t->leaf->category = category;
    t->leaf->text = strdup(yytext);
    t->leaf->lineno = lineno;
    t->leaf->filename = strdup(filename);

    // set defaults for literal vals
    t->leaf->ival = 0;
    t->leaf->dval = 0.0;
    t->leaf->sval = NULL;

    // set specific literal fields depending on data
    switch (category)
    {
    case INT:
        t->leaf->ival = atoi(yytext);
        break;
    case REAL:
        t->leaf->dval = atof(yytext);
        break;
    case STRING:
        t->leaf->sval = consume_sval(STRING);
        if (t->leaf->sval == NULL)
        {
            printf("%s:%d: scanner error: unsupported escape sequence in %s\n", filename, lineno, yytext);
            exit(1);
        }
        break;
    case MULTI_STRING:
        t->leaf->sval = consume_sval(MULTI_STRING);
        if (t->leaf->sval == NULL)
        {
            printf("%s:%d: scanner error: unsupported escape sequence in %s\n", filename, lineno, yytext);
            exit(1);
        }
        break;
    }

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