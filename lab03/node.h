#ifndef NODE
#define NODE


typedef struct Node{
    char ch;
    int freq;
    struct Node *next;
    struct Node *left;
    struct Node *right;
} Node;

#endif
