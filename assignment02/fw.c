/*  
    Name:   Sinjun Jang
    Prog:   fw
    Desc:   Read in files from stdin and output the most common
            appearing words to stdout
*/

#include "hashtable.c"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>

HashTable *read_words(FILE *fp, HashTable *ht){

    
    return ht;


}

HashTable *open_file(const char *filename, HashTable *ht){
    FILE *fp;

    fp = fopen(filename, "r");

    if(!fp){
        printf("%s: no such file in directory\n", filename);
    }
    else{

    {

    return ht;
}


int main(int argc, char **argv){

    HashTable *ht;
    char *cmd_args;
    char *usage_msg = "usage: fw [-n num] [file1 [ file 2 ] ]";
    int max_num = 10;
    int i = 1;

    ht = create_ht(30);
    /*if given args*/
    if(argc > 1){
        for (; i < argc; i++){
            cmd_args = argv[i];
            /*check if -n arg*/
            if(cmd_args[0] == '-'){
                /* if cmd not equal to "-n" */
                if(strcmp(cmd_args, "-n")){
                    printf("%s\n", usage_msg);
                    exit(EXIT_SUCCESS);
                }
                /*arg eqauls "-n" */
                else{
                    if(i + 1 < argc){
                        i++;
                        cmd_args = argv[i];
                        max_num = atoi(cmd_args);
                        /*if option after arg is not a number*/
                        if(!max_num){
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
            }
            /* try to open files and read them*/
            else{
                open_file(argv[i], ht);

            }
        }
    }
    /*if no args, get words from stdin*/
    else{
    }

    return 0;
}
