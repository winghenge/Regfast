#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

int main(){
    struct AVL_Node *r = NULL;

    insert_node(&r, 10);
    insert_node(&r, 5);
    insert_node(&r, 7);

    if(!r) printf("NULL\n");

    print_tree(r, 0);



    printf("Freed: %d\n", free_mem(&r));

    return 0;
}
