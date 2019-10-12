CFLAGS = -Wall -ansi -pedantic -g
CC = gcc

all: fw

fw: fw.c
	gcc -o fw fw.c $(CFLAGS) 


test : fw
	echo Testing hashtable
	./fw input.txt
	echo done

clean:
	rm -f *.o
