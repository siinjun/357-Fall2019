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

#define DEBUG false

char *octal_2str(int octal, int length){
    char *str;
    str = calloc(1, length);
    sprintf(str, "%o", octal);

    return str;
}
char *get_name(char *name, char *prefix){

    if(strlen(prefix)){
        strcat(prefix,"/");
        strcat(prefix,name);
        return prefix;
    }else{
        return name;
    }
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

char *get_linkname(struct stat file, char *name){
    /* may be wrong*/
    if(S_ISLNK(file.st_mode)){
        char *path;
        int valid;
        path = calloc(100, 1);
        valid = readlink(name, path,100);
        if (valid == -1){
            perror("readlink");
            exit(1);
        }
        return path;
    }
    else{
        return NULL;
    }
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
    prefix = calloc(155, 1);

    prefix = strcpy(prefix, path);

    return prefix;
}

char *create_header(struct stat file, char *name, char *path){
    char *header;

    header = calloc(512, sizeof(char));
    get_name(name, path);
    get_mode(file);
    get_uname(file);
    get_uid(file);
    get_guid(file);
    get_size(file);
    get_mtime(file);

    get_typeflag(file);
    get_linkname(file,name);
    get_devmajor(file);
    get_devminor(file);
    get_prefix(path);
    return header;
}

void traverse_dir(char *path){

    struct dirent *de;
    struct stat curr;
    DIR *dir;
    char *name;
    int valid;

    de = malloc(sizeof(struct dirent));

    if(!strlen(path)){
        dir = opendir(".");
    } else{
        dir = opendir(path);
    }
    while((de = readdir(dir)) != NULL){
        name = de->d_name;
        valid = lstat(name, &curr);
        if(valid < 0){
            perror("lstat");
            exit(1);
        }
        create_header(curr, name, path);
    }
}

int main(int argc, char *argv[]){
    char *path =NULL;
    path = calloc(100, 1);

    traverse_dir(path);

    return 0;
}
