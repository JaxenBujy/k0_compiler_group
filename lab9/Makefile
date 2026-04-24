CC = gcc
CFLAGS = -Wall -Wextra -g

all: tac

tac: tac.c tac.h
	$(CC) $(CFLAGS) tac.c -o tac -lfl

clean:
	rm -f tac *.o