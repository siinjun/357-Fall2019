#include "mush.h"

void sighandler(int signum);

int main(void) {
    struct sigaction sa;
    sa.sa_handler = sighandler;
    sigemptyset(&(sa.sa_mask));
    sigaddset(&(sa.sa_mask), SIGINT);
    sigaction(SIGINT, &sa, NULL);

    int i;
    for(i = 0; i < 5; i++) {
        printf("Sleeping...\n");
        sleep(5);
        printf("Woke up!\n");
    }
}

void sighandler(int signum) {
    printf("Signal caught!\n");
}
