/* Lab 3 version of main. All linked list stuff is pointless as we are just calling yyparse once
yyparse is called once to see its return value, so the loop of tokens being returned is no longer happening right now */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "k0gram.tab.h"

extern FILE *yyin;
extern char *yytext;
extern int yyleng;
extern int lineno;
extern int multi_line_start;
extern int yyparse();
extern int yydebug;

char *filename; // defined globally to share with k0lex.l

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("2 arguments required\n");
        exit(1);
    }
    filename = argv[1];
    yyin = fopen(filename, "r");
    if (!yyin)
    {
        printf("Error reading file\n");
        exit(1);
    }

    // for debug prints from bison
    yydebug = 0;
    int rv = yyparse(); // call yyparse once instead of yylex() in a while loop
    printf("yyparse returned %d\n", rv);

    return 0;
}

int yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}
