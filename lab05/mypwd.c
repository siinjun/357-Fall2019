#include<sys/types.h>
#include<limits.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#define BUFF 800
int count=0;

char **get_contents(DIR *dir){

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
    return contents;

}

void getpwd(const char *cd, const char *prev_d){

    /*stuck*/
    DIR *dir;
    char **contents=NULL;
    char *name;
    int i=0;

    /*reads all files in directory*/
    dir = opendir(cd);
    /*if dir is not a directory*/
    if(errno == ENOENT)
        perror(cd);
    else{
        contents = get_contents(dir);
        while(contents[i]){
            printf("%s\n", contents[i++]);
        }
        count += 1;
        if (count == 3)
            return;
        getpwd("..", ".");
    }
}

void find_parent(const char *parent, char **contents){
    /*returns name of child*/
    char *name;


}

int main(int argc, char *argv[]){

    char *currdir = ".";
    getpwd(currdir, NULL);
    return 0;
}
