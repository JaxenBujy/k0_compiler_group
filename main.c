#include <stdio.h>
#include <stdlib.h>

/* Super basic main for the sole purpose of calling yyparse and reading in file names */

extern int yyparse();
extern FILE *yyin;

int main(int argc, char *argv[]){
    
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    yyin = fopen(filename, "r");
    
    int rv = yyparse();
    printf("yyparse returns %d\n", rv);
}

int yyerror (char *s){
    fprintf(stderr, "%s\n", s); exit(1);
}