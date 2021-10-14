#include "../Memory/mem_man.h"
#include "../Core/hash.h"
#include "../Core/regex_if.h"
#include <stdio.h>
#include <stdlib.h>

int main(){

    // First test, if we call free without asking for any nodes we shouldnt get a segfault
    delete_all();
    printf("[PASS] Freeing nothing doesnt cause segfaults!\n");

    // ask for a single IF node
    struct IF_Regex *if_tmp = new_ifr();
    if (if_tmp) printf("[PASS] new_IFR returns valid address\n");
    else printf("[FAIL] new_IFR didnt return valid address\n");

    // remember the address of the ifr node before freeing it. Then, when we
    // re-alloc, check to see if the first address out is the same, showing
    // the stack data structure is working
    struct IF_Regex *if_mem = if_tmp;

    // try to return the node
    free_ifr(&if_tmp);
    if (!if_tmp) printf("[PASS] IF node returned successfully\n");
    else printf("[FAIL] IF node return failed\n");

    // ask for one node back to see if FIFO is preserved
    if_tmp = new_ifr();
    // check to see if the stack FIFO data structure is working as expected
    if (if_mem == if_tmp) printf("[PASS] IFR FIFO structure is being maintained\n");
    else printf("[FAIL] IFR FIFO structure is not maintained!\n");

    // re-return the node
    free_ifr(&if_tmp);

    // try to delete all alloc'd nodes, successful if no seg fault
    delete_ifr();
    printf("[PASS] One chunk of IFR returned to system memory successfully\n");

    // attempt to allocate 1 chunk + 1 of IF nodes
    struct IF_Regex *if_arr[CHUNK_SIZE+1];
    for(int i = 0; i < (CHUNK_SIZE+1); i++){
        if_arr[i] = new_ifr();
    }
    int success = 1;
    for(int i = 0; i < (CHUNK_SIZE+1); i++){
        if (!if_arr[i]){
            success = 0;
            break;
        }
    }
    if (success) printf("[PASS] Successfully recieved over 1 chunk of IF nodes\n");
    else printf("[FAIL] Failed to alloc over 1 chunk of IF nodes\n");


    // try to return all of the nodes
    for (int i = 0; i < (CHUNK_SIZE+1); i++) free_ifr(&(if_arr[i]));
    success = 1;
    for(int i = 0; i < (CHUNK_SIZE+1); i++){
        if (if_arr[i]){
            success = 0;
            break;
        }
    }
    if (success) printf("[PASS] Successfully returned over 1 chunk of IF nodes\n");
    else printf("[FAIL] Failed to return over 1 chunk of IF nodes\n");

    // Repeat for HTD
    // ask for a single htd node
    struct Hash_Datum *htd_tmp = new_htd();
    if (htd_tmp) printf("[PASS] new_HTD returns valid address\n");
    else printf("[FAIL] new_HTD didnt return valid address\n");

    // remember the address of the ifr node before freeing it. Then, when we
    // re-alloc, check to see if the first address out is the same, showing
    // the stack data structure is working
    struct Hash_Datum *htd_mem = htd_tmp;

    // try to return the node
    free_htd(&htd_tmp);
    if (!htd_tmp) printf("[PASS] HTD node returned successfully\n");
    else printf("[FAIL] HTD node return failed\n");

    // ask for one node back to see if FIFO is preserved
    htd_tmp = new_htd();
    // check to see if the stack FIFO data structure is working as expected
    if (htd_mem == htd_tmp) printf("[PASS] HTD FIFO structure is being maintained\n");
    else printf("[FAIL] HTD FIFO structure is not maintained!\n");

    // re-return the node
    free_htd(&htd_tmp);

    // try to delete all alloc'd nodes, successful if no seg fault
    delete_htd();
    printf("[PASS] One chunk of HTD returned to system memory successfully\n");

    // attempt to allocate 1 chunk + 1 of IF nodes
    struct Hash_Datum *htd_arr[CHUNK_SIZE+1];
    for(int i = 0; i < (CHUNK_SIZE+1); i++){
        htd_arr[i] = new_htd();
    }
    success = 1;
    for(int i = 0; i < (CHUNK_SIZE+1); i++){
        if (!htd_arr[i]){
            success = 0;
            break;
        }
    }
    if (success) printf("[PASS] Successfully recieved over 1 chunk of HTD nodes\n");
    else printf("[FAIL] Failed to alloc over 1 chunk of HTD nodes\n");

    // try to return all of the nodes
    for (int i = 0; i < (CHUNK_SIZE+1); i++) free_htd(&(htd_arr[i]));
    success = 1;
    for(int i = 0; i < (CHUNK_SIZE+1); i++){
        if (htd_arr[i]){
            success = 0;
            break;
        }
    }
    if (success) printf("[PASS] Successfully returned over 1 chunk of HTD nodes\n");
    else printf("[FAIL] Failed to return over 1 chunk of HTD nodes\n");

    // free all the memory
    delete_all();
    printf("TESTS CONCLUDED. All memory freed, check with valgrind\n");


    return 0;
}
