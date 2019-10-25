#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include "node.h"
#include "tree.c"



Node *read_header(int fd, uint32_t num_ch, Node *linked){

    uint8_t *ch;
    uint32_t *freq;
    uint32_t count=0;

    ch = calloc(1, sizeof(uint8_t));
    if(!ch){
        perror("Calloc");
        exit(1);
    }
    freq = calloc(1, sizeof(uint32_t));
    if(!freq){
        perror("Calloc");
        exit(1);
    }
    while(count < num_ch){
        Node *new;
        new = malloc(sizeof(Node));
        read(fd, ch, 1);
        read(fd, freq, 4);
        new -> ch = ch[0];
        new -> freq = freq[0];
        new -> next = NULL;
        new -> left = NULL;
        new -> right = NULL;
        linked = insert(linked, new);
        count++;
    }
    return linked;
}

char *convert_hex_to_bin(uint8_t hex){
    char *bin;
    int i=0;
    bin = calloc(9, sizeof(char));
    for(;i<8;i++)
        bin[i] = '0';
    if(hex >= 128){
        hex -= 128;
        bin[0] = '1';
    }
    if(hex >= 64){
        hex -= 64;
        bin[1] = '1';
    }
    if(hex >= 32){
        hex -= 32;
        bin[2] = '1';
    }
    if(hex >= 16){
        hex -= 16;
        bin[3] = '1';
    }
    if(hex >= 8){
        hex -= 8;
        bin[4] = '1';
    }
    if(hex >= 4){
        hex -= 4;
        bin[5] = '1';
    }
    if(hex >= 2){
        hex -= 2;
        bin[6] = '1';
    }
    if(hex >= 1){
        hex -= 1;
        bin[7] = '1';
    }
    bin[8] = '\0';
    return bin;

}

void read_body(int out, uint8_t *body, long int size, Node *root){

    int i=0, j;
    char *binary, *ch;
    Node *tmp = root;

    ch = malloc(1);
    for(;i<size;i++){
        binary = convert_hex_to_bin(body[i]);
        for(j=0; j<8; j++){
            if(binary[j] == '1')
                tmp = tmp -> right;
            else{
                tmp = tmp -> left;
            }
            if(!tmp->right && !tmp->left && tmp->freq > 0){
                ch[0] = tmp->ch;
                tmp-> freq -= 1;
                write(out, tmp, 1);
                tmp = root;
            }
            if(!tmp->right && !tmp->left && tmp->freq== 0){
                tmp = root;
            }

        }
    }
}


int main(int argc, char *argv[]){

    int encoded, decoded;
    off_t body_size;
    uint8_t *body;
    uint32_t *total_characters;
    Node *linked=NULL;
    Node *tree;
    char *code;

    total_characters = calloc(1, sizeof(uint32_t));
    /*set in and out to STDIN and STDOUT*/
    decoded = 1;
    encoded = 1;
    if(argc > 1)
        encoded = open(argv[1], O_RDONLY);
    if(encoded < 0){
        perror(argv[1]);
        exit(1);
    }
    if(argc > 2)
        decoded = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(decoded < 0){
        perror(argv[1]);
        exit(1);
    }
    body_size = find_size(encoded);
    if(body_size > 0){
        read(encoded,total_characters,4);
        linked = read_header(encoded, total_characters[0], linked);
        tree = create_tree(linked);
        
        code = malloc(8);
        code[0] = '\0';
        if (!code){
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        assign_codes(tree, 0, code);

        body_size -= 5 * total_characters[0] + 4;
        body = malloc(body_size);
        if (!body){

            perror("malloc");
            exit(EXIT_FAILURE);
        }
        body = read_file(encoded, body, body_size);
        read_body(decoded, body, body_size, tree);    
    }
    return 0;
}
