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



int main(int argc, char *argv[]){

    Node *tree;
    int i,encoded, decoded;
    off_t size;
    uint8_t *file;
    uint32_t first;
    encoded = open(argv[1], O_RDONLY);
    tree = get_codes(argv[1]);
    size = find_size(encoded);
    file = malloc(size + 1);
    file = read_file(encoded, file, size);
    
    for(i=0; i < size; i++){
        printf("%02x",file[i]);
    }
    printf("\n");
    return 0;
}
