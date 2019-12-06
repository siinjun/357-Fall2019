#include "mush.h"
#include "parseline.c"
#define SIZE 1024
#define MAX_FD 20

int DEBUG = false;
struct sigaction sa;

/*need to work on this*/
void handler(int num){
    /*kill the process child but never the parent*/
    printf("\n");
    execl("mush", "mush", NULL);
}

void close_fd(int fd[], int stage, bool parent){
    int i;
    /*if parent, close only read ends of pipes, else close all*/
    for(i = 0; i < stage + 1; i++){
        close(fd[2*i]);
        if(!parent)
            close(fd[2*i+1]);
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
        }
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
    /*if input is redirected to file*/
    if(stage == 0 && strncmp(input, "stdin", 5)){
        int file;
        file = open(input, O_RDONLY, 0644);
        if(file < 0){
            perror("open");
            exit(1);
        }
        if(dup2(file, STDIN_FILENO) < 0){
            perror("dup2");
            exit(1);
        }
        close(file);
    }
    /*if in middle of pipeline, stdin is the read end of the previous pipe*/
    if(stage > 0){
        if(dup2(fd[stage*2 - 2], STDIN_FILENO) < 0 ){
            perror("dup2");
            exit(1);
        }
    }
    /*close all fds, in child*/
    close_fd(fd, stage, false);
    status = execvp(argv[0], argv);

    if(-1 == status){
        perror(argv[0]);
        free(cmd_line);
        free(pline_pipeline);
        free(pline_args);
        exit(errno);
    } else{
        exit(0);
    }
}

void determine_in_out(int stage){
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
}

int shell(){
    int val, stage=0, fd[MAX_FD];
    char **pipes, **args;

    /*get the parent's id, for signal handling?*/
    parent_id = getpid();
    while(1){

        pipes = pipeline();
        if(skip == false){
            stage = 0;
            val = 0;
            while(pipes[stage]){
                /* find the output and input of each stage*/
                /*free pline args if stage is over 1*/
                if(stage > 0){
                    free(pline_args);
                }
                determine_in_out(stage);
                pline_args = parse_commands(pipes[stage]);
                /*set up pipes*/
                if(pipe(fd + (stage * 2))){
                    perror("pipe");
                    exit(1);
                }
                if(!strcmp(pline_args[0], "cd")){
                    /*if command is cd, don't create child process*/
                    val = chdir(pline_args[1]);
                    if(val==-1){
                        perror(pline_args[1]);
                    }
                } else{
                    /*if not cd, create child process through forking*/
                    val = execute(pline_args, fd, stage);
                    if(val != 0){
                        /*error received so restart cmd line*/
                        free(pline_args);
                        free(pipes);
                        free(cmd_line);
                        close_fd(fd, stage, true);
                        break;
                    }
                }
                stage++;
            }
            /*if process succeeded, free and close everything*/
            if(val==0){
                free(pline_args);
                free(pipes);
                free(cmd_line);
                /*close all read ends of pipes*/
                close_fd(fd, stage-1, true);
            }
        }
        /*if given a bad command and reading from file, end program*/
        else if (skip = true && !write_prompt){
            fclose(scp_fd);
            exit(1);
        }
        /*else skip command*/
        else{
            skip = false;
        }
    }
    return 0;
}

int main(int argc, char *argv[]){

    pid_t child;
    sigset_t mask;

    if(argc > 2){
        fprintf(stderr, "usage: %s [ scriptfile ]\n", argv[0]);
        exit(0);
    }

    /*set up signal handler*/
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK,&mask,NULL);
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT,&sa,NULL);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    if(argc == 1){
        write_prompt = true;
        shell();
    } else if(argc == 2){

        scp_fd = fopen(argv[1], "r");
        if(scp_fd == NULL){
            perror("fopen");
            exit(errno);
        }
        write_prompt = false;
        shell();
    }

    return 0;
}
