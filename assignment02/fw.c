/*  
    Name:   Sinjun Jang
    Prog:   fw
    Desc:   Read in files from stdin and output the most common
            appearing words to stdout
*/

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
    x++;

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

HashTable *open_file(const char *filename, HashTable *ht){
    FILE *fp;

    fp = fopen(filename, "r");

    if(!fp){
        printf("%s: no such file in directory\n", filename);
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
    int j;
    int k;
    int outputted = 0;
    int largest;
    int tmp_app;
    int mst_app;
    Index most_appeared;
    Index tmp;
    /*initialize hashtable*/
    ht = create_ht(50);
    
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
    }
    printf("The top %d words (out of %ld) are:\n", max_num, ht->items);
    while(outputted < max_num){

        if(outputted == ht->items){
            break;
        }
        most_appeared.word = 0x0;
        most_appeared.appearances = 0;
        for(j = 0; j < ht->size; j++){
            tmp = ht->array[j];
            tmp_app = tmp.appearances;
            mst_app = most_appeared.appearances;
            if (mst_app == 0 && tmp_app > 0){
                most_appeared = tmp;
                largest = j;
            }
            else if(mst_app < tmp_app){
                most_appeared = tmp;
                largest = j;
            }
            else if((mst_app > 0 && tmp_app > 0) && mst_app == tmp_app){
                int most = strlen(most_appeared.word);
                int curr = strlen(tmp.word);
                if (curr < most){
                    most_appeared = tmp;
                    largest = j;
                }
                else if (curr == most){
                    int cmp = strcmp(most_appeared.word, tmp.word);
                    if (cmp > 0){
                        most_appeared = tmp;
                        largest = j;
                    }
                }
            }
        }
        printf("%s %ld\n", most_appeared.word, most_appeared.appearances);
        free(ht->array[largest].word);
        ht->array[largest].word = 0x0;
        ht->array[largest].appearances = 0;
        outputted++;
    }
    
    for(k = 0; k < ht->size; k++){
        if (ht->array[k].word)
            free(ht->array[k].word);
    }
    

    free(ht->array);
    free(ht);
    printf("number of mallocs: %d\n", x);

    return 0;
}
