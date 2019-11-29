#include "mush.h"

int process(char *argv[]){
    pid_t child, pid;
    int status;
    struct stat curr;
    if((child = fork())){
        /*parent*/
        pid = getpid();
        if(-1 == wait(&status)){
            perror("wait");
            exit(1);
        }
        if((WIFEXITED(status) && WEXITSTATUS(status)) || WIFSIGNALED(status)){
            return 1;
        } else{
            return 0;
        }
    }
    /*child*/
    pid = getpid();
    /*
    status = lstat(argv[0], &curr);
    if(status == -1){
        perror(argv[1]);
    }
    */
    status = execvp(argv[0], argv);

    if(-1 == status){
        fprintf(stderr, "%s: command not found\n", argv[0]);
        exit(errno);
    } else{
        exit(0);
    }
}
