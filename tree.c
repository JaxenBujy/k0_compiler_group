#include "tree.h"
#include <stdio.h>

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
        printf("Prodrule: %d Symbol: %s Kids: %d\n",
               root->prodrule, root->symbolname, root->nkids);

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

        for (int i = 0; i < root->nkids; i++)
                print_tree_with_depth(root->kids[i], depth + 1);

        // Free leaf if present
        if (root->leaf != NULL)
        {
                free(root->leaf);
        }
        free(root);
}