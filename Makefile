CC = gcc
CFLAGS = -Wall -Wextra -g

all: k0 k0rt.o

k0: k0gram.tab.o lex.yy.o main.o tree.o symtab.o tac.o x86gen.o
	$(CC) $(CFLAGS) k0gram.tab.o lex.yy.o main.o tree.o symtab.o tac.o x86gen.o -o k0 -lfl

k0gram.tab.c k0gram.tab.h: k0gram.y
	bison -dv -t -Wcounterexamples k0gram.y

lex.yy.c: k0lex.l k0gram.tab.h symtab.h tree.h
	flex k0lex.l

k0gram.tab.o: k0gram.tab.c
	$(CC) $(CFLAGS) -c k0gram.tab.c

lex.yy.o: lex.yy.c tree.h symtab.h
	$(CC) $(CFLAGS) -c lex.yy.c

main.o: main.c k0gram.tab.h
	$(CC) $(CFLAGS) -c main.c
tree.o: tree.h
	$(CC) $(CFLAGS) -c tree.c
symtab.o: symtab.h
	$(CC) $(CFLAGS) -c symtab.c
tac.o: tac.h
	$(CC) $(CFLAGS) -c tac.c
x86gen.o: x86gen.c x86gen.h tac.h
	$(CC) $(CFLAGS) -c x86gen.c
k0rt.o: k0rt.c
	$(CC) $(CFLAGS) -c k0rt.c

clean:
	rm -f k0 *.o lex.yy.c k0gram.tab.c k0gram.tab.h k0gram.output *.s *.ic

