#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include "node.h"
#include "tree.c"

int main(int argc, char *argv[]){

    Node *tree;
    int i;

    tree = get_codes(argv[1]);

    return 0;
}
