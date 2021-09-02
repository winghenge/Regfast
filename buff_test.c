#include <stdio.h>
#include <stdlib.h>
#include "file_buff.h"

int main(){
    // open the file and init the buffer
    printf("OPEN_BUFFER: %d\n", open_buffer("./test.txt"));

    // try to print 32 characters (the size of the test buffer)
    for(int i = 0; i < 100; i++) printf("%c", get_c());

    // include the newline
    printf("\n");

    // reverse 5 chars
    for (int i = 0; i < 5; i++) unget_c();

    // get the next 5 chars again
    for(int i = 0; i < 5; i++) printf("%c", get_c());

    // include the newline
    printf("\n\n");

    // read until EOF
    char c = get_c();
    while(c != '\0'){
        printf("%c", c);
        c = get_c();
    }

    printf("\n");
    



    // close the buffer/file
    close_buffer();
    
    return 0;
}
