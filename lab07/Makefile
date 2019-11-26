CFLAGS = -g -ansi -pedantic

all: tryit forkit

tryit: tryit.c
	gcc tryit.c $(CFLAGS) -o tryit

forkit: forkit.c
	gcc forkit.c $(CFLAGS) -o forkit

clean:
	rm *.o
