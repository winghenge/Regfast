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
#define PTR_SIZE    (sizeof(void *))

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
    
    printf("ACIFR 1\n");

    // declare a new Alloc_Head struct to store the info from this new alloc
    struct Alloc_Head *mem = (struct Alloc_Head *)malloc(sizeof(struct Alloc_Head));
    // make it the new root of the mem_root list of the passed memory node
    printf("ACIFR 2\n");
    mem->next = ifr_root.a_h;
    printf("ACIFR 3\n");
    ifr_root.a_h = mem;

    // allocate a new chunk of IF_Regex nodes
    mem->head = (void *)malloc(sizeof(struct IF_Regex) * CHUNK_SIZE);
    printf(">>%p\n", mem->head);
    printf("ACIFR 4\n");

    // temp pointer for building the LL
    struct IF_Regex *tmp = mem->head;
    printf("ACIFR 5\n");

    // update the next struct in the memory node
/*    for (int i = 0; i < CHUNK_SIZE - 1; i++){
        tmp->down = (tmp + PTR_SIZE);
        tmp += PTR_SIZE;
    }
    */
    printf("ACIFR 6\n");

    ifr_root.next = (void *)mem->head;

    printf("ACIFR 7\n");

    free(mem->head);
    printf("ACIFR DEL\n");
}

void alloc_chunk_HTD(){

    // declare a new Alloc_Head struct to store the info from this new alloc
    struct Alloc_Head *mem = (struct Alloc_Head *)malloc(sizeof(struct Alloc_Head));
    // make it the new root of the mem_root list of the passed memory node
    mem->next = htd_root.a_h;
    htd_root.a_h = mem;

    // allocate a new chunk of Hash Datum nodes
    mem->head = (void *)malloc(sizeof(struct Hash_Datum) * CHUNK_SIZE);

    // temp pointer for building the LL
    struct Hash_Datum *tmp = mem->head;

    // update the next struct in the memory node
    for (int i = 0; i < CHUNK_SIZE - 1; i++){
        tmp->next = (tmp + PTR_SIZE);
        tmp += PTR_SIZE;
    }

    htd_root.next = (void *)mem->head;

}

// return the address of a free IF_Regex struct to the stack
struct IF_Regex *new_IFR(){

    printf("NIFR 1\n");

    // if the free_list is empty, alloc a new chunk
    if (!ifr_root.next) alloc_chunk_IFR();

    printf("NIFR 2\n");
    // Grab the next free node
    struct IF_Regex *ret = (struct IF_Regex *)ifr_root.next;

    printf("NIFR 3\n");
    // update the LL
    ifr_root.next = (void *)ret->down;

    printf("NIFR 4\n");
    // clear the node
    ret->down = NULL;

    printf("NIFR 5\n");
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
        printf("DIFR 1\n");
        // isolate the alloc head to free
        struct Alloc_Head *cur = ifr_root.a_h;
        printf("DIFR 2\n");

        // check to see if its non-NULL, if its null we are done
        if (!cur) break;
        printf("DIFR 3\n");

        // remember the next alloc struct
        ifr_root.a_h = cur->next;
        printf("DIFR 4\n");

        printf("<<%p\n", cur->head);

        // free the IF-regex chunk
        free(cur->head);
        printf("DIFR 5\n");

        // free the mem root
        free(cur);
        printf("DIFR 6\n");

    }

    // set the LL pointed by ifr_root to NULL to prevent handing out non-alloc'd address
    ifr_root.next = NULL;
    ifr_root.a_h = NULL;

}

// free all of the HTD memory
void delete_HTD(){

    while(1){
        
        // isolate the alloc head to free
        struct Alloc_Head *cur = htd_root.a_h;

        // check to see if its non-NULL, if its null we are done
        if (!cur) break;

        // remember the next alloc struct
        htd_root.a_h = cur->next;

        // free the HTD chunk
        free(cur->head);

        // free the mem root
        free(cur);

    }

    // make sure the root struct is ready to be re-used
    htd_root.next = NULL;
    htd_root.a_h = NULL;
}

// free all of the allocated memory
void delete_all(){
    
    // Just call all of the structure specific free's
    delete_IFR();
    delete_HTD();

}

