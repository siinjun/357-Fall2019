CFLAGS = -Wall -ansi -pedantic -g
CC = gcc

all: fw

fw: fw.c
	gcc -o fw fw.c $(CFLAGS) 

hashtable: hashtable.c
	gcc -o hashtable -g $(CFLAGS) hashtable.c

test : hashtable
	echo Testing hashtable
	./hashtable
	echo done

clean:
	rm -f *.o
