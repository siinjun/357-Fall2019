CFLAGS = -Wall -ansi -pedantic -g
CC = gcc

all: uniq

uniq: uniq.c
	gcc -o uniq uniq.c $(CFLAGS)

test: uniq
	echo Testing uniq
	./uniq  input.txt
	echo done.

clean:
	rm -f *.o

