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
#define BLOCK 512
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
    if(head.typeflag == '5')
        perms[0] = 'd';
    else if(head.typeflag == '2')
        perms[0] = 'l';
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

int get_size(struct header head){
    int size;

    size= str_2oct(head.size);
    return size;
}

void read_v_headers(int fd){
    struct header hd;
    char *perms, *owner, *mtime, *filename;
    bool done = false;
    int blocks, empty=0, size;
    while(!done){
        /*get tar header into struct header*/
        owner = calloc(17, 1);
        read(fd, &hd, sizeof(struct header));
        lseek(fd, 12, SEEK_CUR);
        if(hd.name[0]){
            empty = 0;
            perms = get_perms(hd);
            size = get_size(hd);
            owner = strcpy(owner, hd.uname);
            owner = strcat(owner, "/");
            owner = strcat(owner, hd.gname);
            printf("owner: %s\n", owner);
            if(perms[0] != 'd' && perms[0] != 'l'){
                blocks = size / BLOCK;
                lseek(fd, (blocks+1)*BLOCK, SEEK_CUR);
            }
        }
        else{
            if(++empty == 2)
                done = true;
        }
        free(owner);
    }
}

void read_header(int fd){
    struct header hd;
    bool done = false;



}

int main(int argc, char *argv[]){

    int fd;

    /*FIXME*/
    fd = open(argv[1], O_RDONLY, 0644);
    read_v_headers(fd);
    return 0;

}
