#include<signal.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

void handler(int num){
    static int count = 0;
    if(count%2 == 0)
        write(0,"Tick...",7);
    else
        write(0,"Tock\n",5);
    count++;
}



int main(int argc, char * argv[]){

    struct sigaction sa;
    struct itimerval timer;
    sigset_t mask;
    bool valid = true;
    char *endptr;
    int repeat;
    int num = 0;


    if(argc > 2 || argc == 1){
        printf("usage: timeit <seconds>\n");
        valid = false;
    }
    if(valid){
        repeat = 2 * strtol(argv[1],&endptr,10);
        if(*endptr != '\0'){
            printf("%s: malformed time.\n", argv[1]);
            printf("usage: timeit <seconds>\n");
            valid = false;
        }
        if(valid){
            sigemptyset(&mask);
            sigaddset(&mask, SIGALRM);
            sigprocmask(SIG_BLOCK,&mask,NULL);
            sa.sa_handler = handler;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sigaction(SIGALRM,&sa,NULL);

            timer.it_value.tv_sec = 0;
            timer.it_value.tv_usec = 500000;
            timer.it_interval.tv_sec = 0;
            timer.it_interval.tv_usec = 500000;
            setitimer(ITIMER_REAL, &timer, NULL);

            sigprocmask(SIG_UNBLOCK,&mask,NULL);
            sigdelset(&mask,SIGALRM);
            while(num++ < repeat)
                pause();

            write(0,"Time's up!\n",11);
        }
    }
    return 0;
}
