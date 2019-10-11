#ifndef SELECTION_SORT
#define SELECTION_SORT
#include "hashtable.h"


void selection_sort(HashTable *table){
    unsigned long index, position;
    Index tmp;

    for(index = table->size - 1; index > 0; index--){
        unsigned long max_val = 0;
        for(position = 1; position < index + 1; position++){
            if(table->array[position].word && 
        table->array[position].appearances > table->array[max_val].appearances){
                max_val = position;
            }
        }
        tmp = table->array[index];
        table->array[index] = table->array[max_val];
        table->array[max_val] = tmp;
    }
}

#endif
