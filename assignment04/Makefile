FLAGS = -g -Wall -pedantic

all: main functions mytar ustar table

mytar: main.c
	gcc $(FLAGS) main.c -o mytar

ustar: ustarformat.c
	gcc $(FLAGS) ustarformat.c -c

table: table.c
	gcc $(FLAGS) table.c -c

create: create.c
	gcc $(FLAGS) create.c -c

functions: functions.c
	gcc $(FLAGS) functions.c -c
