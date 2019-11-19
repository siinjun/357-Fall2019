#include<string.h>
#include<strings.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define CMD_LEN     1024
#define PIPE_MAX    20
#define MAX_ARGS    20
char *output;
char *input;
int argc;

char *get_commands(){
    char *command_line;
    int i;

    command_line = calloc(CMD_LEN, 1);
    printf("Line: ");
    fgets(command_line, CMD_LEN, stdin);
    i = strlen(command_line) - 1;
    command_line[i] = '\0';
    return command_line;
}

void check_pipes(char *cmd){
    int i, len = strlen(cmd);
    char pipe = '\0';

    for(i=0; i < len; i++){
        if(cmd[i] != ' '){
            if(cmd[i] == '|' && pipe != '|'){
                pipe = '|';
            }
            else if(cmd[i] == '|' && pipe == '|'){
                /*pipe line error*/
                fprintf(stderr, "invalid null command.\n");
                exit(1);
            }
            else{
                pipe = cmd[i];
            }
        }
    }
}

char **parse_commands(char *cmd){
    char **args;
    const char parse[2] = " ";
    char *token;
    bool set_in = false, set_out = false;

    argc=0;
    args = calloc(MAX_ARGS, sizeof(char *));
    token = strtok(cmd, parse);

    while(token != NULL){
        if(strcmp(token, "<") && strcmp(token, ">")){
            if(set_in){
                set_in = false;
                input = token;
            }
            else if(set_out){
                set_out = false;
                output = token;
            }
            else{
                args[argc++] = token;
                if(argc > MAX_ARGS){
                    fprintf(stderr, "Too many args.\n");
                    exit(1);
                }
            }
        }
        else{
            if(!strcmp(token, "<")){
                set_in = true;
            }
            else{
                set_out = true;
            }
        }
        token = strtok(NULL, parse);
    }
    return args;
}

char **get_pipeline(char *cmd){
    char **pipe, *token;
    const char parse[2] = "|";
    int i = 0;

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
    return pipe;
}
/*
char **get_inout(char *cmd, char **inout){
    int i;

    for(i = 0; i < strlen(cmd); i++){


    }
}
*/
int main(){
    char *cmd, **pipeline, **arguments;
    int i=0, stage = 0;

    input = calloc(24, 1);
    output = calloc(24,1);

    cmd = get_commands();
    check_pipes(cmd);
    pipeline = get_pipeline(cmd);

    while(pipeline[stage]){
        i = 0;
        input = "standard input";
        output = "standard output";
        printf("command line: %s\n", pipeline[stage]);
        arguments = parse_commands(pipeline[stage++]);
        printf("input: %s\n", input);
        printf("output: %s\n",output);
        printf("argc: %d\n", argc);
        printf("argv: ");
        while(arguments[i]){
            printf("\"%s\" ", arguments[i++]);
        }
        printf("\n");
    }
    return 0;
}
