FLAGS = -g -Wall -pedantic

all: mytar ustar table

mytar: mytar.c
	gcc $(FLAGS) mytar.c -o mytar

ustar: ustarformat.c
	gcc $(FLAGS) ustarformat.c -c

table: table.c
	gcc $(FLAGS) table.c -o tabletar
