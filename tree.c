#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

void print_tree(struct tree *root)
{
        print_tree_with_depth(root, 0);
}

void print_tree_with_depth(struct tree *root, int depth)
{
        if (!root)
                return;

        // Print indentation
        for (int i = 0; i < depth; i++)
                printf("  "); // two spaces per level

        // Print tree node info
        printf("Symbol: %s Prodrule: %d Kids: %d\n",
               root->symbolname, root->prodrule, root->nkids);

        // Print leaf if present
        if (root->leaf != NULL)
        {
                for (int i = 0; i < depth + 1; i++)
                        printf("  ");
                printf("Leaf -> Category: %d, Text: %s, Line: %d, File: %s, "
                       "ival: %d, dval: %f, sval: %s\n",
                       root->leaf->category, root->leaf->text,
                       root->leaf->lineno, root->leaf->filename,
                       root->leaf->ival, root->leaf->dval, root->leaf->sval);
        }

        for (int i = 0; i < root->nkids; i++)
                print_tree_with_depth(root->kids[i], depth + 1);
}

void free_tree(struct tree *root)
{
        free_tree_with_depth(root, 0);
}

void free_tree_with_depth(struct tree *root, int depth)
{
        if (!root)
                return;

        // Free children first (post-order)
        for (int i = 0; i < root->nkids; i++)
        {
                free_tree_with_depth(root->kids[i], depth + 1);
        }

        // Free leaf fields if present
        if (root->leaf != NULL)
        {
                free(root->leaf->text);
                free(root->leaf->filename);
                free(root->leaf->sval);
                free(root->leaf);
        }

        free(root->symbolname);
        free(root);
}

void print_tree_symbols(struct tree *root)
{
        print_tree_symbols_with_depth(root, 0);
}

void print_tree_symbols_with_depth(struct tree *root, int depth)
{
        if (!root)
                return;

        // Print indentation
        for (int i = 0; i < depth; i++)
                printf("  "); // two spaces per level

        printf("%s\n", root->symbolname); // only print the symbol name for non-leaf nodes

        // Print leaf if present
        if (root->leaf != NULL)
        {
                for (int i = 0; i < depth + 1; i++)
                        printf("  ");
                printf("%s\n", root->leaf->text); // only print the lexeme text for leaf nodes
        }

        for (int i = 0; i < root->nkids; i++)
                print_tree_symbols_with_depth(root->kids[i], depth + 1);
}
