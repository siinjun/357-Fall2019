#ifndef QUICKSORT
#define QUICKSORT

#include "hashtable.h"
#include<stdbool.h>


unsigned long partition(HashTable *table, unsigned long start, 
                        unsigned long end){
    
    unsigned long pIndex;
    int i;
    Index pivot;
    Index tmp;
    bool iszero = false;
    
    pivot = table->array[end];
    pIndex = start;
    for(i = start; i<end; i++){
        if(table->array[end].appearances == 0){
            iszero = true;
            break;
        }
        if(table->array[i].appearances >= pivot.appearances){
            if(table->array[i].appearances == pivot.appearances){
                if(table->array[i].word){
                    int cmp = strcmp(table->array[i].word, pivot.word);
                    if (cmp > 0){
                        Index tmp = table->array[i];
                        table->array[i] = table->array[pIndex];
                        table->array[pIndex] = tmp;
                        pIndex++;
                    }
                }
            }
            else{
                Index tmp = table->array[i];
                table->array[i] = table->array[pIndex];
                table->array[pIndex] = tmp;
                pIndex++;
            }
        }
    }
    if(!iszero){
        tmp = table->array[pIndex];
        table->array[pIndex] = table->array[end];
        table->array[end] = tmp;
    }
    else{
        pIndex = end;
    }
    return pIndex;
}

void quickSort(HashTable *table, int start, int end){

    unsigned long index;

    if (start >= end){
        return;
    }
    if (start < end){
        index = partition(table, start, end);
        quickSort(table, start, index - 1);
        quickSort(table, index + 1, end);
    }
}


#endif
