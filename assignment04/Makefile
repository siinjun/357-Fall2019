FLAGS = -g -Wall -pedantic

all: main functions mytar ustar table

mytar: mytar.c
	gcc $(FLAGS) mytar.c -c

ustar: ustarformat.c
	gcc $(FLAGS) ustarformat.c -c

table: table.c
	gcc $(FLAGS) table.c -c

main: main.c
	gcc $(FLAGS) main.c -o mytar

functions: functions.c
	gcc $(FLAGS) functions.c -c
