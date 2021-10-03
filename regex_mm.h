// copyright 2021 Travis Seevers
//
// Memory management function for the regex intermediate format structure
// allows for the allocation of large chunk of structures from the kernel for
// time optimization

#include "regex.h"

#ifndef REGEX_MM_H
#define REGEX_MM_H

#define CHUNK_SIZE (256)

struct IF_Regex *new_node();
void free_node(struct IF_Regex **node);
void free_mem();

#endif //REGEX_MM_H
