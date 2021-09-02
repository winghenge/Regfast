#include "file_buff.h"
#include <stdio.h>
#include <stdlib.h>

// Displacements on the buffer array to common positions
#define FIRST_BUFF_START  (0)
#define FIRST_BUFF_END    (BUFF_SIZE)
#define SECOND_BUFF_START (BUFF_SIZE+1)
#define SECOND_BUFF_END   (2*BUFF_SIZE+1)

// alright, so file IO a single character at a time can become computationally
// expensive, uspeccialy with lookaheads when we will need to un-get a character
// so we will have a two-stage sentinal buffer
// (See Dragon book, pg 91)
struct Buffer{
    char *buff;
    char *lex_start;
    char *forward;
    FILE *fd;
    int load;
} *file_buffer = NULL;

// read in BUFF_SIZE chars from the file into buff_start
void load_buff(char *buff_start){
    // read in the first buff_size chars
    // since fgets reads in n-1 chars, with the nth being a \0
    // we want to specify N+1, then set the N+1th to EOF as a sentinal
    fgets(buff_start, BUFF_SIZE+1, file_buffer->fd);

    return;
}

// open the file at specified path, returns 1 on success, 0 on fopen failure
// and -1 if the buffer `file_buffer` != NULL
int open_buffer(char *path){
    // check to see if file_buffer is null, if it is, quit
    if (file_buffer) return -1;

    // init the buffer's memory
    file_buffer = (struct Buffer *)malloc(sizeof(struct Buffer));

    // open the file for R/W, save the pointer in file_buffer
    file_buffer->fd = fopen(path, "r");

    // check to see if fd is null
    if (!file_buffer->fd) return 0;

    // otherwise we were successful! setup the buffer now
    
    // allocate memory for the buffer
    // the buffer is composed of two halfs, each half of BUFF_SIZE+1
    // with the +1 for the EOF sentinal characters that allow us to use 1 
    // fewer check per incriment (only need to check if at end of array when
    // encountering a sentinal char, not on every incriment, since we'd
    // always check for EOF
    file_buffer->buff = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1) *2);

    // populate the buffer
    // the first half
    load_buff(file_buffer->buff);

    // set the lex_start and forward pointers to the first char in the buffer
    file_buffer->lex_start = file_buffer->buff;
    file_buffer->forward = file_buffer->buff;

    // set load to true
    file_buffer->load = 1;
    
    // return 1 to indicate success
    return 1;
}

// free all of the memoyr assosiated with the buffer
void close_buffer(){
    //: make sure that the file_buffer isnt null
    if (!file_buffer) return;

    // free the buffer
    free(file_buffer->buff);

    // close the file
    fclose(file_buffer->fd);

    // free the file_buffer itself
    free(file_buffer);

    // set the pointer to null
    file_buffer = NULL;

    return;
}

char get_c(){
    // save the current forward char
    char ret = *file_buffer->forward;
    
    // see if we are at a sentinal
    if (ret == '\0'){
        // see if this is a sentinal or a true EOF
        //first, see if we are at the end of the first half 
        if (file_buffer->forward == (file_buffer->buff + FIRST_BUFF_END)){
            // reload the second half 
            if (file_buffer->load) 
                load_buff(file_buffer->buff+SECOND_BUFF_START);

            // point towards the start of the second half
            file_buffer->forward = file_buffer->buff+SECOND_BUFF_START;

            // re-get the return character
            ret = *file_buffer->forward;

            // reset the load flag
            file_buffer->load = 1;
            
        }
        // else, check to see if we are at the end of the second half
        else if (file_buffer->forward == (file_buffer->buff + SECOND_BUFF_END)){
            // reload the second half
            if (file_buffer->load) 
                load_buff(file_buffer->buff);

            // point towards the start of the second half
            file_buffer->forward = file_buffer->buff;
            
            // re-get the return character
            ret = *file_buffer->forward;

            // reset the load flag
            file_buffer->load = 1;

        }
    }

    // incriment the forward pointer
    file_buffer->forward += 1;


    // return
    return ret;
}

void unget_c(){
    // Figure out if we are at the start of a half
    switch(file_buffer->forward - file_buffer->buff){
        case 0:
            // we are at the start of the first half, switch to the end of the 
            // second half
            file_buffer->forward = file_buffer->buff + SECOND_BUFF_END - 1;

            // set the load flag to false so we dont re-load the buffer
            file_buffer->load = 0;
            break;
        case (BUFF_SIZE+1):
            // we are at the start of the second half, switch to the end
            // of the first half
            file_buffer->forward = file_buffer->buff + FIRST_BUFF_END - 1;

            // set the load flag to false so we dont re-load the buffer
            file_buffer->load = 0;
            break;
        default:
            // just decriment the pointer
            file_buffer->forward -= 1;
            break;
    }

    return;
}
