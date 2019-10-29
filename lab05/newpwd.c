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

#define BUFF 8

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
    cat = malloc(len1 + len2 + 2);
    for(i=0; i< len1; i++)
        cat[i]=first[i];
    cat[len1] = '/';
    for(i=0;i < len2; i++)
        cat[i+len1+1] = two[i];
    cat[len1+len2] = '\0';
    return cat;

}


char *redo(const char *traversal, const char *path){

    struct stat *current, *parent;
    int valid;
    char *parentpath, *tmp, *name, *root = "/\0";
    DIR *cd;
    struct dirent *de;

    current = malloc(sizeof(struct stat));
    parent = malloc(sizeof(struct stat));

    valid = stat(traversal, current);
    tmp = strdup(traversal);
    parentpath = strcat(tmp, "../\0");
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
            return;
        }

    }
    printf("path: %s\n", path);
    tmp = cat(root, path);
    return tmp;
}


int main(){

    char *cd = "./\0";
    char *path;

    path = redo(cd, NULL);
    printf("%s\n", path);
    return 0;
}
