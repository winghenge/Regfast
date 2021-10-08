// copyright 2021 Travis Seevers
//
// Memory management function for the regex intermediate format structure
// allows for the allocation of large chunk of structures from the kernel for
// time optimization

#include "regex.h"

#ifndef REGFAST_MM_H
#define REGFAST_MM_H

#define CHUNK_SIZE (256)

//
// General functions
//
// Delete all the maloc'd melmory and return it to the kernel
void delete_all();

//
// Intermediate Formate Regex (IFR) functions
//
// get a new IFR node from the stack
struct IF_Regex *new_IFR();
// return an IFR node to the stack
void free_IFR(struct IF_Regex **node);
//delete all the IFR malloc'd memory back to the kernel
void delete_IFR();

//
// Hash Tabel Datum (HTD) functions
//
// get a new HTD from the stack
struct Hash_Datum *new_HTD();
// return a HTD to the stack
void free_HTD(struct Hash_Datum **node);
// delete all of the HTD malloc'd memory and return it to the kernel
void delete_HTD();

#endif //REGFAST_MM_H
