#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include "node.h"

char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup){
        strcpy(dup, str);
    }
    return dup;
}

Node *swap(Node *swapped, Node *new_node){

    new_node->next = swapped;

    return new_node;

}

Node *insert(Node *head, Node *new_node){
    
    if(!head){
        return new_node;
    }/*
    else if(!head->next){
        if(head->freq < new_node->freq){
            head->next = new_node;
        }
        else
            head = swap(head, new_node);
    }*/
    else{
        Node *tmp;
        tmp = head;
        if (new_node->freq < head->freq){
            new_node->next = head;
            return new_node;
        }
        else{
            bool swapped = false;
            while(tmp->next){
                if(new_node->freq < tmp->next->freq){
                    tmp->next = swap(tmp->next, new_node);
                    swapped = true;
                    break;
                }
                else if (new_node->freq == tmp->next->freq){
                    if(new_node->ch < tmp->next->ch){
                        tmp->next = swap(tmp->next, new_node);
                        swapped = true; 
                        break;
                    }
                }
                tmp = tmp->next;
            }
            if(!swapped){
                tmp->next = new_node;
            }
        }
    }   
    return head;
}

Node *create_linked_list(Node **list){

    int i;
    Node *linked_list = NULL;

    for(i=0; i < 255; i++){
        if(list[i]){
            linked_list = insert(linked_list, list[i]);
        }
    }
    return linked_list;
}


off_t find_size(char *filename, int fd){

    off_t curr_pos, size;

    /*find beginning pos of file*/
    curr_pos = lseek(fd, 0, SEEK_CUR);
    /*find size of file*/
    size = lseek(fd, 0, SEEK_END);
    /*go back to beginning*/
    lseek(fd, curr_pos, SEEK_SET);

    return size;

}

char *read_file(int fd, char *buf, off_t size){
    int num;

    while((num = read(fd, buf, size)) != 0){}
    return buf;
}

Node **create_table(char *buf, Node **list){
    int index, i = 0;

    while(buf[i] != '\0'){
        Node *new;
        new = malloc(sizeof(Node));
        index = buf[i];
        if (!list[index]){
            new -> left = NULL;
            new -> right = NULL;
            new -> next = NULL;
            new -> code = NULL;
            new -> freq = 1;
            new -> ch = index;
            list[index] = new;
        }
        else{
            list[index]->freq += 1;
            free(new);
        }
        i++;
    }
    return list;

}

Node *create_supernode(Node *first, Node *second){

    Node *supernode;

    supernode = malloc(sizeof(Node));
    supernode -> freq = first->freq + second->freq;
    first -> next = NULL;
    second -> next = NULL;
    supernode -> ch = 0;
    supernode -> next = NULL;
    supernode -> code = NULL;
    supernode -> left = first;
    supernode -> right = second;

    return supernode;
}

Node *create_tree(Node *linkedlist){
    
    while(linkedlist->next){
        Node *supernode;
        Node *one = linkedlist;
        Node *two = linkedlist->next;
        /*reset head to next available node or null*/
        if (linkedlist->next->next)
            linkedlist = linkedlist->next->next;
        else
            linkedlist = NULL;
        supernode = create_supernode(one, two);
        linkedlist = insert(linkedlist, supernode);
    }
    return linkedlist;
}

void assign_codes(Node *htree, int strlen, char *code){

    char *tmp;
    strlen++;
    tmp  = strdup(code);
    tmp = realloc(tmp, strlen);
    if(htree->left){
        tmp[strlen-1] = '0';
        assign_codes(htree->left, strlen, tmp);
    }
    if(htree->right){
        tmp[strlen-1] = '1';
        assign_codes(htree->right, strlen, tmp);
    }
    if(htree->ch){
        htree->code = tmp;
    }
    /*free(tmp);*/
    /*free(code);*/
}



void free_tree(Node *htree){

    if(htree->left)
        free_tree(htree->left);
    if(htree->right)
        free_tree(htree->right);
    if(htree->code)
        free(htree->code);
    free(htree);

}


int main(int argc, char *argv[]){
    
    char *buf, *code;
    int fd, i;
    off_t size;
    Node **list;
    Node *linked_list = NULL;
    Node *tree;

    list = calloc(255, sizeof(Node));

    fd = open(argv[1], O_RDONLY);

    size = find_size(argv[1], fd);

    buf = malloc(size);

    buf = read_file(fd, buf, size);

    list = create_table(buf, list);

    linked_list = create_linked_list(list);
/*    tmp = linked_list;
    
    while(tmp){
        printf("ch: %c, freq: %d\n", tmp->ch, tmp->freq);
        tmp = tmp->next;
    }
*/
    tree = create_tree(linked_list);
    code = malloc(1);
    if (!code){
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    assign_codes(tree, 0, code);
    for(i=0; i < 255; i++){
        if(list[i]){
            printf("0x%02x ('%c'): %s\n", list[i]->ch,list[i]->ch, list[i]->code);
        }
    }
    free(list);
    free(buf);
    free(code);

    free_tree(tree);
    close(fd);
    return 0;
}
