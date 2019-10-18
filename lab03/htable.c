#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include "linkedlist.h"
#include "node.h"

#define SIZE 8192


Node *swap(Node *swapped, Node *new_node){

    new_node->next = swapped;

    return new_node;

}

Node *insert(Node *head, Node *new_node){
    
    if(!head){
        return new_node;
    }
    else if(!head->next){
        if(head->freq < new_node->freq){
            head->next = new_node;
        }
        else
            head = swap(head, new_node);
    }
    else{
        Node *tmp;
        tmp = head;
        if (new_node->freq < head->freq){
            new_node->next = head;
        }
        else{
            bool swapped = false;
            while(tmp->next){
                if(new_node->freq < tmp->next->freq){
                    tmp->next = swap(tmp->next, new_node);
                    swapped = true;
                    break;
                }/*
                else if (new_node->freq == tmp->next->freq){

                }*/
                tmp = tmp->next;
            }
            if(!swapped){
                tmp->next = new_node;
            }
        }
    }   
    return head;
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

int main(int argc, char *argv[]){
    
    char *buf;
    int num, fd;
    off_t size;
    Node **list;
    Node *linked_list = NULL;
    int i = 0;
    int index;

    fd = open(argv[1], O_RDONLY);
    size = find_size(argv[1], fd);

    list = malloc(255 * sizeof(Node));

    buf = malloc(size);

    while((num = read(fd, buf, size)) != 0){}
    while(buf[i] != '\0'){
        Node *new;
        new = malloc(sizeof(Node));
        index = buf[i];
        if (!list[index]){
            new -> freq = 1;
            new -> ch = index;
            list[index] = new;
        }
        else
            list[index]->freq += 1;
        i++;
    }

    for(i=0; i < 255; i++){
        if(list[i]){
            linked_list = insert(linked_list, list[i]);
        }
    }

    while(linked_list){
        printf("ch: %c, freq: %d\n", linked_list->ch, linked_list->freq);
        linked_list = linked_list->next;
    }

    close(fd);
    return 0;
}
