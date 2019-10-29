#include<sys/types.h>
#include<limits.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<errno.h>

#define BUFF 40

int contents_child=0;
int contents_par= 0;

char **get_contents(DIR *dir){
    /*gets contents of curr dir*/
    char **contents;
    int buff = BUFF,i=0, count=0;
    struct dirent *de;

    contents = calloc(1, buff);
    while((de=readdir(dir)) != 0){
        count+=8;
        if(count >= buff){
            contents = realloc(contents, buff * 2);
            if(!contents){
                perror("Realloc");
                exit(1);
            }
            buff *= 2;
        }
        contents[i++] = de->d_name;
    }
    contents_child = i;
    return contents;

}
char **get_par_contents(DIR *dir){
    /*gets contents of parent dir and its sub dirs*/
    char **contents;
    int buff = BUFF,i=0, count=0;
    struct dirent *de;

    contents = calloc(1, buff);
    while((de=readdir(dir)) != 0){
        count+=8;
        if(count >= buff){
            contents = realloc(contents, buff * 2);
            if(!contents){
                perror("Realloc");
                exit(1);
            }
            buff *= 2;
        }
        contents[i++] = de->d_name;
    }
    contents_par = i;
    return contents;

}

bool compare_contents(char**one, char **two){

    int i =0;
    if(contents_par != contents_child){
        return false;
    }
    else{
        for(;i < contents_par;i++){
            if(strcmp(one[i], two[i]))
                return false;
        }
        return true;
    }
}

void find_parent(const char *parent, char **contents){
    /*returns name of child, or NULL if at root*/
    char *name;
    char **new_con=NULL;

}

void getpwd(const char *cd, const char *prev_d){

    DIR *dir;
    char **contents=NULL;
    char *path_to_root=NULL;
    char *prev = "../\0";
    int i=0, buff = 8;

    path_to_root = malloc(buff);
    if(!path_to_root){
        perror("malloc");
        exit(1);
    }
    /*reads all files in directory*/
    dir = opendir(cd);
    /*if dir is not a directory*/
    if(errno == ENOENT)
        perror(cd);
    else{
        contents = get_contents(dir);
        i += 3;
        if(i>=buff){
            path_to_root = realloc(path_to_root, buff*2);
            if(!path_to_root){
                perror("malloc");
                exit(1);
            }
        }
        path_to_root = strcat(path_to_root, prev);
        find_parent(path_to_root, contents);
    }
}


int main(int argc, char *argv[]){

    char *currdir = ".";
    getpwd(currdir, NULL);
    return 0;
}
