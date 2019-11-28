CFLAGS = -g -Wall -ansi -pedantic

all: mush

mush: mush.c
	gcc mush.c -o mush $(CFLAGS)

parseline: parseline.c
	gcc parseline.c -c $(CFLAGS)

clean:
	rm *.o
