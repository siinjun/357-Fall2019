FLAGS = -g -Wall -pedantic

all: main functions mytar ustar table

mytar: mytar.c
	gcc $(FLAGS) mytar.c -o create

ustar: ustarformat.c
	gcc $(FLAGS) ustarformat.c -c

table: table.c
	gcc $(FLAGS) table.c -o tabletar

main: main.c
	gcc $(FLAGS) main.c -o mytar

functions: functions.c
	gcc $(FLAGS) functions.c -o extract
