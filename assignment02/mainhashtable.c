int main(int argc, char **argv){                                                
                                                                                
    HashTable *ht;                                                              
                                                                                
    printf("the size of hashtable is %ld\n", sizeof(HashTable));                
    ht = create_ht(10);                                                         
                                                                                
    ht = insert_ht(ht, "oops\0");                                               
    ht = insert_ht(ht, "ops\0");                                                
    ht = insert_ht(ht, "os\0");                                                 
    ht = insert_ht(ht, "op\0");                                                 
    ht = insert_ht(ht, "ps\0");                                                 
    ht = insert_ht(ht, "aops\0");                                               
    ht = insert_ht(ht, "hi\0");                                                 
    ht = insert_ht(ht, "hit\0");                                                
    ht = insert_ht(ht, "shit\0");                                               
    ht = insert_ht(ht, "hit\0");                                                
    ht = insert_ht(ht, "hgit\0");                                               
    ht = insert_ht(ht, "hait\0");                                               
                                                                                
    printf("the size of ht is %ld\n", ht->size);                                
    int i;                                                                      
    for (i = 0; i < ht->size; i++)                                              
        printf("%s at index %d of hash with %ld appearances\n", ht->array[i].word, i, ht->array[i].appearances);
    return 0;                                                                   
}             
