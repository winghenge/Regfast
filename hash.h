// Travis Seevers, september 2021
// A closed/chaining hash table to be used for a symtable, can be edited for other uses
// This replaces using an AVL tree for a symtable, since we can acheive ~O(1) times with 
// a quality has table implimentation and only  O(logn) for AVL trees, at the expense of added and
// wasted memory. Which, on modern systems can be considered to be near limitless for our
// aplications.

#ifndef HASH_H
#define HASH_H

// Errors:
// trying to insert a key that already exists
#define E_KEY_COLL      1

// The Hash Datum struct will represent the actual data stored in the hash table as a key:value
// pair, and chained as linked lists
struct Hash_Datum{

    // remember the hash of this datum. wasting ~8 bytes per datum seems worthwhile versus
    // recalculating the hash in a case of a resize event
    unsigned int hash;

    // the pointer to a C style string that contains the actual key to our hash. In our usecase this
    // will comprise the identifiers.
    char *key;

    // We are going to have a closed hash table, which is going to nesessitate chaining datums
    // together. We will represent this as a linked list, and thus need a next pointer.
    struct Hash_Datum *next;

    // Alright, so a hash table is fairly worthless without the value to pair with the key. 
    // will use a void * to allow more flexable use of this library. user is
    // responsible for memory management of the value.
    void *val;

};

// The Hash Bucket struct will comprise the spine of the hash table, with the chained data being
// pointed to initialy from a hash bucket. Each bucket can keep track of how many elements are
// chained to it, so when the index is empty, it can quickly be identified as such, and for
// load-balancing, wthe width/depth of the table can quickly be seen without having to itterate
// through each chain.
struct Hash_Bucket{

    // how many elements are currently chained to this bucket
    unsigned int depth;

    // the next/first datum in the chain
    struct Hash_Datum *next;

};

// A pretty wrapper for our hash table. contains the basic information about the table: how many
// buckets, how much data. this information may be used later to load-balance the table to optimize
// access times when the table files
struct Hash_Table{
    
    // how many buckets: the width of the table
    unsigned int width;

    // the pointer to the start of the table
    struct Hash_Bucket *table;

};

// Our table create, insert, remove, and delete functions
struct Hash_Table *init_ht();
void delete_ht(struct Hash_Table **table);
int insert_ht(struct Hash_Table *table, char *key, void *value);
void remove_ht(struct Hash_Table *table, char *key);

// the lookup function to return the stored value
void *lookup_ht(struct Hash_Table *table, char *key);

unsigned int hash(char *key);

#endif // HASH_H
