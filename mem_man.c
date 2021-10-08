#include "regex_if.h"
#include "hash.h"
#include "mem_man.h"
#include <stdio.h>
#include <stdlib.h>

// Sizeof is a compile-time oeprator, thus, by having repeated calls to it wont bog down performance
// durring run-time. Thus, below are the definitions of each structure size, using sizeof, for
// memmory offset calcualtions
#define IFR_SIZE    (sizeof(struct IF_Regex))
#define HTD_SIZE    (sizeof(struct Hash_Datum))

// Everytime we grab a chunk of memory for some RegFast struct
// we need to keep track of the returned address from malloc
// since only that address can be freed to return all of the memory
// back to the system.
// It will be easist to handle this as a linked list,
struct Alloc_Head{
    void *head;
    struct Alloc_Head *next;
};

// Since we will be handling multiple structure types, for all of Regfast, lets encapsulate
// the memory alloc_head LL, and the free lists into a nice, portable struct
struct Memory_Node{
    struct Alloc_Head *a_h;
    void *next;
};
    

// We need to keep track of the Memory Node structs for each type of struct we handle
// IF_Regex structures:
struct Memory_Node ifr_root = {NULL, NULL};
// Hash Datums
struct Memory_Node htd_root = {NULL, NULL};


// grab a chunk of memory from the head and assemble the free list from it
// we need one per struct supported
void alloc_chunk_IFR(){

    // declare a new Alloc_Head struct to store the info from this new alloc
    struct Alloc_Head *mem = (struct Alloc_Head *)malloc(sizeof(struct Alloc_Head));
    // make it the new root of the mem_root list of the passed memory node
    mem->next = ifr_root.a_h;
    ifr_root.a_h = mem;

    // allocate a new chunk of IF_Regex nodes
    mem->head = (void *)malloc(sizeof(struct IF_Regex) * CHUNK_SIZE);

    // update the next struct in the memory node
    for (int i = 0; i < CHUNK_SIZE - 1; i++){
        ((struct IF_Regex *)mem->head)->down = (struct IF_Regex *)(mem->head + IFR_SIZE);
        mem->head += IFR_SIZE;
    }

    ifr_root.next = (void *)mem->head;

}

void alloc_chunk_HTD(){

    // declare a new Alloc_Head struct to store the info from this new alloc
    struct Alloc_Head *mem = (struct Alloc_Head *)malloc(sizeof(struct Alloc_Head));
    // make it the new root of the mem_root list of the passed memory node
    mem->next = htd_root.a_h;
    htd_root.a_h = mem;

    // allocate a new chunk of Hash Datum nodes
    mem->head = (void *)malloc(sizeof(struct Hash_Datum) * CHUNK_SIZE);

    // update the next struct in the memory node
    for (int i = 0; i < CHUNK_SIZE - 1; i++){
        ((struct Hash_Datum *)mem->head)->next = (struct Hash_Datum *)(mem->head + HTD_SIZE);
        mem->head += HTD_SIZE;
    }

    htd_root.next = (void *)mem->head;

}

// return the address of a free IF_Regex struct to the stack
struct IF_Regex *new_IFR(){

    // if the free_list is empty, alloc a new chunk
    if (!ifr_root.next) alloc_chunk_IFR();

    // Grab the next free node
    struct IF_Regex *ret = (struct IF_Regex *)ifr_root.next;

    // update the LL
    ifr_root.next = (void *)ret->down;

    // clear the node
    ret->down = NULL;

    // return the new node
    return ret;
}

// return the address of a free Hash_Datum struct to the stack
struct Hash_Datum *new_HTD(){

    // if the free_list is empty, alloc a new chunk
    if (!htd_root.next) alloc_chunk_HTD();

    // Grab the next free node
    struct Hash_Datum *ret = (struct Hash_Datum *)htd_root.next;

    // update the LL
    htd_root.next = (void *)ret->next;

    // clear the node
    ret->next = NULL;

    // return the new node
    return ret;
}

// clear a node and add it back to the free list
void free_IFR(struct IF_Regex **node){

    // clear all of the node's feilds
    (*node)->up = NULL;
    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->modifier = 0;
    (*node)->symbol = '\0';
    (*node)->token = -1;
    
    // add the node back to the free list
    (*node)->down = (struct IF_Regex *)ifr_root.next;
    ifr_root.next = (void *)*node;

    // clear this pointer to prevent an accidental usage
    *node = NULL;

    return;
}

// clear a node and add it back to the free list
void free_HTD(struct Hash_Datum **node){

    // clear all of the node's feilds
    (*node)->next = NULL;
    
    // add the node back to the free list
    (*node)->next = (struct Hash_Datum *)htd_root.next;
    htd_root.next = (void *)*node;

    // clear this pointer to prevent an accidental usage
    *node = NULL;

    return;
}

// free all of the IFR memory
void delete_IFR(){

    while(1){
        
        // isolate the alloc head to free
        struct Alloc_Head *cur = ifr_root.a_h;

        // check to see if its non-NULL, if its null we are done
        if (!cur) break;

        // free the if_regex chunk
        free(cur->head);

        // remember the next alloc struct
        ifr_root.a_h = cur->next;

        // free the mem root
        free(cur);

    }
}

// free all of the HTD memory
void delete_HTD(){

    while(1){
        
        // isolate the alloc head to free
        struct Alloc_Head *cur = htd_root.a_h;

        // check to see if its non-NULL, if its null we are done
        if (!cur) break;

        // free the if_regex chunk
        free(cur->head);

        // remember the next alloc struct
        htd_root.a_h = cur->next;

        // free the mem root
        free(cur);

    }
}

// free all of the allocated memory
void delete_all(){
    
    // Just call all of the structure specific free's
    delete_IFR();
    delete_HTD();

}

