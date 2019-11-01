#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<dirent.h>
#include<errno.h>
#define path_max 2048
bool DEBUG = false;


char * mypwd(char *pwd){

    struct stat *current=NULL, *parent=NULL;
    int valid;
    char *name, *root = "/\0";
    DIR *cd;
    struct dirent *de;

    current = calloc(1, sizeof(struct stat));
    parent = calloc(1, sizeof(struct stat));

    valid = lstat(".", current);
    if(valid < 0){
        perror("stat()");
        exit(1);
    }
    valid = lstat("..", parent);
    if(valid < 0){
        perror("stat()");
        exit(1);
    }
    if(parent->st_dev == current->st_dev){
        valid = chdir("..");
        if (valid < 0){
            perror("chdir()");
            exit(1);
        }
        cd = opendir(".");
        while((de=readdir(cd)) != NULL){
            name = de->d_name;
            valid = lstat(name, parent);
            if(valid < 0){
                perror("stat()");
                exit(1);
            }
            if(parent->st_dev == current->st_dev){
                if(parent->st_ino == current->st_ino &&
                        (!strcmp(name, "..") || !strcmp(name, ".")))
                    break;
                if(parent->st_ino == current->st_ino){
                    pwd = mypwd(pwd);
                    pwd = strcat(pwd,root);
                    pwd = strcat(pwd,name);
                    closedir(cd);
                    free(current);
                    free(parent);
                    return pwd;
                }
            }
        }
    }
    free(current);
    free(parent);
    if(parent->st_dev == current->st_dev)
        closedir(cd);
    return pwd;
}
int main(){

    char *pwd;
    pwd = calloc(path_max, sizeof(char));
    pwd = mypwd(pwd);
    printf("%s\n", pwd);
    free(pwd);
    return 0;
}
