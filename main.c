/* Lab 3 version of main. All linked list stuff is pointless as we are just calling yyparse once
yyparse is called once to see its return value, so the loop of tokens being returned is no longer happening right now */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "k0gram.tab.h"
#include "symtab.h"

extern FILE *yyin;
extern char *yytext;
extern int yyleng;
extern int lineno;
extern int multi_line_start;
extern int yyparse();
extern int yydebug;
extern struct tree *root;
int exit_status = 0; // status that main will return. 0 = no errors, 1 = lexical error, 2 = syntax error, 3 = semantic error

char *filename; // defined globally to share with k0lex.l
void print_graph(struct tree *t, char *filename);
void print_graph2(struct tree *t, FILE *f);
void print_leaf(struct tree *t, FILE *f);
void print_branch(struct tree *t, FILE *f);
char *pretty_print_name(struct tree *t);
char *escape(char *s);

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("\nError: at least file name required\nUsage: ./ko <filename> <options>\n");
        printf("OPTIONAL:\n");
        printf("-dot: generate a dot file of the syntax tree\n");
        printf("-tree: print the syntax tree\n");
        printf("-symtab: print the syntax tree\n");
        printf("-none: prints nothing\n");
        exit(1);
    }
    int dot_bool = 0;    // bool flag to determine if dot will be used to produce png image of AST
    int tree_bool = 0;   // bool flag to determine if tree will be printed
    int symtab_bool = 0; // bool flag to determine if symbol table will be printed
    int none_bool = 0;

    // filename is required to come first
    filename = argv[1];

    // collect any optional arguments
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-dot") == 0)
        {
            dot_bool = 1; // -dot provided, turn the flag on
        }
        if (strcmp(argv[i], "-tree") == 0)
        {
            tree_bool = 1;
        }
        if (strcmp(argv[i], "-symtab") == 0)
        {
            symtab_bool = 1;
        }
        if (strcmp(argv[i], "-none") == 0)
        {
            none_bool = 1;
        }
    }

    printf("Opening file '%s'...\n\n", filename);
    yyin = fopen(filename, "r");
    if (!yyin)
    {
        fprintf(stderr, "Error reading file '%s'\n", filename);
        exit(1);
    }

    // for debug prints from bison
    yydebug = 0;
    int rv = yyparse(); // parse the given file, constructing an AST

    if (rv == 0 && exit_status == 0) // yyparse returned no syntax errors
    {
        // construct the symbol table
        int symtab_err_flag = 0;

        // global scope, predefined functions in k0 specification
        struct sym_table *global = mksymtab_global(64);

        // package scope, the current file being read
        struct sym_table *current_package = mksymtab(64);
        current_package->parent = global;

        // set the current package as a child of the global table
        current_package->sibling = global->child;
        global->child = current_package;

        // set package name to be the name of the file being read
        size_t len = strlen("package ") + strlen(filename) + 1;
        current_package->scope_name = malloc(len);
        snprintf(current_package->scope_name, len, "package %s", filename);

        // build symbol table starting at package scope
        build_symtab(root, current_package, &symtab_err_flag, filename);

        // print the tree if specified
        if (tree_bool)
        {
            printf("\n---print_tree output---\n");
            print_tree(root);
        }
        // make dot file of the tree if specified
        if (dot_bool)
        {
            print_graph(root, filename);
            printf("dot file of AST generated in %s_tree.dot. To generate png image of the tree, run \"dot -Tpng %s_tree.dot > tree_img.png\"\n", filename, filename);
        }
        if(none_bool)
        {
            printf("used for testing and clarity of output\n");
        }

        // if no semantic errors were found
        if (!symtab_err_flag)
        {
            printf("No semantic errors found in symbol tables\n");
            // print symbol table if specified
            if (symtab_bool)
            {
                printf("\n---print_symtab output---\n");
                print_symtab(global, 0);
            }
        }
        else
        {

            exit_status = 3; // exit status 3 for semantic errors
        }
    }
    else
    {
        // if no scanner errors were found, otherwise exit_status would be 1
        if (exit_status == 0)
        {
            exit_status = 2; // exit status 2 for parser errors
        }
    }

    free_tree(root);
    printf("\nexiting with status %d...\n\n", exit_status);
    return exit_status;
}

int yyerror(const char *s)
{
    fprintf(stderr, "%s:%d: parser error: %s\n", filename, lineno, s); // print sytax error message with file name and line number
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