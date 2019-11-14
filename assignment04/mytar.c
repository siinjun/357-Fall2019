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

#define DEBUG false
#define OCTAL_LIMIT 2097151

bool val_head = true;
int P_FLG = 0;

char *octal_2str(int octal, int length){
    char *str, *padded;
    int zero;

    str = calloc(length, 1);
    sprintf(str, "%o", octal);
    if(!str[8] && !str[7]){
        padded = calloc(length,1);
        zero = length - strlen(str) - 1;
        memset(padded, 48, zero);
        padded = strcat(padded, str);
        return padded;
    }
    return str;
}
char *get_name(char *name, char *prefix){
    /*will redo this one later*/
    char *path;
    int len;
    path = calloc(100, 1);
    memset(path, 0, 100);
    /*increase head of path by 155 chars, if get_prefix func was used*/
    if(P_FLG){
        prefix += 155;
        P_FLG = 0;
    }
    if((len = strlen(prefix))){
        memcpy(path, prefix,len);
        len += strlen(name);
        /*if invalid*/
        if(len > 100){
            val_head = false;
            free(path);
            return NULL;
        }
        strcat(path, name);
    }
    else{
        len = strlen(prefix);
        memcpy(path, prefix, len);

        strcat(path, name);
    }
    return path;
}

char *get_mode(struct stat file){
    char *octal;
    int mode;
    mode = file.st_mode & 0777;
    octal = octal_2str(mode, 8);
    return octal;
}

char *get_uid(struct stat file){
    char *octal;
    uid_t uid = file.st_uid;
    if(uid < OCTAL_LIMIT){
        octal = octal_2str(uid, 8);
    }else{
        int valid;
        octal = calloc(8,1);
        valid = insert_special_int(octal, 8, uid);
        if(valid){
            perror("Bad uid");
        }
    }
    return octal;
}

char *get_guid(struct stat file){
    char *octal;
    gid_t gid= file.st_gid;
    if(gid < OCTAL_LIMIT){
        octal = octal_2str(gid, 8);
    }else{
        int valid;
        octal = calloc(8,1);
        valid = insert_special_int(octal, 8, gid);
        if(valid){
            perror("Bad gid");
        }
    }
    return octal;
}

char *get_size(struct stat file){
    char *octal;
    octal = octal_2str(file.st_size, 12);
    return octal;
}

char *get_mtime(struct stat file){
    char *octal;
    octal = octal_2str(file.st_mtime, 12);
    return octal;
}

char *get_chksum(struct header header){
    char *hp;
    char *octal, space = ' ';
    /*initialized w 8 bc chksum assumed to be all spaces*/
    int i,chars = 8 * (int)space;

    hp = (char*)&header;
    for(i=0; i < sizeof(struct header); i++){
        if(hp[i] != '\0'){
            chars += (unsigned char)hp[i];
        }
    }
    octal = octal_2str(chars, 8);
    return octal;
}

char get_typeflag(struct stat file){
    mode_t mode = file.st_mode;
    char type;
    if(S_ISREG(mode))
        type = '0';
    else if(S_ISDIR(mode))
        type = '5';
    else
        type = '2';
    return type;
}

char *get_linkname(char *name){
    /* may be wrong*/
    char *path;
    int valid;
    path = calloc(105, 1);
    valid = readlink(name, path,100);
    if (valid == -1){
        perror("readlink");
        exit(1);
    }
    if(strlen(path) > 100){
        val_head = false;
        return NULL;
    }
    return path;
}

char  *get_magic(){
    char *magic;
    magic = malloc(6);

    magic[0] = 'u';
    magic[1] = 's';
    magic[2] = 't';
    magic[3] = 'a';
    magic[4] = 'r';
    magic[5] = '\0';

    return magic;

}

char *get_version(){
    char *version;
    version = malloc(2);

    version[0] = '0';
    version[1] = '0';
    return version;
}

char *get_uname(struct stat file){
    struct passwd *pw;
    char *uname;

    pw = malloc(sizeof(struct passwd));
    pw = getpwuid(file.st_uid);
    uname = malloc(32);
    memcpy(uname, pw->pw_name, 32);
    return uname;
}

char *get_gname(struct stat file){
    struct group *gp;
    char *gname;

    gp = malloc(sizeof(struct group));
    gp = getgrgid(file.st_gid);
    gname = malloc(32);
    memcpy(gname, gp->gr_name, 32);
    return gname;
}

char *get_devmajor(struct stat file){
    unsigned int maj;
    char *octal=NULL;

    dev_t dev = file.st_rdev;
    octal = calloc(1,8);
    maj = major(dev);
    if(maj)
        octal = octal_2str(maj, 8);
    return octal;
}

char *get_devminor(struct stat file){
    unsigned int min;
    char *octal=NULL;

    dev_t dev = file.st_rdev;
    octal = calloc(1,8);
    min = minor(dev);
    if(min)
        octal = octal_2str(min, 8);
    return octal;
}

char *get_prefix(char *path){
    char *prefix;
    int len;
    prefix = calloc(155, 1);
    len = strlen(path);
    memcpy(prefix, path, len);
    P_FLG = 1;
    return prefix;
}

struct header create_header(struct stat file, char *name, char *path){
    struct header head;
    char *tmp;
    int i = 0;
    /*initialize struct to 0*/
    memset(&head, 0, sizeof(struct header));
    /*initialize that file may be used for valid header*/
    val_head = true;
    for(;i<1;i++){
        if(strlen(path) <= 100){
            strcpy(head.name, get_name(name, path));
        }
        else{
            strcpy(head.prefix, get_prefix(path));
            tmp = get_name(path,name);
            if(!tmp){
                val_head = false;
                break;
            }else{
                strcpy(head.name, tmp);
            }
        }
        strcpy(head.mode, get_mode(file));
        memcpy(head.uid, get_uid(file), 8);
        memcpy(head.gid, get_guid(file), 8);
        if(S_ISREG(file.st_mode)){
            strcpy(head.size, get_size(file));
        }
        strcpy(head.mtime, get_mtime(file));
        if(S_ISLNK(file.st_mode)){
            strcpy(head.linkname, get_linkname(name));
        }
        strcpy(head.magic, get_magic());
        strcpy(head.version, get_version());
        strcpy(head.uname, get_uname(file));
        strcpy(head.gname, get_gname(file));
        head.typeflag = get_typeflag(file);
        strcpy(head.devmajor, get_devmajor(file));
        strcpy(head.devminor, get_devminor(file));
        strcpy(head.chksum, get_chksum(head));
    }
    if(DEBUG){
        printf("path/name: %s\n", head.name);
        printf("mode: %s\n", head.mode);
        printf("uid: %s\n", head.uid);
        printf("gid: %s\n", head.gid);
        printf("size: %s\n", head.size);
        printf("mtime: %s\n", head.mtime);
        printf("chcksum %s\n", head.chksum);
        printf("typeflag: %c\n", head.typeflag);
        printf("linkename: %s\n", head.linkname);
        printf("magic: %s\n", head.magic);
        printf("version: %s\n", head.version);
        printf("uname: %s\n", head.uname);
        printf("gname: %s\n", head.gname);
        printf("devmajor: %s\n", head.devmajor);
        printf("devminor: %s\n", head.devminor);

    }
    return head;
}


void write_to_file(struct header head, struct stat file,
                    char *name, int fd, int bd){
    /* write to argv[2] but for now use test.tar*/
    char *cont;
    size_t size = file.st_size;
    int od, leftover;

    fchdir(bd);
    cont = malloc(size);
    write(fd, &head, sizeof(head));
    lseek(fd, 12, SEEK_CUR);
    /*if reg file*/
    if(!S_ISDIR(file.st_mode) && !S_ISLNK(file.st_mode)){
        /*get contents of file*/
        od = open(name, O_RDONLY);
        read(od, cont, size);
        /*write contents of file to new file*/
        write(fd, cont, size);

        leftover = size % 512;
        leftover = 512 - leftover;
        lseek(fd, leftover, SEEK_CUR);
    }
}

void end_padding(int fd){
    /*pad ending with two blocks of 512 Bytes*/
    char *zero;
    zero = calloc(1024, 1);

    write(fd, zero, 1024);
}

void traverse_dir(char *dir_name, char *path, int fd, int begin_dir){

    struct dirent *de;
    struct stat curr;
    struct header head;
    DIR *dir;
    char *name, *new_path;
    int valid;

    de = malloc(sizeof(struct dirent));
    if(strlen(path)){
        valid = chdir(dir_name);
        if(valid == -1){
            perror("chdir");
            exit(1);
        }
    }
    dir = opendir(".");
    while((de = readdir(dir)) != NULL){
        name = de->d_name;
        valid = lstat(name, &curr);
        if(valid < 0){
            perror("lstat");
            exit(1);
        }
        head = create_header(curr, name, path);
        /*check if valid header*/
        if(val_head){
            int cur_dir;
            cur_dir = open(".", O_RDONLY);
            write_to_file(head, curr, name, fd, begin_dir);
            fchdir(cur_dir);
            close(cur_dir);
        }
        /*dont recurse if "." or ".."*/
        if((S_ISDIR(curr.st_mode) && strcmp(name, ".")) &&
                (S_ISDIR(curr.st_mode) && strcmp(name, ".."))){
            dir_name = name;
            new_path = strdup(path);
            new_path = strcat(new_path, name);
            new_path = strcat(new_path,"/");
            traverse_dir(dir_name, new_path, fd, begin_dir);

            /*go back to where u came from*/
            chdir("..");
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]){
    char *path =NULL;
    char *cd = NULL;
    int fd, begin_dir;

    begin_dir = open(".", O_RDONLY);
    fd = open("test.tar", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    path = calloc(256, 1);

    traverse_dir(cd, path, fd, begin_dir);
    end_padding(fd);
    return 0;
}
