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
    /*
     *function reads the header of a hencoded file
     *arg:  fd - file descriptor of input
            num_ch - num of diff characters in file
            linked - null link list
     *returns a linked list of the file based on the header
     */
    uint8_t ch;
    uint32_t freq;
    uint32_t count=0;

    while(count < num_ch){
        Node *new;
        new = malloc(sizeof(Node));
        read(fd, &ch, 1);
        read(fd, &freq, 4);
        new -> ch = ch;
        new -> freq = freq;
        new -> next = NULL;
        new -> left = NULL;
        new -> right = NULL;
        linked = insert(linked, new);
        count++;
    }
    return linked;
}

char *convert_hex_to_bin(uint8_t hex){
    /*
     *Converts a hex val into binary
     *returns string of hex val in binary
     */
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
    /*
     *Reads the body of the file
     *Arg:  out - file descriptor for output
     *      body - the encoded body of the file to be decoded
     *      size - number of bytes int the body
     *      root - root of Huffman tree
     */
    int i, j, num;
    char *binary;
    unsigned char out_ch;
    Node *tmp = root;
    /* for the number of bytes in the body, traverse tree */
    for(i=0;i<size;i++){
        binary = convert_hex_to_bin(body[i]);
        /*iterate over binary string*/
        for(j=0; j<8; j++){
            if(binary[j] == '1')
                tmp = tmp -> right;
            else{
                tmp = tmp -> left;
            }
            /*if at leaf node write to out if freq > 0 */
            if(!tmp->right && !tmp->left){
                if(tmp->freq > 0){
                    out_ch = tmp->ch;
                    tmp-> freq -= 1;
                    num = write(out, &out_ch, 1);
                    if (num< 0){
                        perror("write");
                        exit(1);
                    }
                }
                /*reset tmp to root*/
                tmp = root;
            }
        }
        free(binary);
    }
}

void read_one_char(int out, Node *root){
    /*
     * If file only contains one char, this function deals with that
     */
    int i, size, num;
    char cp;

    size = root->freq;
    cp = root->ch;

    for(i=0; i < size;i++){
        num = write(out, &cp, 1);
        if(num<0){
            perror("write");
            exit(1);
        }
    }
}

int main(int argc, char *argv[]){

    int encoded, decoded;
    off_t body_size;
    uint8_t *body;
    uint32_t total_characters;
    Node *linked=NULL;
    Node *tree;
    char *code;

    /*if incorrect args*/
    if (argc > 3){
        printf("Usage: hdecode [ ( infile | - ) [ outfile ] ]\n");
        exit(0);
    }

    /*set in and out to STDIN and STDOUT*/
    encoded = 0;
    decoded = 1;

    /*if more than 1 arg and second arg isnt "-"*/
    if(argc > 1 && strcmp(argv[1], "-"))
        encoded = open(argv[1], O_RDONLY);

    if(encoded < 0){
        perror(argv[1]);
        exit(1);
    }
    /*if given three args*/
    if(argc == 3)
        decoded = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(decoded < 0){
        perror(argv[1]);
        exit(1);
    }
    body_size = find_size(encoded);
    /*checks if file size is larger than size of empty compressed file*/
    if(body_size > 4){
        read(encoded, &total_characters,4);
        linked = read_header(encoded, total_characters, linked);
        tree = create_tree(linked);

        code = malloc(8);
        code[0] = '\0';
        if (!code){
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        assign_codes(tree, 0, code);
        /*set body_size to size of body in bytes*/
        body_size -= 5 * total_characters + 4;
        body = malloc(body_size);
        if (!body){
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        if(total_characters > 1){
            body = read_file(encoded, body, body_size);
            read_body(decoded, body, body_size, tree);
        }

        if(total_characters == 1){
            read_one_char(decoded, tree);
        }

        free(code);
        free(body);
        free_tree(tree);
    }
    return 0;
}
