CFLAGS = -g -ansi -pedantic

all: mush parseline

mush: mush.c
	gcc mush.c -o mush $(CFLAGS)

parseline: parseline.c
	gcc parseline.c -c $(CFLAGS)

pipe: pipe.c
	gcc pipe.c -o trying -g

clean:
	rm *.o mush
