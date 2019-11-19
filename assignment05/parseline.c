#include<string.h>
#include<strings.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>

#define CMD_LEN     1024
#define PIPE_MAX    20
#define MAX_ARGS    20

char *get_commands(){
    char *command_line;

    command_line = calloc(CMD_LEN, 1);
    printf("Line: ");
    fgets(command_line, CMD_LEN, stdin);

    return command_line;
}

char **parse_commands(char *cmd){
    char **args;
    const char parse[2] = " ";
    char *token;
    int i=0;

    args = calloc(MAX_ARGS, sizeof(char *));
    token = strtok(cmd, parse);

    while(token != NULL){
        args[i++] = token;
        token = strtok(NULL, parse);
        if(i > MAX_ARGS){
            fprintf(stderr, "Too many args.\n");
            exit(1);
        }
    }
    return args;
}

char **get_pipeline(char *cmd){
    char **pipe, *token;
    const char parse[2] = "|";
    int i = 0, j;

    pipe = calloc(PIPE_MAX, sizeof(char *));

    token = strtok(cmd, parse);
    while(token != NULL){
        pipe[i++] = token;
        token = strtok(NULL, parse);
        if(i > MAX_ARGS){
            fprintf(stderr, "Too many args.\n");
            exit(1);
        }
    }
    j = strlen(pipe[--i]);
    pipe[i][j-1] = '\0';
    return pipe;

}

int main(){
    char *cmd, **pipeline, **arguments;
    int i=0, stage = 0;

    cmd = get_commands();

    pipeline = get_pipeline(cmd);

    while(pipeline[stage]){
        i = 0;
        arguments = parse_commands(pipeline[stage++]);
        while(arguments[i]){
            printf("%s\n", arguments[i++]);
        }
    }
    return 0;
}
