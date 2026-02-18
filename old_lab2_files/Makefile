all: scanner

scanner: flex_main.o lex.yy.o
	gcc -g -Wall -o scanner flex_main.o lex.yy.o
lex.yy.c: k0lex.l
	flex k0lex.l
lex.yy.o: lex.yy.c
	gcc -g -Wall -c lex.yy.c
flex_main.o: flex_main.c
	gcc -g -Wall -c flex_main.c
clean:
	rm -f *.o lex.yy.c scanner
