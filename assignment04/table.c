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
#define BLOCK 512

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
                case '\0':  break;

                default:    fprintf(stderr, "Malformed header. Quiting.");
                            exit(EXIT_FAILURE);
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
    return perms;
}

int get_size_of_file(struct header head){
    int size;

    size= str_2oct(head.size);
    return size;
}

char *get_name(struct header hd){

    char *name;

    name = calloc(257,1);

    if(hd.prefix[0]){
        name = strncpy(name, hd.prefix, 155);
        name = strncat(name, "/", 1);
        name = strncat(name, hd.name,100);
    }
    else{
        name = strncpy(name, hd.name,100);
    }

    return name;
}

char *get_time(struct header head){

    char *mtime;
    long int secs;
    struct tm *time;

    mtime = calloc(17,1);
    secs = (long)str_2oct(head.mtime);
    time = localtime(&secs);

    strftime(mtime, 17, "%Y-%m-%d %H:%M", time);
    return mtime;
}

char *create_verbose(char *perms,char *filename,int size,struct header head){
    char *bytes, *verbose, *owner, *space;

    int buf, pad, owner_len;

    bytes = calloc(8,1);
    sprintf(bytes, "%u", size);
    /*len of owner*/
    owner_len = strlen(head.uname) + strlen(head.gname) + 2;

    /*buf is for the total size of verbose that's allowed*/
    buf = 256 + 10 + owner_len + 8 + 16 + 4;

    /*the padding between owner and size*/
    pad = 8 - strlen(bytes) + 1;
    space = calloc(pad,1);
    memset(space,32, pad-1);

    /*find out length of owner*/
    owner = calloc(owner_len, 1);
    owner = strcpy(owner, head.uname);
    owner = strcat(owner, "/");
    owner = strcat(owner, head.gname);

    verbose = calloc(buf, 1);

    verbose = strncpy(verbose, perms, 10);

    verbose = strncat(verbose, " ", 1);

    verbose = strncat(verbose, owner, owner_len);

    verbose = strncat(verbose, space,pad);

    verbose = strncat(verbose, " ", 1);

    verbose = strncat(verbose, bytes, 8);

    verbose = strncat(verbose, " ", 1);

    verbose = strncat(verbose, get_time(head), 16);

    verbose = strncat(verbose, " ", 1);

    verbose = strcat(verbose, filename);

    return verbose;
}

bool checkheader(struct header head){

    if(strncmp(head.magic, "\0\0\0\0\0\0", 6) == 0)
        return true;
    if(strncmp(head.magic, "ustar\0", 6) && strncmp(head.magic, "ustar ", 6)){
        return false;
    }
    return true;
}

void traverse(char *dir_name, int fd, int v_flg){

    struct header hd;
    char *perms, *filename, *verbose;
    bool inside = true, valid = false, good_header = true;
    int n = strlen(dir_name), blocks, size;
    while(inside){
        valid = false;
        read(fd, &hd, sizeof(struct header));
        lseek(fd, 12, SEEK_CUR);
        if(hd.name[0] || hd.prefix[0]){
            filename = get_name(hd);
            if(strncmp(dir_name, filename,n)==0){
                valid = true;
            }
            if(valid){
                perms = get_perms(hd);
                size = get_size_of_file(hd);
                filename = get_name(hd);
                if(v_flg){
                    verbose = create_verbose(perms,filename,size,hd);
                    printf("%s\n", verbose);
                    fflush(stdout);
                }
                if(!v_flg){
                    printf("%s\n", filename);
                }
                if(perms[0] != 'd' && perms[0] != 'l'){
                    blocks = size / BLOCK;
                    lseek(fd, (blocks+1)*BLOCK, SEEK_CUR);
                }
            }
            else{
                inside = false;
            }

        }
        else{
            inside = false;
        }
    }

}

void read_all_headers(int fd, int v_flg){
    struct header hd;
    char *perms, *filename, *verbose;
    bool done = false, good_header = true;
    int blocks, empty=0, size;

    while(!done){
        /*get tar header into struct header*/
        read(fd, &hd, sizeof(struct header));
        lseek(fd, 12, SEEK_CUR);
        good_header = checkheader(hd);
        /*
        if(!good_header){
            fprintf(stderr, "Malformed header. Quiting.");
            exit(2);
        }*/
        if(hd.name[0] || hd.prefix[0]){
            empty = 0;
            perms = get_perms(hd);
            size = get_size_of_file(hd);
            filename = get_name(hd);
            if(v_flg){
                verbose = create_verbose(perms,filename,size,hd);
                printf("%s\n", verbose);
                fflush(stdout);
            }
            if(!v_flg){
                printf("%s\n", filename);
            }
            if(perms[0] != 'd' && perms[0] != 'l'){
                blocks = size / BLOCK;
                lseek(fd, (blocks+1)*BLOCK, SEEK_CUR);
            }
            free(perms);
        }
        else{
            if(++empty == 2)
                done = true;
        }
    }
}

void read_specific_files(int fd, int v_flg, int argc, char *argv[]){
    struct header hd;
    char *perms, *filename, *verbose;
    bool done = false, valid = false, subdir = false, good_header = true;
    int blocks, empty=0, size, i;

    while(!done){
        valid =false;
        /*if traversed a subdir then go back one header block*/
        if(subdir){
            subdir = false;
            lseek(fd, -512, SEEK_CUR);
        }
        /*get tar header into struct header*/
        read(fd, &hd, sizeof(struct header));
        lseek(fd, 12, SEEK_CUR);
        if(hd.name[0] || hd.prefix[0]){
            filename = get_name(hd);
            for(i=3; i < argc; i++){
                if(strcmp(argv[i], filename)==0){
                    valid = true;
                    break;
                }
            }
            if(valid){
                empty = 0;
                perms = get_perms(hd);
                size = get_size_of_file(hd);
                filename = get_name(hd);
                if(v_flg){
                    verbose = create_verbose(perms,filename,size,hd);
                    printf("%s\n", verbose);
                    fflush(stdout);
                }
                if(!v_flg){
                    printf("%s\n", filename);
                }
                if(perms[0] != 'd' && perms[0] != 'l'){
                    blocks = size / BLOCK;
                    lseek(fd, (blocks+1)*BLOCK, SEEK_CUR);
                }
                if(hd.typeflag == '5'){
                    /*traverse all files in dir*/
                    traverse(filename, fd, v_flg);
                    subdir = true;
                }
                free(perms);
            }
        }
        else{
            if(++empty == 2)
                done = true;
        }
    }
}

void table(int v_flg, int argc, char *argv[]){
    int fd;

    fd = open(argv[2], O_RDONLY, 0644);
    if(fd==-1){
        perror(argv[2]);
        exit(2);
    }
    else if(argc > 3){
        read_specific_files(fd, v_flg, argc, argv);
    } 
    else{
        read_all_headers(fd,v_flg);
    }
}
