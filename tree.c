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

        // Print leaf if present
        if (root->leaf != NULL)
        {
                for (int i = 0; i < depth + 1; i++)
                        printf("  ");
                printf("Leaf Node: %s\n",
                       root->leaf->text);
        }
        else
        {
                // Print tree node info
                printf("Internal Node: %s\n",
                       root->symbolname);
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
