CFLAGS = -g -Wall -ansi -pedantic

all: hencode

hencode: hencode.c
	gcc -o a.out $(CFLAGS) hencode.c
hdecode: hdecode.c
	gcc -o b.out $(CFLAGS) hdecode.c

test : hencode
	echo "testing hencode"
	./a.out inputs/basic basic.mine
	echo "done"

clean:
	rm -rf *.o
