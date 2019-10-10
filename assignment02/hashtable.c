#include<string.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct Index{

    char *word;
    unsigned long appearances;

}Index;

typedef struct HashTable{

    unsigned long size;
    unsigned long items;
    Index *array;

}HashTable;

HashTable *create_ht(unsigned long size){

    HashTable *table;
    Index *indices;

    if (size < 1){
        perror("invalid size");
        exit(EXIT_FAILURE);
    }

    table = malloc(sizeof(HashTable));


    indices = calloc((size_t)size, sizeof(Index));

    table -> array = indices;
    table -> size = size;
    table -> items = 0;

    return table;

}

unsigned long hash(char *word, unsigned long size){

    int i = 0;
    unsigned long hash = 0;

    while(word && word[i]){
        hash = (hash + word[i]) % size;
        i++;
    }
    return hash;

}

long double load_factor(HashTable *ht){
    /*cast unsigned long to long double to perform division*/
    long double num_items = (long double)ht->items;
    long double ht_size = (long double)ht -> size;
    
    long double lf = num_items / ht_size;

    return lf;
}

HashTable *resize(HashTable *ht){

    HashTable *old = ht;
    HashTable *new;
    Index tmp;
    int i;
    int index;
    unsigned long new_size = old->size * 2;

    new = create_ht(new_size);

    for (i = 0 ; i < old->size; i++){
        /*if an index exists in the old hash table*/
        if (old->array[i].word){
            /*assign the old index to the temp Index*/
            tmp = old->array[i];
            index = hash(tmp.word, new_size);

            /*if index isn't filled*/
            if (!new->array[index % new_size].word){
                printf("placing '%s' in index of new table %ld\n", tmp.word, index % new_size);
                new->array[index % new_size] = tmp;
            }
            /*if it's occupied*/
            else{
                while(new->array[index % new_size].word){
                    index++;
                }
                printf("placing '%s' in index of new table %ld\n", tmp.word, index % new_size);
                new->array[index % new_size] = tmp;
            }
        }

    }

    /*free the old hashtable*/
    free(old);
    return new;
}

HashTable *insert_ht(HashTable *ht, char *word){

    int index;
    Index tmp;
    double lf;
    unsigned long size = ht->size;

    index = hash(word, size);
    
    tmp.word = word;
    tmp.appearances = 1;
    /*index is occupied*/
    if (ht->array[index].word != NULL){
        /*loop until spot is found*/
        while (1){
            /*new word after finding NULL*/
            if (ht->array[index%size].word == NULL){
                ht->array[index%size] = tmp;
                ht->items += 1;
                break;
            }
            /*word is already in hashtable*/
            else if (strcmp(ht->array[index%size].word, word) == 0){
                ht->array[index%size].appearances += 1;
                break;
            }
            index++;
        }
    }
    /*new word in new spot, no linear rehashing*/
    else{
        ht->array[index] = tmp;
        ht->items += 1;
    }

    lf = load_factor(ht);
    if (lf > 0.75){
        printf("time to resize\n");
        ht = resize(ht);
    }

    return ht;
}

