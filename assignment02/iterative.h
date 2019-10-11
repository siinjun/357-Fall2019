#ifndef ITERATIVE
#define ITERATIVE
#include "hashtable.h"

void swap(Index a, Index b){
    Index tmp = a;
    a = b;
    b = tmp;
}

unsigned long partition_i(Index *array, unsigned long start, unsigned long end){

    unsigned long pivot = array[start].appearances;
    unsigned long i = start - 1;
    int j;

    for(j=start; j < end; ++j){
        if(array[j].appearances <= pivot){
            swap(array[j], array[pIndex]);
            pIndex++;
        }

    }
    swap(array[pIndex], array[end]);
    return pIndex;

}

void quickSortIterative(HashTable *ht, unsigned long start, unsigned long end){

    unsigned long *stack;
    unsigned long p;
    int top = -1;

    stack = calloc(ht->size, sizeof(unsigned long));
    stack[++top] = start;
    stack[++top] = end;

    while(top>=0){
        end = stack[top--];
        start = stack[top--];

        p = partition_i(ht->array, start, end);

        if (p - 1 > start){
            stack[++top] = start;
            stack[++top] = p - 1;
        }
        
        if ( p + 1 < end){
            stack[++top] = p + 1;
            stack[++top] = end;

        }

    }

}

#endif
