#include "symtab.h"
#include "k0gram.tab.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

struct sym_table *mksymtab()
{
        struct sym_table *t = malloc(sizeof(struct sym_table));
        if (t == NULL)
        {
                fprintf(stderr, "Failed to allocate memory for symbol table\n");
                exit(1);
        }
        t->nEntries = 1;
        t->nBuckets = 1;
        t->parent = NULL;
        t->next = NULL;
        return t;
}

int hash(struct sym_table *st, char *s)
{
        register int h = 0;
        register char c;
        while (c = *s++)
        {
                h += c & 0377;
                h *= 37;
        }
        if (h < 0)
                h = -h;
        return h % st->nBuckets;
}

// print all symbols in the given tree
void printsyms(struct tree *t)
{
        if (t == NULL)
                return;

        // Leaf node with identifier
        if (t->leaf != NULL)
        {
                if (t->leaf->category == IDENT)
                {
                        printsymbol(t->leaf->text);
                }
        }

        // Recurse on children
        for (int i = 0; i < t->nkids; i++)
        {
                printsyms(t->kids[i]);
        }
}