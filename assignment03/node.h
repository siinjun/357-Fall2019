#ifndef NODE
#define NODE


typedef struct Node{
    unsigned char ch;
    char *code;
    int freq;
    struct Node *next;
    struct Node *left;
    struct Node *right;
} Node;
/*global vars to share*/
unsigned char *file_cont;
Node *glob_tree;
off_t file_size;
#endif
