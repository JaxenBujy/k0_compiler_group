CC = gcc
CFLAGS = -Wall -Wextra -g

all: k0

k0: k0gram.tab.o lex.yy.o main.o
	$(CC) $(CFLAGS) k0gram.tab.o lex.yy.o main.o -o k0 -lfl

k0gram.tab.c k0gram.tab.h: k0gram.y
	bison -d k0gram.y

lex.yy.c: k0lex.l k0gram.tab.h
	flex k0lex.l

k0gram.tab.o: k0gram.tab.c
	$(CC) $(CFLAGS) -c k0gram.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

main.o: main.c k0gram.tab.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f k0 *.o lex.yy.c k0gram.tab.c k0gram.tab.h

