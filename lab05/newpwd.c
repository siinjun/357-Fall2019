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
#define PATH_MAX 2018
#define BUFF 8
char *parentpath;

char *strdup(const char *str)
{
    int n;
    char *dup = NULL;
    if(str)
        n = strlen(str) + 1;
    else
        n = 1;
    dup = malloc(n);
    if(dup && str){
        memcpy(dup, str, n);
    }
    return dup;
}

char *cat(const char *first, const char *two){

    int len1,len2, i;
    char *cat;
    len1 = strlen(first);
    len2 = strlen(two);/*
    printf("len1: %d, len2: %d, str1: %s, str2: %s\n",len1,len2, first, two);*/
    cat = malloc(len1 + len2 + 3);
    for(i=0; i< len1; i++)
        cat[i]=first[i];
    cat[len1] = '/';
    for(i=0;i < len2; i++)
        cat[i+len1+1] = two[i];
    cat[len1+len2+2] = '\0';
    return cat;

}


/*char *redo(const char *traversal, const char *path){

    struct stat *current, *parent;
    int valid;
    char *parentpath, *tmp, *name, *root = "/\0";
    DIR *cd;
    struct dirent *de;

    current = malloc(sizeof(struct stat));
    parent = malloc(sizeof(struct stat));

    valid = stat(traversal, current);
    if(valid<0){
        perror("stat()");
        exit(1);
    }
    tmp = strdup(traversal);
    parentpath = strcat(tmp, "../\0");
    valid = stat(parentpath, parent);
    if(valid<0){
        perror("stat()");
        exit(1);
    }
    if(parent->st_dev != current->st_dev)
        printf("diff devices\n");
    else{
        cd = opendir(parentpath);
        if(errno){
            perror("parentpath");
            exit(2);
        }
        while((de=readdir(cd)) != NULL){
            name = de->d_name;
            tmp = strdup(parentpath);
            strcat(tmp, name);
            valid = stat(tmp, parent);
            if(valid<0){
                perror("tmp");
                exit(1);
            }
            if(parent->st_ino == current->st_ino && !strcmp(name, "."))
                break;
            if(parent->st_ino == current->st_ino &&
                parent->st_dev == current->st_dev){
                if(!path){
                    int len;
                    len = strlen(name);
                    tmp = strdup(name);
                    tmp = realloc(tmp, len+1);
                    tmp[len] = '/';
                    tmp[len+1] = '\0';
                    redo(parentpath, tmp);
                }
                else{
                    tmp = strdup(path);
                    tmp = cat(name, tmp);
                    redo(parentpath, tmp);
                }
                closedir(cd);
                return;
            }

        }
    }
    printf("path: %s\n", path);
    tmp = cat(root, path);
    return tmp;
}*/

char *getpwd(const char *path, char *pwd){

    struct stat *current, *parent;
    int valid;
    char *tmp, *name, *root = "/\0";
    DIR *cd;
    struct dirent *de;

    current = malloc(sizeof(struct stat));
    parent = malloc(sizeof(struct stat));

    printf("%s\n", pwd);
    valid = stat(path, current);
    if(valid < 0){
        perror("stat()");
        exit(1);
    }
    tmp = strdup(path);
    parentpath = strcat(tmp, "../\0");
    valid = stat(parentpath, parent);
    if(valid < 0){
        perror("stat()");
        exit(1);
    }
    if(parent->st_dev == current->st_dev){
        cd = opendir(parentpath);
        while((de=readdir(cd)) != NULL){
            name = de->d_name;
            tmp = strdup(parentpath);
            tmp = strcat(tmp, name);
            valid = stat(tmp, parent);
            if (valid < 0){
                perror("stat()");
                exit(1);
            }
            if(parent->st_ino == current->st_ino && !strcmp(name, "."))
                break;
            if(parent->st_ino == current->st_ino){
                if(pwd[0] == '\0')
                    pwd = name;
                else
                    pwd = cat(name, pwd);
                name = getpwd(parentpath, pwd);
                return name;
            }
        }
    }
    pwd = cat(root, pwd);
    return pwd;
}

int main(){

    char *cd = "./\0";
    char *path;
    DIR *wot;

    path = calloc(PATH_MAX, sizeof(char));
    parentpath = calloc(PATH_MAX, sizeof(char));
    /*path = redo(cd, NULL);*/
    getpwd(cd, path);
    return 0;
}
