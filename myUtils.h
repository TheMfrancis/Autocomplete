#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>

// This function allocates size bytes and returns a pointer to the allocated
// memory.
void* myMalloc(int size);

// This function allocates memory for an array of nitems elements, each of size
// bytes, and returns a pointer to the allocated memory. The memory is set to
// zero.
void* myCalloc(int nitems, int size);

// This function opens the file whose name is specified in the parameter
// filename and does an action to it whether it be read or write, etc.
FILE* safeOpen(char* filename, char* action);

#endif
