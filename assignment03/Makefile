CFLAGS = -g -Wall -ansi -pedantic

all: hencode

hencode: hencode.c
	gcc $(CFLAGS) hencode.c

test: hencode
	echo "testing hencode"
	./a.out inputs/basic basic.mine
	echo "done"
