/*  
    Name:   Sinjun Jang
    Prog:   fw
    Desc:   Read in files from stdin and output the most common
            appearing words to stdout
*/

#include "quicksort.h"
#include "hashtable.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>

#define WORD 8

int x = 0;

HashTable *read_words(FILE *fp, HashTable *ht){
    /*function reads words from a whole file and inserts them into a ht*/
    char *wordptr = NULL;
    size_t word_len = 0;
    size_t size = WORD;
    char ch;
    int alpha;

    wordptr = malloc(size);
    while ((ch = fgetc(fp)) != EOF) {
        alpha = isalpha(ch);
        if ((!alpha) && word_len > 0){
            char *tmp;
            wordptr[word_len] = '\0';
            tmp = realloc(wordptr, ++word_len);
            if (!tmp){
                perror("realloc failed");
                exit(EXIT_SUCCESS);
            }
            wordptr = tmp;
            ht = insert_ht(ht, wordptr);
            wordptr = 0x0;
            word_len = 0;
            free(wordptr);
            size = WORD;
            wordptr = malloc(size);
        }
        else if(alpha){
            /*if letter is uppercase change to lower*/
            if (isupper(ch)){
                ch = tolower(ch);
            }
            wordptr[word_len] = ch;
            word_len++;
            if(word_len >= size){
                char *tmp = realloc(wordptr, size * 2);
                if (!tmp){
                    perror("realloc failed in read_words");
                    exit(EXIT_SUCCESS);
                }
                wordptr = tmp;
                size *= 2;
            }
        }
    }
    
    free(wordptr);
    return ht;
}

HashTable *read_from_stdin(HashTable *ht){
    /*function reads words from a whole file and inserts them into a ht*/
    char *wordptr = NULL;
    size_t word_len = 0;
    size_t size = WORD;
    char ch;
    int alpha;

    wordptr = malloc(size);
    while ((ch = getchar()) != EOF) {
        alpha = isalpha(ch);
        if ((!alpha) && word_len > 0){
            char *tmp;
            wordptr[word_len] = '\0';
            tmp = realloc(wordptr, ++word_len);
            if (!tmp){
                perror("realloc failed");
                exit(EXIT_SUCCESS);
            }
            wordptr = tmp;
            ht = insert_ht(ht, wordptr);
            wordptr = 0x0;
            word_len = 0;
            free(wordptr);
            size = WORD;
            wordptr = malloc(size);
        }
        else if(alpha){
            /*if letter is uppercase change to lower*/
            if (isupper(ch)){
                ch = tolower(ch);
            }
            wordptr[word_len] = ch;
            word_len++;
            if(word_len >= size){
                char *tmp = realloc(wordptr, size * 2);
                if (!tmp){
                    perror("realloc failed in read_words");
                    exit(EXIT_SUCCESS);
                }
                wordptr = tmp;
                size *= 2;
            }
        }/*
        else if(ch == '\b' && word_len > 0){
            word_len--;
        }*/
    }
    
    free(wordptr);
    return ht;
}

HashTable *open_file(const char *filename, HashTable *ht){
    FILE *fp;

    fp = fopen(filename, "r");

    if(!fp){
        printf("%s: No such file or directory\n", filename);
    }
    else{
        ht = read_words(fp, ht);
        fclose(fp);
    } 

    return ht;
}

int check_arg(char *cmd_args, int argc, char **argv, int i){
    int new_max;
    char *usage_msg = "usage: fw [-n num] [file1 [ file 2 ] ]";
    /* if cmd not equal to "-n" */
    if(strcmp(cmd_args, "-n")){
        printf("%s\n", usage_msg);
        exit(EXIT_SUCCESS);
    }
    /*arg eqauls "-n" */
    else{
        if(i + 1 < argc){
            int k = 0;
            bool numeric = true;
            i++;
            cmd_args = argv[i];
            while(k<strlen(cmd_args)){
                if(isalpha(cmd_args[k])){
                    numeric = false;
                    break;
                }
                k++;
            }
            if(numeric){
                new_max = atoi(cmd_args);
            }
            else{
                printf("%s\n", usage_msg);
                exit(EXIT_SUCCESS);
            }
        }
        /*if no arg after -n*/
        else{
            printf("%s\n", usage_msg);
            exit(EXIT_SUCCESS);
        }
    }
    return new_max;
}


int main(int argc, char **argv){

    HashTable *ht;
    char *cmd_args;
    int max_num = 10;
    int i = 1;
    int k;
    /*initialize hashtable*/
    ht = create_ht(20);
    
    /*if given args*/
    if(argc > 1){
        for (; i < argc; i++){
            cmd_args = argv[i];
            /*check if -n arg*/
            if(cmd_args[0] == '-'){
                max_num = check_arg(cmd_args, argc, argv, i);
                i++;
            }
            /* try to open files and read them*/
            else{
                ht = open_file(argv[i], ht);
            }
        }
    }
    /*if no args, get words from stdin*/
    else{
        ht = read_from_stdin(ht);
    }
    
    printf("The top %d words (out of %ld) are:\n", max_num, ht->items);
    
    quickSort(ht, 0, ht->size - 1);
    k=0;
    while(k<max_num){
        if (k == ht->items){
            break;
        }
        printf("%9ld %s\n", ht->array[k].appearances, ht->array[k].word);
        k++;
    }
    for(k = 0; k < ht->items; k++){
        if (ht->array[k].word)
            free(ht->array[k].word);
    }
    

    free(ht->array);
    free(ht);

    return 0;
}
