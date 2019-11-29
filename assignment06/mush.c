#include "mush.h"
#include "parseline.c"



int main(int argc, char *argv[]){
    int stage=0;
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
    pipes = pipeline();
    while(pipes[stage]){
        printf("%s\n", pipes[stage]);
        args = parse_commands(pipes[stage++]);
    }
    return 0;
}
