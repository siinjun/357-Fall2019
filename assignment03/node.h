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

#endif
