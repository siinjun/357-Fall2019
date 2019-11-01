CFLAGS = -g -Wall -pedantic

all: mypwd

mypwd: mypwd.c
	gcc -o mypwd $(CFLAGS) mypwd.c

test: mypwd
	echo "testing"
	./mypwd
	echo "done"

clean:
	rm *.o
