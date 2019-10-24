#ifndef NODE
#define NODE


typedef struct Node{
    char ch;
    char *code;
    int freq;
    struct Node *next;
    struct Node *left;
    struct Node *right;
} Node;

Node *get_codes(char *filename);

#endif
