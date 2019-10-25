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

char *get_bin(char *filecontents, off_t size, Node **list){

    char *file_in_bin, *code;
    int i, count=0, buff=40;
    Node *tmp;

    file_in_bin = malloc(buff);
    file_in_bin[0] = '\0';

    if(!file_in_bin){
        perror("malloc");
        exit(2);
    }

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
    file_in_bin = realloc(file_in_bin, count);
    printf("count=%d\n", count);
    if(!file_in_bin){
        perror("realo");
        exit(1);
    }
    file_in_bin[count] = '\0';
    return file_in_bin;    

}

int write_header(int outfile, Node **list){

    uint8_t *ch;
    uint32_t *count;
    int i = 0;
    int single_bytes=0, multi_bytes=0;
    int total;

    count = calloc(1, sizeof(uint32_t));
    ch = calloc(1, sizeof(uint8_t));
    for(; i < 256; i++){
        if(list[i]){
            Node *node = list[i];
            ch[0] = node->ch;
            count[0] = node->freq;
            write(outfile, ch, 1);
            write(outfile, count, 4);
            single_bytes += 1;
            multi_bytes += 1;
        }
    }
    
    total = single_bytes + (4*multi_bytes);
    return total;
}


uint8_t binary_to_hex(char *binary){

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

int convert_to_hex(char *binary, uint8_t *pp){
    /* Returns number of bytes written to buffer*/
    int i=0, bytes=0;
    char *byte;
    size_t buff = 8;
    byte = malloc(8);

    while(binary[i] != '\0'){
        byte[i%8] = binary[i];
        if(i%8 == 7){
            if (bytes > buff){
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
    if(i%8 != 0){
        if (bytes > buff){
            pp = realloc(pp, buff * 2);
            if (!pp){
                perror("realloc");
                exit(1);
            }
            buff *= 2;
        }
        pp[bytes++] = binary_to_hex(byte);
    }
    pp = realloc(pp, bytes);
    return bytes;

}

int main(int argc, char *argv[]){

    Node **list;
    int i, infile, outfile, bytes;
    uint32_t ch_count = 0;
    uint32_t *number_of_chars;
    uint8_t *hex;
    off_t size;
    char *filecontents, *binary;

    list = get_list(argv[1]);

    infile = open(argv[1], O_RDONLY);
    /*if given bad file*/
    if (infile == -1){
        perror(argv[2]);
        exit(1);
    }
    /*write to file given*/
    if(argc == 3){
        outfile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    /*if file can't be opened*/
    if (outfile == -1){
        perror(argv[2]);
        exit(1);
    }
    /*print to STDOUT*/
    if (argc == 2){
        outfile = 1;
    }

    for(i=0; i < 256; i++){
        if(list[i]){
            ch_count++;
        }
    }
    number_of_chars = calloc(1, sizeof(uint32_t));
    number_of_chars[0] = ch_count;
    size = find_size(infile);
    filecontents = malloc(size + 1);
    filecontents = read_file(infile, filecontents, size);

    write(outfile, number_of_chars, 4);
    if(ch_count > 0){
        bytes = 4;
        bytes += write_header(outfile, list);
        if (ch_count > 1){
            binary = get_bin(filecontents, size, list);

            hex = malloc(8);
            bytes = convert_to_hex(binary, hex);
            write(outfile, hex, bytes);
        }
        close(infile);
        close(outfile);
    }
    return 0;
}
