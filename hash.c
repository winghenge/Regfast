#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "mem_man.h"

// Hash table presets
#define WIDTH_DEFAULT   64  // Width

// Optimisation presets
#define LOAD_BALANCE    70  // defined as a % ratio of depth/width. 

// Creates an empty hash table
struct Hash_Table *init_ht(){
    // allocate memory for the table struct
    struct Hash_Table *table = (struct Hash_Table *)malloc(sizeof(struct Hash_Table));

    // set the default width
    table->width = WIDTH_DEFAULT;

    // ensure that the reserve and data_mem pointers are NULL
    table->reserve = NULL;
    table->datum_mem = NULL;

    // create a tabble from the bbuckers (array of buckets)
    table->table = (struct Hash_Bucket *)malloc(sizeof(struct Hash_Bucket) * WIDTH_DEFAULT);

    // set all feilds of the bucket array to 0/NULL
    for (int i = 0; i < WIDTH_DEFAULT; i++){
        table->table[i].depth = 0;
        table->table[i].next = NULL;
    }
    
    // return the created table
    return table;
}

// delete the hash table and all of its assosiative memory
void delete_ht(struct Hash_Table **table){

    // with our table implementation, the char *key doesnt need to be freed, since its being handled
    // by an optimizer (large character buffer). the value is assumed to be handled by the
    // user and thus not freed
    // All of the datums are freed back to mem_manager, and the buckets need to
    // be freed as well

    // itterate through each bucket and free all of the buckets back to mem_man
    while(1){
        // remember the next mem structure
        tmp = (*table)->datum_mem->next;

        // free the datum chunk
        free((*table)->datum_mem->head);

        // free the memory structure itself
        free((*table)->datum_mem);

        // reset the head memory structure for the next itteration
        (*table)->datum_mem = tmp;
        
        // if we are done with the memory structures, break out of this loop
        if (!tmp) break;
    }

    // now we can free the table itself!
    free(*table);
    *table = NULL;
    

    return;
}

// all of the insert, remove, and lookup functions need a hash function to generate the has from the
// key
unsigned int hash(char *key){

    // What makes a good hashing function?
    // 1) the same key will always provide the same hash
    // 2) a slight change in the key, such as one letter change, should provide a drasticly
    // different result
    // 3) its fast to calculate

    // Im thinking something along the lines of: add to the sum the value of the current char * next
    // char. heres the results of initial testings, I beleive that this will be sufficient
    /*

    Key     Hash    Index
    A:      65,     5
    B:      66,     6
    C:      67,     7
    Hello:  42332,  2
    hello:  45596,  6
    helol:  45920,  0
    Bob:    18479,  9

    */

    int hash = 0;
    while(1){
        if (!*key) break;

        // add one to the next char multiply so that null characters dont multiply by zero.
        // either way would work, but it prevents all single char keys from being zero
        hash += *key * (*(key+1)+1);
        key++;
    }

    return hash;
}

// In a few of these functions, we need to first check to see if the the key alread exists in the
// index
// if the key exists, return the address of that datum, otherwise NULL
struct Hash_Datum *key_exists(struct Hash_Datum *root, char *key, struct Hash_Datum **prev){

    // the poiter `prev` is populated with the address of the datum that points to the found key:value pair. 
    // if the address of prev is NULL, this is not done.
    // This is used to be able to remove/insert/traverse the datum chain
    // using this function, instead of having a function that performs this
    // as well.
    
    // if we are returning the prev address, set to NULL as default.
    // if NULL is returned, the Hash_Buccket was the previous
    if (prev) *prev = NULL;
    
    while(1){

        if (!root) return NULL;
        if (!strcmp(root->key, key)) return root;

        if (prev) *prev = root;

        root = root->next;
    }

}

int insert_ht(struct Hash_Table *table, char *key, void *value){

    // What we need to do:
    // 0) Check to see if the key is already in the table
    // 1) grab next datum structure from the chunk
    // 2) populate it and calculate the hash
    // 3) From the hash, calculate the index
    // 4) Insert into the chain
    // 5) TODO: check the hash table loading and optimize if nessisary

    // generate the hash -> index
    unsigned int index = hash(key) % table->width;

    // check to see if the key already exists in the table
    // if it does, return an exit code
    if (key_exists(table->table[index].next, key, NULL)) return -E_KEY_COLL;

    // Alright, grab the next datum struct from the reserve and populate it
    // First, if the reserve is empty, refill it
    if (!table->reserve) alloc_chunk(table);

    struct Hash_Datum *datum = table->reserve;
    table->reserve = datum->next;

    // populate
    datum->hash = hash(key);
    datum->key = key;
    datum->val = value;

    // insert into the chain
    table->table[index].depth++;
    datum->next = table->table[index].next;
    table->table[index].next = datum;

    // TODO: table optimizations

    // return without error
    return 0;
}

void remove_ht(struct Hash_Table *table, char *key){
    // Call key exists to:
    // A) see if the key:datum pair exists
    // B) get the address of the key:datum pair
    // C) get the address of the previous pair

    // get the index of the key
    unsigned int index = hash(key) % table->width;

    // perform the lookup
    struct Hash_Datum *prev;
    struct Hash_Datum *p_datum = key_exists(table->table[index].next, key, &prev);


    // if the key isnt in the table, we can return now
    if (!p_datum) return;

    // Alright, we need to delete the datum
    // first, we need to remove it from the chain
    // if prev == NULL, the bucket is the previous
    if (prev) prev->next = p_datum->next;
    else table->table[index].next = p_datum->next;

    // now we can insert this datum back into the reserve chain
    p_datum->next = table->reserve;
    table->reserve = p_datum;


    return;
}

// return the pointer to the value for the key:value pair
void *lookup_ht(struct Hash_Table *table, char *key){
    
    // try to lookup the datum thats linked to that key
    struct Hash_Datum *tmp = key_exists(table->table[hash(key) % table->width].next, key, NULL);

    // if tmp != NULL (the value exists) return the pointer, otherwise 
    // return NULL
    return (tmp) ? tmp->val : NULL;
}
