#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *arg[]){
    pid_t child, pid;
    int status;

    printf("Hello, world!\n");

    if((child = fork())){
        /*parent*/
        pid = getpid();
        printf("This is the parent, pid %d.\n", pid);
        if(-1 == wait(&status)){
            perror("wait");
            exit(1);
        }
        if((WIFEXITED(status) && WEXITSTATUS(status)) || WIFSIGNALED(status)){
            exit(1);
        } else{
        printf("This is the parent, pid %d, signing off.\n", pid);
            exit(0);
        }
    }
    /*child*/
    pid = getpid();
    printf("This is the child, pid %d.\n", pid);
    exit(0);
}
