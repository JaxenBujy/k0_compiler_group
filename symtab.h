struct sym_entry{
    // struct sym_table table;
    char *s;
    struct sym_entry *next;
};

struct sym_table{
    int nBuckets; //Number of buckets
    int nEntries; //Nuber of symbols in the table
    struct sym_table *parent; // enclosing scope, superclass, etc.? no idea what that means
    struct sym_entry *next;   // next symbol table
    // struct sym_entry **tbl;
};