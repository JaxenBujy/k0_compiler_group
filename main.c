/* Lab 3 version of main. All linked list stuff is pointless as we are just calling yyparse once
yyparse is called once to see its return value, so the loop of tokens being returned is no longer happening right now */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "k0gram.tab.h"
#include "tree.h"

extern FILE *yyin;
extern char *yytext;
extern int yyleng;
extern int lineno;
extern int multi_line_start;
extern int yyparse();
extern int yydebug;
extern struct tree *root;

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

    if (rv == 0) // yyparse returned no syntax errors, print AST
    {
        print_tree(root);
        free_tree(root);
        printf("yyparse returned %d\n", rv);
    }
    else
    {
        free_tree(root);
        printf("exiting with status 2...\n");
        exit(2); // exit status 2 for parser errors
    }

    return 0;
}

int yyerror(const char *s)
{
    fprintf(stderr, "\nparser error: %s:%d: %s\n", filename, lineno, s); // print sytax error message with file name and line number
    return 0;
}
