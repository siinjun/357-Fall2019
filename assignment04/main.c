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
#include "functions.c"
#include "table.c"
#include "mytar.c"
#define SE stderr

int main(int argc, char *argv[]){
    int i,v_flg=0,s_flg=0, command=0, fd;
    bool crt=false, tbl = false, xtrct = false, run=false;

    if(argc < 3){
        fprintf(SE, "Usage: mytar [ctvS]f tarfile [ path [ ... ] ]\n");
        exit(1);
    }
    else{
        for(i=0; i<strlen(argv[1]); i++){
            if(argv[1][i] == 'c'){
                crt = true;
                command++;
            }
            else if(argv[1][i] == 't'){
                tbl = true;
                command++;
            }
            else if(argv[1][i] == 'x'){
                xtrct = true;
                command++;
            }
            else if(argv[1][i] == 'v'){
                v_flg = 1;
            }
            else if(argv[1][i] == 'S' || argv[1][i] == 's'){
                s_flg = 1;
            }
            else if(argv[1][i] == 'f'){
                run = true;
            }
            else{
                fprintf(SE, "Unrecognized option '%c'.\n",argv[1][i]);
                run = false;;
            }
        }
        if(run){
            if(command == 0){
                fprintf(SE, "you must choose one of the 'ctx' options.\n");
                fprintf(SE, "Usage: mytar [ctvS]f tarfile");
                fprintf(SE, " [ path [ ... ] ]\n");
            }
            else if(command > 1){
                fprintf(SE, "you may only choose one of the 'ctx' options.\n");
                fprintf(SE, "Usage: mytar [ctvS]f tarfile [ path [ ... ] ]\n");
            }
            else if(run && command == 1){
                if(crt){
                    creation(v_flg, s_flg, argc, argv);
                }
                else if(tbl){
                    table(v_flg, argv);
                }
                else if(xtrct){
                    fd = open(argv[2], O_RDONLY);
                    extract(fd, argv[2]);
                }

            }
        }
        else{
            fprintf(SE, "Usage: mytar [ctvS]f tarfile [ path [ ... ] ]\n");
        }
    }
    return 0;
}
