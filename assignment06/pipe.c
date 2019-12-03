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
/*
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
                ;
            }
        }
        else{
            if(i == 4){
                ;
            } else{
                if(dup2(fd[2*i+1], STDOUT_FILENO) < 0){
                    perror("dup2");
                    if(errno == EBADF){
                        printf("wtf\n");
                    }
                    exit(10);
                }
            }
            if(i == 0){
                ;
            } else{
                if(dup2(fd[2*i - 2], STDIN_FILENO) < 0){
                    perror("dup2");
                    exit(1);
                }
            }
            close(fd[2*i]);
            close(fd[2*i - 2]);
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
}*/

int new(){
    pid_t child;
    int i = 0;
    int fd[2], status;

    if(pipe(fd) < 0){
        perror("pipe");
        exit(1);
    }
    while(i < 2){
        if(i>0)
            close(fd[1]);
        if((child = fork())){
            /*parent*/
            if(-1 == wait(&status)){
                perror("wait");
                exit(1);
            }
            printf("done waiting\n");
            if(WIFEXITED(status) && WEXITSTATUS(status) || WIFSIGNALED(status)){
                exit(1);
            }
            else{
                ;
            }

        } else{
            /*child*/
            if(i ==0){
                /*do ls*/
                if(dup2(fd[1], STDOUT_FILENO) < 0){
                    perror("dup2");
                    exit(1);
                }
                close(fd[0]);
                close(fd[1]);
                execlp("ls", "ls", NULL);
            } else {
                /*sort output of ls*/
                if(dup2(fd[0], STDIN_FILENO) < 0){
                    perror("dup2");
                    exit(1);
                }
                close(fd[0]);
                close(fd[1]);
                printf("right before sort\n");
                execlp("tr", "tr", "\'A-Z\'", "\'a-z\'", NULL);
            }
        }
        i++;
    }
    return 0;
}
int main(){
    int e;
    e = new();
    return 0;
}
