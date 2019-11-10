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

#define DEBUG false

bool val_head = true;
int P_FLG = 0;

char *octal_2str(int octal, int length){
    char *str;
    str = calloc(length, 1);
    sprintf(str, "%o", octal);

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
    octal = octal_2str(file.st_mode, 8);
    return octal;
}

char *get_uid(struct stat file){
    char *octal;
    octal = octal_2str(file.st_uid, 8);
    return octal;
}

char *get_guid(struct stat file){
    char *octal;
    octal = octal_2str(file.st_uid, 8);
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

void get_chksum(){}

char *get_typeflag(struct stat file){
    mode_t mode = file.st_mode;
    char *type;
    type = malloc(1);
    if(S_ISREG(mode))
        type[0] = '0';
    else if(S_ISDIR(mode))
        type[0] = '5';
    else
        type[0] = '2';
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
    char *octal;
    dev_t dev = file.st_dev;

    octal = malloc(8);
    maj = major(dev);
    octal = octal_2str(maj, 8);
    return octal;
}

char *get_devminor(struct stat file){
    unsigned int min;
    char *octal;
    dev_t dev = file.st_dev;

    octal = malloc(8);
    min = minor(dev);
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

    val_head = true;
    for(;i<1;i++){
        if(strlen(path) <= 100){
            head.name = get_name(name, path);
            head.prefix = NULL;
        }
        else{
            head.prefix = get_prefix(path);
            tmp = get_name(path,name);
            if(!tmp){
                val_head = false;
                break;
            }else{
                head.name = tmp;
            }
        }
        head.mode = get_mode(file);
        head.uname = get_uname(file);
        head.uid = get_uid(file);
        head.gid = get_guid(file);
        head.size = get_size(file);
        head.mtime = get_mtime(file);
        head.version = get_version();
        head.typeflag = get_typeflag(file);
        if(S_ISLNK(file.st_mode)){
            head.linkname = get_linkname(name);
        }else{
            head.linkname = NULL;
        }
        head.devmajor = get_devmajor(file);
        head.devminor = get_devminor(file);
    }
    if(DEBUG)
        printf("path: %s\n", head.name);
    return head;
}
void write_to_file(struct header head, struct stat file, char *name){
    /* write to argv[2] but for now use test.tar*/
    char *cont;
    size_t size = file.st_size;
    int fd, od;

    cont = malloc(size);
    fd = open("test.ta", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, head, 512);
    od = open(name, O_RDONLY);
    read(od, cont, size);
    write(fd, cont, size);
}
void traverse_dir(char *dir_name, char *path){

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
            write_to_file(head, curr, name);
            break;
        }
        /*dont recurse if "." or ".."*/
        if((S_ISDIR(curr.st_mode) && strcmp(name, ".")) &&
                (S_ISDIR(curr.st_mode) && strcmp(name, ".."))){
            dir_name = name;
            new_path = strdup(path);
            new_path = strcat(new_path, name);
            new_path = strcat(new_path,"/");
            traverse_dir(dir_name, new_path);

            /*go back to where u came from*/
            chdir("..");
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]){
    char *path =NULL;
    char *cd = NULL;
    path = calloc(256, 1);

    traverse_dir(cd, path);

    return 0;
}
