
#ifndef FILEBUFF_H
#define FILEBUFF_H

#define BUFF_SIZE (32)

int open_buffer(char *path);
void close_buffer();
char get_c();
void unget_c();

#endif
