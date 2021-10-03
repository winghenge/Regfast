#include "regex_if.h"
#include "regex_mm.h"
#include <stdio.h>
#include <stdlib.h>

// Everytime we grab a chunk of memory for IF_Regex structs
// we need to keep track of the returned address from malloc
// since only that address can be freed to return all of the memory
// back to the system.
// It will be easist to handle this as a linked list,
struct Alloc_Head{
    struct IF_Regex *head;
    struct Alloc_Head *next;
};

// We need to keep track of the alloc_head structs as a LL
struct Alloc_Head *mem_root = NULL;

// Also, we shall keep track of the regex nodes as a linked list
struct IF_Regex *free_list = NULL;

// grab a chunk of memory from the head and assemble the free list from it
void alloc_chunk(){

    // declare a new Alloc_Head struct to store the info from this new alloc
    struct Alloc_Head *mem = (struct Alloc_Head *)malloc(sizeof(struct Alloc_Head));
    // make it the new root of the mem_root list
    mem->next = mem_root;
    mem_root = mem;

    // allocate a new chunk of IF_Regex nodes
    mem->head = (struct IF_Regex *)malloc(sizeof(struct IF_Regex) * CHUNK_SIZE);

    // Now itterate through the new chunk and re-populate the free_list
    for(int i = 0; i < CHUNK_SIZE; i++){
        mem->head[i].down = free_list;
        free_list = &(mem->head[i]);
    }

}

// return the address of a free IF_Regex struct
struct IF_Regex *new_node(){

    // if the free_list is empty, alloc a new chunk
    if (!free_list) alloc_chunk();

    // Grab the next free node
    struct IF_Regex *ret = free_list;

    // update the LL
    free_list = ret->down;

    // clear the node
    ret->down = NULL;

    // return the new node
    return ret;
}

// clear a node and add it back to the free list
void free_node(struct IF_Regex **node){

    // clear all of the node's feilds
    (*node)->up = NULL;
    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->modifier = 0;
    (*node)->symbol = '\0';
    (*node)->token = -1;
    
    // add the node back to the free list
    (*node)->down = free_list;
    free_list = *node;

    // clear this pointer to prevent an accidental usage
    *node = NULL;

    return;
}

// free all of the allocated memory
void free_mem(){
    
    // itterate through the mem_root LL and free all of the memory
    struct Alloc_Head *cur = mem_root;

    // if cur == NULL return
    if (!cur) return;

    while(1){

        // free the if_regex chunk
        free(cur->head);

        // remember the next alloc struct
        cur = mem_root->next;

        // free the mem root
        free(mem_root);

        mem_root = cur;

        // if we're at the end of the list, exit
        if (!cur) break;

    }

}

