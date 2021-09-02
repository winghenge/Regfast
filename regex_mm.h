#include "regex.h"

#ifndef REGEX_MM_H
#define REGEX_MM_H

#define CHUNK_SIZE (256)

struct IF_Regex *new_node();
void free_node(struct IF_Regex **node);
void free_mem();

#endif //REGEX_MM_H
