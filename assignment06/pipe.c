#include "mush.h"
#define SIZE 1024
#define WRITE_END 1
#define READ_END 0
int process(int fd, int out){
    int num;
    char buf[SIZE];
    memset(buf, 0 , SIZE);
    while((num=read(fd,buf,SIZE)) > 0){
        if(write(out, buf, num) < 0){
            perror("write");
            exit(1);
        }
    }
    if(num < 0){
        perror("read");
        exit(1);
    }
}

int main(int argc, char *argv[]){
    int fd[10];
    int i=0, status;
    pid_t child;
    while(i<5){
        if(pipe(fd + 2*i)){
            perror("pipe");
            exit(1);
        }
        if((child = fork())){
            /*parent*/
            close(fd[2*i+1]);
            if(i==0){
                ;
            } else{
                process(fd[2*i], 0);
            }
            if(-1 == wait(&status)){
                perror("wait");
                exit(1);
            }
            if(WIFEXITED(status) && WEXITSTATUS(status) || WIFSIGNALED(status)){
                exit(1);
            }
            else{
                /*close(fd[2*i]);*/
                ;
            }
        }
        else{
            if(dup2(fd[2*i+1], STDOUT_FILENO) < 0){
                perror("dup2");
                if(errno == EBADF){
                    printf("wtf\n");
                }
                exit(10);
            }
            if(dup2(fd[2*i], STDIN_FILENO) < 0){
                perror("dup2");
                exit(1);
            }
            close(fd[2*i]);
            close(fd[2*i + 1]);
            printf("This is stage %d.\n", i);
            exit(0);
        }
        i++;
    }
    process(fd[0], STDOUT_FILENO);
    process(fd[2], STDOUT_FILENO);
    process(fd[4], STDOUT_FILENO);
    process(fd[6], STDOUT_FILENO);
    process(fd[8], STDOUT_FILENO);
}

