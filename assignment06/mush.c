#include "mush.h"
#include "parseline.c"
/*#include "process.c"*/

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

int main(int argc, char *argv[]){
    int val,stage=0;
    char **pipes, **args;
    pid_t child;
    /*struct sigaction sa;
    struct itimerval timer;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK,&mask,NULL);
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM,&sa,NULL);
    */
    while(1){
        pipes = pipeline();
        while(pipes[stage]){
            args = parse_commands(pipes[stage++]);
            val = process(args);
            if(val){
                printf("failure\n");
            } else{
                printf("success\n");
            }
            /*
            while(args[i]){
                printf("\"%s\"", args[i++]);
            }
            printf("\n");
            i=0;
            */
        }
        free(args);
        stage = 0;
    }
    
    return 0;
}
