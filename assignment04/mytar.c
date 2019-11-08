#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdint.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
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
    printf("mode: %s\n", octal);
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
    printf("size: %s\n", octal);
    return octal;
}

void get_mtime(){}

void get_chksum(){}

void get_typeflag(){}

void get_linkname(){}

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

void get_devmajor(){}

void get_devminor(){}

void get_prefix(){}

char *create_header(struct stat file, char *name, char *path){
    char *header;

    header = calloc(512, sizeof(char));
    get_name(name, path);
    get_mode(file);
    get_uname(file);
    get_uid(file);
    get_guid(file);
    get_size(file);
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
