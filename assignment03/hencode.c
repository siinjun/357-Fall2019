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

#define CHONK 80

int bytes_for_file=0;

char *get_bin(unsigned char *filecontents, off_t size, Node **list){
    /*
     *This function takes in a file's contents and turns it into binary based
     *on the code for each char.
     *
     *Returns a char pointer of the file in binary(so not really binary but
     *y'know)
    */
    char *file_in_bin, *code;
    int i, count=0, buff=CHONK, extra;
    Node *tmp;

    file_in_bin = malloc(buff);
    file_in_bin[0] = '\0';

    if(!file_in_bin){
        perror("malloc");
        exit(2);
    }
    /*iterate through each char in the file*/
    for(i=0; i < size; i++){
        unsigned char ch = filecontents[i];
        tmp = list[ch];
        code = tmp->code;
        count += strlen(code);

        if(count >= buff){
            file_in_bin = realloc(file_in_bin, buff * 2);
            if(!file_in_bin){
                perror(file_in_bin);
                exit(2);
            }
            buff *= 2;
        }
        file_in_bin = strcat(file_in_bin, code);

    }
    /*if length of string doesn't end at a byte, pad buffer with 0s*/
    extra = strlen(file_in_bin) % 8;
    while(extra%8 != 0){
        if(count >= buff){
            file_in_bin = realloc(file_in_bin, buff * 2);
            if(!file_in_bin){
                perror(file_in_bin);
                exit(2);
            }
            buff *= 2;
        }
        file_in_bin[count++] = '0';
        extra++;
    }
    /*realloc pointer to actual size*/
    file_in_bin = realloc(file_in_bin, count+1);
    if(!file_in_bin){
        perror("realloc");
        exit(1);
    }
    file_in_bin[count] = '\0';
    return file_in_bin;

}

void write_header(int outfile, Node **list){
    /*writes the header of the file, returns void*/
    uint8_t ch;
    uint32_t count;
    int i;

    for(i=0; i < 256; i++){
        if(list[i]){
            Node *node = list[i];
            ch = node->ch;
            count = node->freq;
            write(outfile, &ch, 1);
            write(outfile, &count, 4);
        }
    }
}


uint8_t binary_to_hex(char *binary){
    /*
    converts binary string to a hex val...
    */
    uint8_t size = 0;

    if(binary[0] == '1')
        size += 128;
    if(binary[1] == '1')
        size += 64;
    if(binary[2] == '1')
        size += 32;
    if(binary[3] == '1')
        size += 16;
    if(binary[4] == '1')
        size += 8;
    if(binary[5] == '1')
        size += 4;
    if(binary[6] == '1')
        size += 2;
    if(binary[7] == '1')
        size += 1;
    return size;
}

uint8_t *convert_to_hex(char *binary){
    /*Takes in string of 1s and 0s and converts them into uint8_t nums
     *
     *Returns number of bytes written to buffer
     */
    int i=0, bytes=0;
    char *byte;
    size_t buff = CHONK;
    uint8_t *pp;
    pp = malloc(buff);
    byte = malloc(8);
    /*while binary string is not at its end, keep converting*/
    while(binary[i] != '\0'){
        byte[i%8] = binary[i];
        if(i%8 == 7){
            if (bytes >= buff){
                pp = realloc(pp, buff * 2);
                if (!pp){
                    perror("realloc");
                    exit(1);
                }
                buff *= 2;
            }
            pp[bytes++] = binary_to_hex(byte);
            byte = memset(byte, 0, 7);
        }
        i++;
    }
    /*sets how many bytes to write*/
    bytes_for_file = bytes;
    free(byte);
    return pp;
}

int main(int argc, char *argv[]){

    Node **list;
    int i, infile, outfile, num;
    uint32_t ch_count = 0;
    uint8_t *hex;
    off_t size;
    unsigned char *filecontents;
    char *binary;

    /*usage handling*/
    if(argc < 2){
        printf("Usage: hencode infile [ outfile ]\n");
        exit(0);
    }
    /*open file*/
    infile = open(argv[1], O_RDONLY);

    /*if given bad file*/
    if (infile == -1){
        perror(argv[1]);
        exit(1);
    }
    /*initialize output to stdout*/
    outfile = 1;

    /*write to file given, if given*/
    if(argc == 3){
        outfile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }

    /*if file can't be opened*/
    if (outfile == -1){
        perror(argv[2]);
        exit(1);
    }

    list = get_list(argv[1]);

    for(i=0; i < 256; i++){
        if(list[i]){
            ch_count++;
        }
    }
    /*initialize file contents to extern var*/
    size = file_size;
    filecontents = file_cont;

    /*initial write to file*/
    write(outfile, &ch_count, 4);

    /*if file is not empty, continue to encode*/
    if(ch_count > 0){
        write_header(outfile, list);
        if (ch_count > 1){
            binary = get_bin(filecontents, size, list);
            hex = convert_to_hex(binary);
            num = write(outfile, hex, bytes_for_file);
            if(num < 0){
                perror("write");
                exit(1);
            }
            free(binary);
            free(hex);
        }
        close(infile);
        close(outfile);
        /*free tree since no longer needed*/
        free_tree(glob_tree);
    }
    free(list);
    free(filecontents);
    return 0;
}
