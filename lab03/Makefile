CFLAGS = -g -Wall -ansi -pedantic

all: htable

htable: htable.c
	gcc -o htable $(CFLAGS) htable.c

test: htable
	echo Testing Tree
	./htable /tests/05_binary
	echo done
