/*  Name:   Sinjun Jang
    Assgn:  Lab 02
    Prog:   uniq
    Desc:   discards any idetical lines. then prints out lines to stdout
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE    100
#define POINTER 24

void *safe_malloc(size_t size) {
    void *new;
    new = malloc(size);

    if (!new) {
        perror("safe_malloc");
        exit(EXIT_FAILURE);
    }
    return new;
}

char *read_long_line(FILE *file) {
    char *lineptr = NULL;
    char *lasttmp = NULL;
    size_t bufflen = 0;
    size_t size = SIZE;
    char ch;

    lineptr = safe_malloc(size);

    while((ch = fgetc(file)) != '\n' && ch != EOF) {
        lineptr[bufflen++] = ch;

        if (bufflen + 1 >= size) {
            char *tmp = realloc(lineptr, size * 2);

            if (!tmp) {
                perror("realloc failed in read_long_line");
                exit(EXIT_FAILURE);
            }
        lineptr = tmp;
        size *= 2;
        }

    }
    /*add zero char to end*/
    lineptr[bufflen] = '\0';

    if (bufflen == 0 && ch ==EOF){
        free(lineptr);
        return NULL;
    }
    /*tmp pointer for lineptr before reallocing to size of string*/
    lasttmp = realloc(lineptr, bufflen + 1);

    if (!lasttmp) {
        perror("realloc failed line at end of read_long_line");
        exit(EXIT_FAILURE);
    }

    lineptr = lasttmp;
    return lineptr;
}

int main(int argc, char **argv) {

    FILE *fp;
    int i;
    int k;
    char *filename;
    char *line;
    char **all_lines;
    char *prev_line = NULL;
    size_t pointer_buffer = 0;
    size_t size_double_pointer = POINTER;

    filename = argv[1];

    fp = fopen(filename, "r");

    if (!fp){
        perror("file opening error");
        exit(EXIT_FAILURE);
    }

    all_lines = NULL;
    all_lines = safe_malloc(size_double_pointer);

    i = 0;
    
    /*
    while ((line = read_long_line(fp))){
        if (i > 0){
            int repeat = 0;
            int j;
            for(j = 0; j < i; j++){
                if (strcmp(line, all_lines[j]) == 0){
                    repeat = 1;
                }

            }
            if (repeat==0){
                pointer_buffer += 8;
                if (pointer_buffer >= size_double_pointer){
                    char **tmp = realloc(all_lines, size_double_pointer*2);
                    if(!tmp){
                        perror("realloc, no mem left");
                        exit(EXIT_FAILURE);
                    }
                    all_lines = tmp;
                    size_double_pointer *= 2;

                }
                all_lines[i] = line;
                i++;
            }
            else{free(line);}
        }
        else{
            all_lines[i] = line;
            i++;
        }
    }*/

    while((line = read_long_line(fp))){

        if (!prev_line){
            size_t size = strlen(line);
            prev_line = safe_malloc(size);
            prev_line = line;
            printf("%s", line);
        }
        else{
            if(!strcmp(prev_line, line)){
                printf("%s", line);
                free(prev_line);
                size_t size = strlen(line);
                prev_line = safe_malloc(size);
                prev_line = line;
            }
        }
    }

    k = 0;
    /*
    while(k < i){
        printf("%s\n", all_lines[k]);
        free(all_lines[k]);
        k++;
    }
    */
    free(all_lines);
    fclose(fp);

    return 0;
}
