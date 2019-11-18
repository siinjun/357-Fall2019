FLAGS = -g -Wall -pedantic

all: mytar ustar table mine create

mytar: main.c table create mine
	gcc $(FLAGS) main.c -o mytar

ustar: ustarformat.c
	gcc $(FLAGS) ustarformat.c -c

table: table.c
	gcc $(FLAGS) table.c -c

create: create.c
	gcc $(FLAGS) create.c -c

mine: extraction.c
	gcc $(FLAGS) extraction.c -c
