#include "myUtils.h"

#include <assert.h>
#include <stdlib.h>

// This function allocates memory for an array of nitems elements, each of size
// bytes, and returns a pointer to the allocated memory. The memory is set to
// zero.
void* myCalloc(int nitems, int size) {
  void* pt = calloc(nitems, size);
  assert(pt);
  return pt;
}

// This function allocates size bytes and returns a pointer to the allocated
// memory.
void* myMalloc(int size) {
  void* pt = malloc(size);
  assert(pt);
  return pt;
}

// This function opens the file whose name is specified in the parameter
// filename and does an action to it whether it be read or write, etc.
FILE* safeOpen(char* filename, char* action) {
  FILE* file = fopen(filename, action);
  assert(file);
  return file;
}
