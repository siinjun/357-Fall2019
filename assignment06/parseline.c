#include "mush.h"

#define CMD_LEN     1025
#define PIPE_MAX    20
#define MAX_ARGS    20
#define PROMPT      4
char *strdup(const char *c)
{
    char *dup = calloc(strlen(c) + 1, 1);

    if (dup != NULL)
       strcpy(dup, c);

    return dup;
}

char *get_commands(){
    char *command_line;
    int i, check;

    cmd_line = calloc(CMD_LEN, 1);
    /*printf("8-P ");
    fflush(stdin);*/
    if(write_prompt){
        check = write(STDOUT_FILENO, "8-P ", PROMPT);
        if(check < 0){
            perror("write");
            exit(errno);
        }
    }
    /*if reading from scriptfile, read from scriptfile pointer*/
    if(!write_prompt){
        strcpy(cmd_line, scriptfile);
    }else{
        check = read(STDIN_FILENO, cmd_line, CMD_LEN);
        if(check < 0){
            perror("read");
            exit(errno);
        }
    }
    
    /*fgets(cmd_line, CMD_LEN, stdin);*/
    if(cmd_line[0] == '\n'){
        free(cmd_line);
        skip = true;
        return NULL;
    }    
    if(cmd_line[0] == 0){
        free(cmd_line);
        if(write_prompt){
            printf("\n");
        }
        exit(0);
    }
    if(cmd_line[CMD_LEN -1] != 0){
        fprintf(stderr, "command over 1024 bytes\n");
        free(cmd_line);
        skip = true;
        return NULL;
    }
    i = strlen(cmd_line) - 1;
    cmd_line[i] = '\0';
    return cmd_line;
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
                free(cmd);
                skip = true;
                return;
            }
            else{
                pipe = cmd[i];
            }
        }
    }
    if(pipe == '|'){
        fprintf(stderr, "invalid null command\n");
        skip = true;
        free(cmd);
        return;
    }
}

void check_inputs_redirection(char **pipeline){
    char *token, *tmp;
    int i=0, j, inputs, outputs, len;
    const char *delim = " ";
    bool ambig_out=false;

    for(;pipeline[i];i++){
        if(ambig_out){
            fprintf(stderr, "%s: ambiguous output\n", token);
            skip = true;
            free(cmd_line);
            free(pipeline);
            free(tmp);
            return;
        } else if(i>0 && !ambig_out){
            free(tmp);
        }
        inputs = 0;
        outputs = 0;
        len = strlen(pipeline[i]);
        /*create tmp so pipeline isn't affected*/
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
                    skip = true;
                    free(cmd_line);
                    free(pipeline);
                    free(tmp);
                    return;
                }
            }
            if(outputs == 1){
                ambig_out = true;
            }
            if(inputs > 1){
                fprintf(stderr, "%s: bad input redirection\n", token);
                skip = true;
                free(cmd_line);
                free(pipeline);
                free(tmp);
                return;
            }
            if(outputs > 1){
                fprintf(stderr, "%s: bad output redirection\n", token);
                skip = true;
                free(cmd_line);
                free(pipeline);
                free(tmp);
                return;
            }
        }
        if(pipeline[i][j-1] == '<'){
            fprintf(stderr, "%s: bad input redirection\n", token);
            skip = true;
            free(cmd_line);
            free(pipeline);
            free(tmp);
            return;
        }
        if(pipeline[i][j-1] == '>'){
            fprintf(stderr, "%s: bad input redirection\n", token);
            skip = true;
            free(cmd_line);
            free(pipeline);
            free(tmp);
            return;        
        }
    }
    free(tmp);
}

char **parse_commands(char *cmd){
    char **args;
    const char parse[2] = " ";
    char *token, *prog;
    bool set_in = false, set_out = false;

    pline_argc=0;
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
                args[pline_argc++] = token;
                if(pline_argc > MAX_ARGS){
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
    num_pipes = -1;
    pipe = calloc(PIPE_MAX, sizeof(char *));

    token = strtok(cmd, parse);
    while(token != NULL){
        num_pipes++;
        pipe[i++] = token;
        token = strtok(NULL, parse);
        if(i > PIPE_MAX){
            fprintf(stderr, "Too many args.\n");
            skip = true;
            return NULL;
        }
    }
    return pipe;
}


char ** pipeline(){
    char *cmd, **pipeline;

    cmd = get_commands();
    if(cmd == NULL){
        return NULL;
    }
    check_pipes(cmd);
    if(skip == true){
        return NULL;
    }
    pline_pipeline = get_pipeline(cmd);
    if(skip == true){
        return NULL;
    }
    check_inputs_redirection(pline_pipeline);
    if(skip == true){
        return NULL;
    }
    return pline_pipeline;
}