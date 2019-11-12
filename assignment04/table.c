#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
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
#include "header.h"
#include "ustarformat.c"

int V_FLG = 0;
int str_2oct(char *octal){
    /*FIXME middle nulls*/
    int i = 0, num=0;

    while(octal[i]){
        if(octal[i]){
            switch(octal[i]){

                case '0':   num = num << 3;
                            break;
                case '1':   num = num << 3;
                            num |= 0x01;
                            break;
                case '2':   num = num << 3;
                            num |= 0x02;
                            break;
                case '3':   num = num << 3;
                            num |= 0x03;
                            break;
                case '4':   num = num << 3;
                            num |= 0x04;
                            break;
                case '5':   num = num << 3;
                            num |= 0x05;
                            break;
                case '6':    num = num << 3;
                            num |= 0x06;
                            break;
                case '7':   num = num << 3;
                            num |= 0x07;
                            break;
                default: break;
            }
        }
        i++;
    }
    return num;
}
char *get_perms(struct header head){
    char *perms, *octal;
    int mode;

    perms = malloc(10);
    memset(perms, 45, 10);
    octal = head.mode;
    mode = str_2oct(octal);
    printf("perms b4: %s\n", perms);
    if(S_ISDIR(mode))
        perms[0] = 'd';
    if(S_IRUSR & mode)
        perms[1] = 'r';
    if(S_IWUSR & mode)
        perms[2] = 'w';
    if(S_IXUSR & mode)
        perms[3] = 'x';
    if(S_IRGRP & mode)
        perms[4] = 'r';
    if(S_IWGRP & mode)
        perms[5] = 'w';
    if(S_IXGRP & mode)
        perms[6] = 'x';
    if(S_IROTH & mode)
        perms[7] = 'r';
    if(S_IWOTH & mode)
        perms[8] = 'w';
    if(S_IXOTH & mode)
        perms[9] = 'x';
    printf("perms a4: %s\n", perms);
    return perms;
}

void read_tar(int fd){
    struct header hd;
    char *perms;
    /*get tar header into struct header*/
    read(fd, &hd, sizeof(struct header));

    perms = get_perms(hd);
}
int main(int argc, char *argv[]){

    int fd;

    /*FIXME*/
    fd = open(argv[1], O_RDONLY, 0644);
    read_tar(fd);
    return 0;

}
