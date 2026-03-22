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
void print_graph(struct tree *t, char *filename);
void print_graph2(struct tree *t, FILE *f);
void print_leaf(struct tree *t, FILE *f);
void print_branch(struct tree *t, FILE *f);
char *pretty_print_name(struct tree *t);
char *escape(char *s);
struct sym_table mksymtab();
int hash(struct sym_table st, char *s);


int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("\nerror: at least two arguments required\nUsage: ./ko <filename> <-dot>\n");
        exit(1);
    }
    int dot = 0; // bool flag to determine if dot will be used to produce png image of AST
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-dot") == 0)
        {
            dot = i; // -dot provided, turn the flag on
        }
    }
    switch (dot)
    {
    case 1:
        filename = argv[2];
        break;
    case 2:
        filename = argv[1];
        break;
    default:
        filename = argv[1];
    }

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
        printf("\n\n--------------------------------------------------------------------\n");
        printf("\n\nprintnode() output to prove leaf node information above ^. Beginning tree traversal/printing below:\n\n");
        printf("\n--------------------------------------------------------------------\n\n");
        if (dot)
        {
            print_graph(root, filename);
            printf("png image of file generated in %s_tree.png", filename);
        }
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

/* add a \ before leading and trailing double quotes */
char *escape(char *s)
{
    if (s == NULL)
        return NULL;

    int len = strlen(s);
    char *s2 = malloc(len * 2 + 1); // worst case: every char is "
    int i, j = 0;

    for (i = 0; i < len; i++)
    {
        if (s[i] == '"')
        {
            s2[j++] = '\\';
        }
        s2[j++] = s[i];
    }

    s2[j] = '\0';
    return s2;
}

char *pretty_print_name(struct tree *t)
{
    char *s2 = malloc(128); // larger buffer

    if (t->leaf == NULL)
    {
        sprintf(s2, "%s#%d", t->symbolname, t->prodrule % 10);
        return s2;
    }
    else
    {
        sprintf(s2, "%s:%d", escape(t->leaf->text), t->leaf->category);
        return s2;
    }
}

void print_branch(struct tree *t, FILE *f)
{
    char *name = pretty_print_name(t);
    fprintf(f, "N%d [shape=box label=\"%s\"];\n", t->id, name);
    free(name);
}

void print_leaf(struct tree *t, FILE *f)
{
    char *s = t->leaf->text;

    fprintf(f, "N%d [shape=box style=dotted label=\" %s \\n ",
            t->id, escape(s));

    if (t->leaf->category == STRING)
    {
        fprintf(f, "ID: #%d \\l text = %s \\l lineno = %d\\l\"];\n",
                t->id,
                escape(t->leaf->sval),
                t->leaf->lineno);
    }
    else
    {
        fprintf(f, "ID: #%d \\l text = %s \\l lineno = %d\\l\"];\n",
                t->id,
                escape(t->leaf->text),
                t->leaf->lineno);
    }
}

void print_graph2(struct tree *t, FILE *f)
{
    int i;

    if (t->leaf != NULL)
    {
        print_leaf(t, f);
        return;
    }

    /* not a leaf ==> internal node */
    print_branch(t, f);

    for (i = 0; i < t->nkids; i++)
    {
        if (t->kids[i] != NULL)
        {
            fprintf(f, "N%d -> N%d;\n", t->id, t->kids[i]->id);
            print_graph2(t->kids[i], f);
        }
    }
}

void print_graph(struct tree *t, char *filename)
{
    char output_file[56];

    snprintf(output_file, sizeof(output_file), "%s_tree.dot", filename);

    FILE *f = fopen(output_file, "w");
    if (!f)
    {
        printf("error opening dot output file\n");
        return;
    }

    fprintf(f, "digraph {\n");
    print_graph2(t, f);
    fprintf(f, "}\n");

    fclose(f);
}