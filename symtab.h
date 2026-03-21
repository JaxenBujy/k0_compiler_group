struct sym_entry{
    // struct sym_table table;
    char *s;
    struct sym_entry *next;
};

struct sym_table{
    int nEntries;
    // struct sym_table *parent;
    struct sym_entry *next;
};