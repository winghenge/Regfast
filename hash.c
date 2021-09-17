#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// Memory managment presets
#define DATUM_CHUNK     512 // how many datum structs to malloc at once

// Hash table presets
#define WIDTH_DEFAULT   64  // Width

// Optimisation presets
#define LOAD_BALANCE    70  // defined as a % ratio of depth/width. 

// First, we will deal with memory management optimizations. 
// We're going to perform a similar task as was done with the regex_mm library: grab a large initial
// chunk of memory from the kernel, then use it as needed. We can run under the assumption that very
// few hash tables will be made, and that growing the table will be a rare occurance, so we will
// focus our attentions to optimizing the memory allotment of the Hash_Datum structs

// takes a pointer to a hash_table struct and adds a new chunk of datums to the reserve
void alloc_chunk(struct Hash_Table *table){
    // assert that the table isnt NULL
    if (!table) return;

    // Ask the kernel for some memory space for a new Hash_Mem struct, then insert it into the
    // table's linked list 
    struct Hash_Mem *tmp_mem = (struct Hash_Mem *)malloc(sizeof(struct Hash_Mem));
    tmp_mem->next = table->datum_mem;
    table->datum_mem = tmp_mem;

    // Grab a chunk of memory for datums and store it in the new memory structure
    tmp_mem->head = (struct Hash_Datum *)malloc(sizeof(struct Hash_Datum) * DATUM_CHUNK);

    // Chain together the chunk of new datums into a linked list
    for (int i = 0; i < (DATUM_CHUNK-1); i++)
        tmp_mem->head[i].next = &(tmp_mem->head[i+1]);

    // NULL terminate the list
    tmp_mem->head[DATUM_CHUNK-1].next = NULL;

    // Add the new chunk to the reserves
    tmp_mem->head[DATUM_CHUNK-1].next = table->reserve;
    table->reserve = tmp_mem->head;

    return;
}

// Creates an empty hash table
struct Hash_Table *init_ht(){
    // allocate memory for the table struct
    struct Hash_Table *table = (struct Hash_Table *)malloc(sizeof(struct Hash_Table));

    // set the default width
    table->width = WIDTH_DEFAULT;

    // ensure that the reserve and data_mem pointers are NULL
    table->reserve = NULL;
    table->datum_mem = NULL;

    // allocate a chunk of datums
    alloc_chunk(table);

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
        hash += *key * (*(++key)+1);
    }

    return hash;
}

// In a few of these functions, we need to first check to see if the the key alread exists in the
// index
// if the key exists, return the address of that datum, otherwise NULL
struct Hash_Datum *key_exists(struct Hash_Datum *root, char *key){
    
    while(1){

        if (!root) return NULL;
        if (!strcmp(root->key, key)) return root;

        root = root->next;
    }

}

int insert_ht(struct Hash_Table *table, char *key, int value){

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
    if (key_exists(table->table[index].next, key)) return -E_KEY_COLL;

    // Alright, grab the next datum struct from the reserve and populate it
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
    return;
}

struct Hash_Datum *lookup_ht(struct Hash_Table *table, char *key){
    
    // try to lookup the datum thats linked to that key
    return 0;
}
