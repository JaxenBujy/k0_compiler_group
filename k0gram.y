%{
int yylex();
int yyerror(char *);
%}
%token NAME NUMBER
%%
sequence : pair sequence | ;
pair : NAME NUMBER ;
