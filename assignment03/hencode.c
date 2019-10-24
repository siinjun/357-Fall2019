#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include "tree.c"
#include "node.h"

void write_to_file(int outfilei, char *filecontents, off_t size){


}


int main(int argc, char *argv[]){

    Node **list;
    int i, infile, outfile;
    uint32_t ch_count = 0;
    uint32_t *bigger_data;
    off_t size;
    char *filecontents;

    list = get_list(argv[1]);
    printf("%s\n", argv[2]);
    infile = open(argv[1], O_RDONLY);
    /*creat(argv[2], );*/
    outfile = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    printf("outfile = %d\n", outfile);


    for(i=0; i < 256; i++){
        if(list[i]){
            ch_count++;
        }
    }
    bigger_data = calloc(5, sizeof(uint32_t));
    bigger_data[0] = ch_count;
    size = find_size(infile);
    filecontents = malloc(size + 1);
    filecontents = read_file(infile, filecontents, size);

    write(outfile, bigger_data, 4);
    
    return 0;
}
