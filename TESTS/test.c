#include <stdlib.h>
#include <stdio.h>
#include "regex_mm.h"
#include "regex.h"



int main(){
 
    struct IF_Regex *foo = new_node();

    foo->token = 42;
    printf("%d\n", foo->token);

    free_node(&foo);

    foo = new_node();

    printf("%d\n", foo->token);

    free_mem();

    return 0;

}
