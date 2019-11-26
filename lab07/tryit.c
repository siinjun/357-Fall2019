#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<errno.h>
int main(int argc, char *argv[]){
    pid_t child, pid;
    int status;
    struct stat curr;
    if(argc != 2){
        fprintf(stderr, "usage: tryit command\n");
        exit(1);
    }
    if((child = fork())){
        /*parent*/
        pid = getpid();
        if(-1 == wait(&status)){
            perror("wait");
            exit(1);
        }
        if((WIFEXITED(status) && WEXITSTATUS(status)) || WIFSIGNALED(status)){
            printf("Process %d exited with an error value.\n", pid);
            exit(1);
        } else{
            printf("Process %d succeeded.\n", pid);
            exit(0);
        }
    }
    /*child*/
    pid = getpid();

    status = lstat(argv[1], &curr);
    if(status == -1){
        perror(argv[1]);
    }

    status = execl(argv[1], argv[1], NULL);

    if(-1 == status){
        exit(errno);
    } else{
        exit(0);
    }
}
