#include "../mem_man.h"
#include "../hash.h"
#include "../regex_if.h"
#include <stdio.h>

int main(){

    // First test, if we call free without asking for any nodes we shouldnt get a segfault
    delete_all();
    printf("[PASS] Freeing nothing doesnt cause segfaults!\n");

    // ask for a single IF node
    struct IF_regex *if_tmp = new_IFR();
    if (if_tmp) printf("[PASS] new_IFR returns valid address\n");
    else printf("[FAIL] new_IFR didnt return valid address\n");

    // try to return the node
    free_IFR(&if_tmp);
    if (!if_tmp) printf("[PASS] IF node returned successfully\n");
    else printf("[FAIL] IF node return failed\n");

    // attempt to allocate 1 chunk + 1 of IF nodes
    struct IF_Regex *tf_arr[CHUNK_SIZE+1];
    for(int i = 0; i < (CHUNK_SIZE+1); i++)
        tf_arr[i] = new_IFR();
    int success = 1;
    for(int i = 0; i < (CHUNK_SIZE+1); i++){
        if (!tf_arr[i]){
            success = 0;
            break;
        }
    }
    if (success) printf("[PASS] Successfully recieved over 1 chunk of IF nodes\n");

    return 0;
}
