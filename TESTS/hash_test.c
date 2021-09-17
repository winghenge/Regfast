#include <stdlib.h>
#include <stdio.h>
#include "../hash.h"

int main(){

    // test the hash function
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
    printf("Inserting bob: %d\n", insert_ht(table, "bob", 42));
    printf("Inserting alice: %d\n", insert_ht(table, "alice", 420));
    printf("Inserting max: %d\n", insert_ht(table, "max", 69));
    printf("Inserting bob: %d\n", insert_ht(table, "bob", 8008));

    return 0;
}
