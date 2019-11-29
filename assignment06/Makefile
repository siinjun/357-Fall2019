CFLAGS = -g -ansi -pedantic

all: mush parseline process

mush: mush.c
	gcc mush.c -o mush $(CFLAGS)

parseline: parseline.c
	gcc parseline.c -c $(CFLAGS)

process: process.c
	gcc process.c -c $(CFLAGS)

clean:
	rm *.o
