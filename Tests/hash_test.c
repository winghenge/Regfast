#include <stdlib.h>
#include <stdio.h>
#include "../Core/hash.h"
#include "../Memory/mem_man.h"

int main(){
    // test the hash function
    printf("====================\n");
    printf("==Hash Function=====\n");
    printf("====================\n");
    printf("A: %d, %d\n", hash("A"), hash("A")%10);
    printf("B: %d, %d\n", hash("B"), hash("B")%10);
    printf("C: %d, %d\n", hash("C"), hash("C")%10);
    printf("Hello: %d, %d\n", hash("Hello"), hash("Hello")%10);
    printf("hello: %d, %d\n", hash("hello"), hash("hello")%10);
    printf("helol: %d, %d\n", hash("helol"), hash("helol")%10);
    printf("Bob: %d, %d\n", hash("Bob"), hash("Bob")%10);

    // test the table
    // try to create the table
    struct Hash_Table *table = init_ht();

    // try to insert a key:value pair
    printf("====================\n");
    printf("==Insert============\n");
    printf("====================\n");
    int bob = 42;
    printf("Inserting bob: %d\n", insert_ht(table, "bob", (void *)(&bob)));
    int alice = 420;
    printf("Inserting alice: %d\n", insert_ht(table, "alice", (void *)(&alice)));
    int max = 69;
    printf("Inserting max: %d\n", insert_ht(table, "max", (void *)(&max)));
    printf("Inserting bob: %d\n", insert_ht(table, "bob", (void *)(&bob)));

    // try to lookup bob and alice
    printf("====================\n");
    printf("==Lookup============\n");
    printf("====================\n");
    void *val = lookup_ht(table, "bob");
    if (val) printf("[Pass] bob found! value %d\n", *((int *)val));
    else printf("[FAIL] bob not found\n");

    val = lookup_ht(table, "alice");
    if (val) printf("[PASS] alice found! value %d\n", *((int *)val));
    else printf("[FAIL] alice not found\n");

    val = lookup_ht(table, "boo");
    if (val) printf("[FAIL] boo found! value %d\n", *((int *)val));
    else printf("[PASS] boo not found\n");


    // Try to remove bob
    printf("====================\n");
    printf("==Remove bob========\n");
    printf("====================\n");
    remove_ht(table, "bob");
    val = lookup_ht(table, "bob");
    if (val) printf("[FAIL] bob found! value %d\n", *((int *)val));
    else printf("[PASS] bob not found\n");

    // Try to delete the table
    printf("====================\n");
    printf("==Free Table Mem====\n");
    printf("====================\n");
    delete_ht(&table);
    if (table) printf("[FAIL] Table not freed!\n");
    else printf("[PASS] Table freed!\n");

    // free memory assosiated with mem_man
    delete_all();



    return 0;
}
