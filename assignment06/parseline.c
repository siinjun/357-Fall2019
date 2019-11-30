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
int num_pipes = -1;

char *strdup(const char *c)
{
    char *dup = calloc(strlen(c) + 1, 1);

    if (dup != NULL)
       strcpy(dup, c);

    return dup;
}

char *get_commands(){
    char *command_line;
    int i;

    command_line = calloc(CMD_LEN, 1);
    printf("$8-P ");
    fgets(command_line, CMD_LEN, stdin);
    if(command_line[0] == '\0'){
        free(command_line);
        printf("\n");
        exit(0);
    }
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
                fprintf(stderr, "invalid null command\n");
                exit(1);
            }
            else{
                pipe = cmd[i];
            }
        }
    }
    /*
    if(cmd[len-1] == '|'){
        fprintf(stderr, "invalid null command\n");
    }
    */
}

void check_inputs_redirection(char **pipeline){
    char *token, *tmp;
    int i=0, j, inputs, outputs, len;
    const char *delim = " ";
    bool ambig_out=false;

    for(;pipeline[i];i++){
        if(ambig_out){
            fprintf(stderr, "%s: ambiguous output\n", token);
            exit(1);
        }
        inputs = 0;
        outputs = 0;
        len = strlen(pipeline[i]);
        tmp = calloc(len + 1, 1);
        tmp = strcpy(tmp, pipeline[i]);
        token = strtok(tmp, delim);
        for(j=0;j<len;j++){
            if(pipeline[i][j] == '<'){
                inputs++;
            }
            if(pipeline[i][j] == '>'){
                outputs++;
            }
            if(i > 0){
                if(inputs > 0){
                    fprintf(stderr, "%s: ambiguous input\n", token);
                    exit(1);
                }
            }
            if(outputs == 1){
                ambig_out = true;
            }
            if(inputs > 1){
                fprintf(stderr, "%s: bad input redirection\n", token);
                exit(1);
            }
            if(outputs > 1){
                fprintf(stderr, "%s: bad output redirection\n", token);
                exit(1);
            }
        }
    }
}

char **parse_commands(char *cmd){
    char **args;
    const char parse[2] = " ";
    char *token, *prog;
    bool set_in = false, set_out = false;

    argc=0;
    args = calloc(MAX_ARGS, sizeof(char *));
    token = strtok(cmd, parse);
    prog = strdup(token);

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
                    fprintf(stderr, "%s: too many args\n", prog);
                    exit(1);
                }
            }
        }else{
            if(!strcmp(token, "<")){
                set_in = true;
            }
            else{
                set_out = true;
            }

        }
        token = strtok(NULL, parse);
    }
    free(prog);
    return args;
}

char **get_pipeline(char *cmd){
    char **pipe, *token;
    const char parse[2] = "|";
    int i = 0;
    pipe = calloc(PIPE_MAX, sizeof(char *));

    token = strtok(cmd, parse);
    while(token != NULL){
        num_pipes++;
        pipe[i++] = token;
        token = strtok(NULL, parse);
        if(i > PIPE_MAX){
            fprintf(stderr, "Too many args.\n");
            exit(1);
        }
    }
    return pipe;
}


/*
int other(){
    char *cmd, *line, **pipeline, **arguments;
    int stage = 0;

    cmd = get_commands();
    check_pipes(cmd);
    pipeline = get_pipeline(cmd);
    check_inputs_redirection(pipeline);
    line = calloc(CMD_LEN, 1);

    while(pipeline[stage]){
        if(stage == 0){
            input = "original stdin";
            if(num_pipes > 0){
                output = "pipe to stage ";
            }else{
                output = "original stdout";
            }
        } else {
            input = "pipe from stage ";
            if(stage == num_pipes){
                output = "original stdout";
            }else{
                output = "pipe to stage ";
            }
        }
        line = strcpy(line, pipeline[stage]);
        arguments = parse_commands(pipeline[stage]);
        format_stdout(arguments, line, stage++);
        memset(line, 0, CMD_LEN);
        free(arguments);
    }
    free(cmd);
    free(pipeline);
    free(line);
    return 0;
}
*/
char ** pipeline(){
    char *cmd, *line, **pipeline;

    cmd = get_commands();
    check_pipes(cmd);
    pipeline = get_pipeline(cmd);
    check_inputs_redirection(pipeline);
    line = calloc(CMD_LEN, 1);

    return pipeline;
}