CFLAGS = -g -Wall -ansi -pedantic

all: hencode hdecode

hencode: hencode.c
	gcc -o hencode $(CFLAGS) hencode.c
hdecode: hdecode.c
	gcc -o hdecode $(CFLAGS) hdecode.c

test : hencode
	echo "testing hencode"
	./a.out inputs/basic basic.mine
	echo "done"

clean:
	rm -rf *.o
