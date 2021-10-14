#include "../Core/regex_if.h"
#include "../Core/hash.h"
#include "struct_mem.h"
#include <stdio.h>
#include <stdlib.h>

// alright, Im just re-writing mem_man to see if I can find the issue
// IFR memory structures
struct Chunk_Node_IFR{
    struct IF_Regex *head;
    struct Chunk_Node_IFR *next;
};

struct Chunk_Wad_IFR{
    struct Chunk_Node_IFR *nodes;
    struct IF_Regex *next;
} ifr_wad = {NULL, NULL};

// HTD memory structures
struct Chunk_Node_HTD{
    struct Hash_Datum *head;
    struct Chunk_Node_HTD *next;
};

struct Chunk_Wad_HTD{
    struct Chunk_Node_HTD *nodes;
    struct Hash_Datum *next;
} htd_wad = {NULL, NULL};

int alloc_chunk_ifr(){

    // check to ensure that the ifr wad is indeed out of nodes
    if (ifr_wad.next) return -1;

    // get a new ifr chunk node
    struct Chunk_Node_IFR *cn = (struct Chunk_Node_IFR *)malloc(sizeof(struct Chunk_Node_IFR));
    
    // chain the cn's in the wad
    cn->next = ifr_wad.nodes;
    ifr_wad.nodes = cn;

    // alloc a new chunk of nodes
    cn->head = (struct IF_Regex *)malloc(sizeof(struct IF_Regex) * CHUNK_SIZE);

    // built a LL out of the newly alloc'd nodes
    for (int i = 0; i < CHUNK_SIZE-1; i++) cn->head[i].down = &(cn->head[i+1]);
    cn->head[CHUNK_SIZE-1].down = NULL;

    // have the ifr wad point to the head of out new LL
    ifr_wad.next = cn->head;

    // return success
    return 1;
}

int alloc_chunk_htd(){
    // check to ensure that the htd wad is indeed out of nodes
    if (htd_wad.next) return -1;

    // get a new ifr chunk node
    struct Chunk_Node_HTD *cn = (struct Chunk_Node_HTD *)malloc(sizeof(struct Chunk_Node_HTD));

    // chain the cn's in the wad
    cn->next = htd_wad.nodes;
    htd_wad.nodes = cn;

    // alloc a new chunk of nodes
    cn->head = (struct Hash_Datum *)malloc(sizeof(struct Hash_Datum) * CHUNK_SIZE);

    // built a LL out of the newly alloc'd nodes
    for (int i = 0; i < CHUNK_SIZE-1; i++) cn->head[i].next = &(cn->head[i+1]);
    cn->head[CHUNK_SIZE-1].next = NULL;

    // have the htd wad point to the head of out new LL
    htd_wad.next = cn->head;

    // return success
    return 1;
}

struct IF_Regex *new_ifr(){
    
    // if the wad is out of nodes, get more
    if (!ifr_wad.next) alloc_chunk_ifr();

    // grab the next free node, then update the LL
    struct IF_Regex *ret = ifr_wad.next;
    ifr_wad.next = ret->down;

    return ret;
}

struct Hash_Datum *new_htd(){

    // if the wad is out of nodes, get more
    if (!htd_wad.next) alloc_chunk_htd();

    // grab the next free node, then update the LL
    struct Hash_Datum *ret = htd_wad.next;
    htd_wad.next = ret->next;

    return ret;
}

void free_ifr(struct IF_Regex **node){
    // cleanup the node
    (*node)->up = NULL;
    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->modifier = 0;
    (*node)->symbol = '\0';
    (*node)->token = -1;

    // add back to the wad's ll
    (*node)->down = ifr_wad.next;
    ifr_wad.next = (*node);

    // clear the pointer
    *node = NULL;

}

void free_htd(struct Hash_Datum **node){

    // cleanup the node
    (*node)->key = NULL;
    (*node)->hash = 0;
    (*node)->val = NULL;

    // add back to the wad's ll
    (*node)->next = htd_wad.next;
    htd_wad.next = (*node);

    // clear the pointer
    *node = NULL;
}

void delete_ifr(){
    // free all of the alloc'd memory for IFR nodes
    while(1){
        struct Chunk_Node_IFR *tmp = ifr_wad.nodes;
        
        if (!tmp) break;

        ifr_wad.nodes = tmp->next;

        free(tmp->head);
        free(tmp);
    }

    ifr_wad.nodes = NULL;
    ifr_wad.next = NULL;

}

void delete_htd(){

    // free all of the alloc'd memory for htd nodes
    while(1){
        struct Chunk_Node_HTD *tmp = htd_wad.nodes;

        if (!tmp) break;

        htd_wad.nodes = tmp->next;

        free(tmp->head);
        free(tmp);
    }

    htd_wad.nodes = NULL;
    htd_wad.next = NULL;
}

void delete_all(){
    delete_ifr();
    delete_htd();
}


