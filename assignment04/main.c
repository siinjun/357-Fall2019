#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdint.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/sysmacros.h>
#include<unistd.h>
#include<time.h>
#include<pwd.h>
#include<string.h>
#include<dirent.h>
#include<grp.h>
#include<errno.h>
#include "header.h"
#include "ustarformat.c"
#include "functions.c"
#include "table.c"
#include "mytar.c"

int main(int argc, char *argv){
    int i,v_flg=0,s_flg=0, command=0, fd;
    bool create=false, table = false, extract = false, run=true;;

    if(argc < 3){
        printf("Usage: mytar [ctvS]f tarfile [ path [ ... ] ]\n");
        exit(1);
    }
    else{
        for(i=0; i<strlen(argv[1]); i++){
            if(argv[1][i] = 'c'){
                create = true;
                command++;
            }
            else if(argv[1][i] = 't'){
                table = true;
                command++;
            }
            else if(argv[1][i] = 'x'){
                extract = true;
                command++;
            }
            else if(argv[1][i] = 'v'){
                v_flg = 1;
            }
            else if(argv[1][i] = 'S'){
                s_flg = 1;
            }
            else{
                printf("Unrecognized option '%c'.\n",argv[1][i]);
                run = false;;
            }
        }
        if(command == 0){
            printf("you must choose one of the 'ctx' options.\n");
            printf("Usage: mytar [ctvS]f tarfile [ path [ ... ] ]\n");
        }
        else if(command > 1){
            printf("you may only choose one of the 'ctx' options.\n");
            printf("Usage: mytar [ctvS]f tarfile [ path [ ... ] ]\n");
        }
        else if(run && command == 1){
            if(create){
                i = creation(v_flg, s_flg, argc, argv);
            }
            else if(table){
                i = table(v_flg, argv);
            }
            else{
                fd = open(argv[2], O_RDONLY);
                extract(fd, argv[2]);
            }

        }
    }
}
