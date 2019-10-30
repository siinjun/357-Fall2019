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
#define PATH_MAX 2048
bool DEBUG = false;



char *mypwd(char *path, char *pwd){

    struct stat *current=NULL, *parent=NULL;
    int valid, len;
    char *tmp, *name, *root = "/\0";
    DIR *cd;
    struct dirent *de;

    current = calloc(1, sizeof(struct stat));
    parent = calloc(1, sizeof(struct stat));
    tmp = calloc(PATH_MAX, sizeof(char));

    valid = stat(path, current);
    if(valid < 0){
        perror("stat()");
        exit(1);
    }
    path = strcat(path, "../\0");
    valid = stat(path, parent);
    if(valid < 0){
        perror("stat()");
        exit(1);
    }
    if(parent->st_dev == current->st_dev){
        cd = opendir(path);
        if(cd == NULL){
            perror("opendir()");
            exit(1);
        }
        while((de=readdir(cd)) != NULL){
            name = de->d_name;
            tmp = strcat(tmp, path);
            tmp = strcat(tmp, name);
            valid = stat(tmp, parent);
            if(parent->st_ino == current->st_ino &&
                    (!strcmp(name, "..") || !strcmp(name, ".")))
                break;
            if(parent->st_dev == current->st_dev &&
                    parent->st_ino == current->st_ino){
                pwd = mypwd(path, pwd);
                pwd = strcat(pwd, root);
                pwd = strcat(pwd, name);
                closedir(cd);
                free(tmp);
                free(current);
                free(parent);
                return pwd;
            }

            len = strlen(tmp);
            memset(tmp, 0, len);

        }
    }
    free(current);
    free(parent);
    free(tmp);
    closedir(cd);
    pwd = strcat(pwd, root);
    return pwd;
}
int main(){

    char *cd;
    char *pwd;
    pwd = calloc(PATH_MAX, sizeof(char));
    cd = calloc(PATH_MAX, sizeof(char));
    cd[0] = '.';
    cd[1] = '/';
    cd[2] = '\0';
    /*
    path = getpwd(cd, path);
    */
    pwd = mypwd(cd, pwd);
    printf("%s\n", pwd);
    free(cd);
    free(pwd);
    return 0;
}
