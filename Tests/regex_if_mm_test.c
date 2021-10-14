// copyright 2021 Travis Seevers
// tests the regex_if and regex_mm, regex intermediate format and memory 
// management, files

#include <stdlib.h>
#include <stdio.h>
#include "../Memory/mem_man.h"
#include "../Core/regex_if.h"



int main(){
    
    // try to get a regex_if node.
    struct IF_Regex *foo = new_node();
    if (foo) printf("[PASS] Regex MM allocated first node\n");
    else printf("[FAIL] Regex MM failed on first call\n");

    // Try to assign the node's token value. default is -1
    foo->token = 42;
    if (foo->token == 42) printf("[PASS] Token value changed successfully\n");
    else printf("[FAIL] Token value not changed\n");

    // remember the address of this first node
    // this is for future test
    struct IF_Regex *foo_old = foo;

    // Free the node and add it back to the reserve
    free_node(&foo);
    if (!foo) printf("[PASS] Regex IF structure freed back to MM!\n");
    else printf("[FAIL] Regex IF structure not correctly freed to MM!\n");


    // Re-allocate the same structure
    foo = new_node();
    // based on how the memory management works, stack based MM so that the
    // freed foo will be the next struct alloc'd
    // foo should be the same address it was pre-free, but with a token value 
    // of -1
    if (foo == foo_old) printf("[PASS] Re-Allcoed struct has the same address!\n");
    else printf("[FAIL Memory management stack not working as intended\n");

    if (foo->token == -1) printf("[PASS] Re-Alloced struct has blanked token\n");
    else printf("[FAIL] Re-Alloced struct has non-default token value\n");

    // CHUNKSIZE is defined by regex_mm.h
    // when one chunk of memory is used, a new chunk should be allocated
    // so lets try to grab CHUNK_SIZE+1 nodes. 
    //
    // we've alreay alloced Foo, so we just need 1 chunksize more
    for (int i = 0; i < CHUNK_SIZE; i++)
        foo = new_node();

    // check to see if we segfaulted/have a null node
    if (foo) printf("[PASS] Regex MM allocated oveer 1 chunk of nodes\n");
    else printf("[FAIL] Regex MM failed to allocate over 1 chunk of nodes\n");

    // free the memory, no way to test in file. can test with valgrind
    free_mem();

    return 0;

}
