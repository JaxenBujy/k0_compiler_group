#include <stdio.h>
#include <stdlib.h>

struct tree
{
    int prodrule;
    char *symbolname;
    int nkids;
    struct tree *kids[10]; /* if nkids >0 */
    struct token *leaf;    /* if nkids == 0; NULL for ε productions */
};

struct token
{
    int category;   /* the integer code returned by yylex */
    char *text;     /* the actual string (lexeme) matched */
    int lineno;     /* the line number on which the token occurs */
    char *filename; /* the source file in which the token occurs */
    int ival;       /* for integer constants, store binary value here */
    double dval;    /* for real constants, store binary value here */
    char *sval;     /* for string constants, malloc space, de-escape, store the string (less quotes and after escapes) here */
};

void print_tree(struct tree *root);
void print_tree_with_depth(struct tree *root, int depth);
void free_tree(struct tree *root);
void free_tree_with_depth(struct tree *root, int depth);
