#include <stdlib.h>
#include <stdio.h>
#include "../hash.h"

int main(){
    struct Hash_Datum *tmp_datum;

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
    printf("Inserting bob: %d\n", insert_ht(table, "bob", 42));
    printf("Inserting alice: %d\n", insert_ht(table, "alice", 420));
    printf("Inserting max: %d\n", insert_ht(table, "max", 69));
    printf("Inserting bob: %d\n", insert_ht(table, "bob", 8008));

    // try to lookup bob and alice
    printf("====================\n");
    printf("==Lookup============\n");
    printf("====================\n");
    tmp_datum = lookup_ht(table, "bob");
    if (tmp_datum) printf("bob found! value %d\n", tmp_datum->val);
    else printf("bob not found\n");

    tmp_datum = lookup_ht(table, "alice");
    if (tmp_datum) printf("alice found! value %d\n", tmp_datum->val);
    else printf("alice not found\n");

    tmp_datum = lookup_ht(table, "boo");
    if (tmp_datum) printf("boo found! value %d\n", tmp_datum->val);
    else printf("boo not found\n");


    // Try to remove bob
    printf("====================\n");
    printf("==Remove bob========\n");
    printf("====================\n");
    remove_ht(table, "bob");
    tmp_datum = lookup_ht(table, "bob");
    if (tmp_datum) printf("bob found! value %d\n", tmp_datum->val);
    else printf("bob not found\n");

    // Try to delete the table
    printf("====================\n");
    printf("==Remove bob========\n");
    printf("====================\n");
    delete_ht(&table);
    if (table) printf("Table not freed!\n");
    else printf("Table freed!\n");



    return 0;
}
