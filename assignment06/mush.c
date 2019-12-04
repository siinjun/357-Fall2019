#include "mush.h"
#include "parseline.c"
#define SIZE 1024
/*#include "process.c"*/
int DEBUG = false;
void handler(int num){
    /*kill the process child but never the parent*/
    pid_t child;
}
void close_fd(int fd[], int stage){
    int i;
    for(i = 0; i < stage + 1; i++){
        close(fd[2*i]);
        close(fd[2*i+1]);
    }
}
/*idk the purpose of this???*/
void process(int fd){
    int num;
    char buf[SIZE];

    memset(buf, 0, SIZE);
    while((num=read(fd,buf,SIZE)) > 0){
        if(write(STDOUT_FILENO, buf, num) < 0){
            perror("write");
            exit(1);
        }
    }
    if(num < 0){
        perror("read");
        exit(1);
    }
}

int execute(char *argv[], int fd[], int stage){
    pid_t child;
    int status;
    int i;

    if((child = fork())){
        /*parent*/
        /*close the write end of previous pipe*/
        for(i = 0; i < stage; i++){
            close(fd[(i*2) + 1]);
        };
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
    /*if at final stage, output to stdout or output file*/
    if(stage == num_pipes){
        if(strncmp(output, "stdout", 6)){
        /*if output is not stdout, dup2 file to stdout*/
            int out;
            out = open(output, O_CREAT | O_TRUNC | O_WRONLY, 0644);
            if(dup2(out, STDOUT_FILENO) < 0){
                perror("dup2");
                exit(1);
            }
            close(out);
        }
    }
    /* else manipulate stdout to pipe*/
    else{
        if(dup2(fd[stage*2 + 1], STDOUT_FILENO) < 0){
            perror("dup2");
            exit(1);
        }
    }
    /*if in middle of pipeline, stdin is the read end of the previous pipe*/
    if(stage > 0){
        if(dup2(fd[stage*2 - 2], STDIN_FILENO) < 0 ){
            perror("dup2");
            exit(1);
        }
    }
    /*close all fds, in child*/
    close_fd(fd, stage);
    status = execvp(argv[0], argv);

    if(-1 == status){
        /*FIXME idk which one is right*/
        perror(argv[0]);
        fprintf(stderr, "%s: command not found\n", argv[0]);
        exit(errno);
    } else{
        exit(0);
    }
}

int main(int argc, char *argv[]){
    int val,stage=0, fd[40];
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
        if(skip == false){
            stage = 0;
            while(pipes[stage]){
                /* find the output and input of each stage*/
                if(stage == 0){
                    input = "stdin";
                    if(num_pipes > 0){
                        output = "pipe to stage ";
                    }else{
                        output = "stdout";
                    }
                } else {
                    input = "pipe from stage ";
                    if(stage == num_pipes){
                        output = "stdout";
                    }else{
                        output = "pipe to stage ";
                    }
                }
                args = parse_commands(pipes[stage]);
                /*set up pipes*/
                if(pipe(fd + stage * 2)){
                    perror("pipe");
                    exit(1);
                }
                if(DEBUG){
                    printf("%s\n", input);
                    printf("%s\n", output);
                    printf("stage #%d\n", stage);
                }
                if(!strcmp(args[0], "cd")){
                    /*if command is cd, don't create child process*/
                    val = chdir(args[1]);
                    if(val==-1){
                        perror(args[1]);
                    }
                } else {
                    /*if not cd, create child process through forking*/
                    val = execute(args, fd, stage);
                }
                stage++;
            }
            free(args);
            free(pipes);
            /*close all pipes*/
        }
        else{
            skip = false;
        }

    }
    
    return 0;
}
