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
#define PATH_MAX 200
#define BUFF 8
char *parentpath;
bool DEBUG = false;

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
        for(n=0; n<strlen(str); n++)
            dup[n] = str[n];
    }
    dup[n] = '\0';
    return dup;
}

char *cat(const char *first, const char *two){

    int len1,len2, i;
    char *cat;
    if(two!=NULL){
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
    }
    else{
        len1 = strlen(first) + 1;
        cat = malloc(len1);
        memcpy(cat, first, len1);
    }
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
/*
char *getpwd(char *path, char *pwd){

    struct stat *current=NULL, *parent=NULL;
    int valid;
    char *tmp, *name, *root = "/\0";
    DIR *cd;
    struct dirent *de;
    if(DEBUG)
        printf("%s\n", pwd);
    current = malloc(sizeof(struct stat));
    parent = malloc(sizeof(struct stat));
    valid = stat(path, current);
    if(valid < 0){
        perror("stat()");
        exit(1);
    }
    path = strcat(path, "../\0");
    valid = stat(path, parent);
    if(valid < 0){
        perror(path);
        exit(1);
    }
    if(parent->st_dev == current->st_dev){
        cd = opendir(path);
        while((de=readdir(cd)) != NULL){
            name = de->d_name;
            tmp = strdup(path);
            tmp = strcat(tmp, name);
            valid = stat(tmp, parent);
            if (valid < 0){
                perror("stat()");
                exit(1);
            }
            if(parent->st_ino == current->st_ino && !strcmp(name, "."))
                break;
            if(parent->st_ino == current->st_ino){
                free(parent);
                free(current);
                pwd = getpwd(path, NULL);
                pwd = strcat(pwd, root);
                pwd = strcat(pwd, name);
                closedir(cd);
                return pwd;
            }
            free(tmp);
        }
    }
    free(tmp);
    free(parentpath);
    pwd = cat(pwd, root);
    return pwd;
}
*/

char *again(char *path, char *pwd){

    struct stat *current=NULL, *parent=NULL;
    int valid, len;
    char *tmp, *name, *root = "/\0";
    DIR *cd;
    struct dirent *de;

    current = calloc(1, sizeof(struct stat));
    parent = calloc(1, sizeof(struct stat));
    tmp = malloc(200);
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
            printf("%s\n", tmp);
            valid = stat(tmp, parent);
            if(parent->st_ino == current->st_ino && !strcmp(name, "."))
                break;
            if(parent->st_ino == current->st_ino && parent->st_ino == current->st_ino){
                pwd = again(path, pwd);
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
    pwd = again(cd, pwd);
    printf("%s\n", pwd);
    return 0;
}
