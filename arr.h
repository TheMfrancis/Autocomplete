#ifndef _DICT_H_  // If _DICT_H_ is not defined then compile the following code
#define _DICT_H_  // Define _DICT_H_

#include "data.h"

// Constant definitions
#define INITIAL 100
#define NOT_FOUND -1

// Function prototypes
// Function to perform a linear search on the array
void linearSearch(data_t **arr, int len, int index, char *key, int *cComps,
                  int *sComps, int *start, int *end);

// Function to print the keys that match a given key
void printMatchingKeys(data_t **arr, char *key, FILE *f, int start, int end);

// Function to compare two trading_names of a data_t struct
int cmpfunc(const void *a, const void *b);

// Function to build a sorted array from a file
data_t **buildSortedArr(FILE *f, data_t **arr, int *len, int *maxLen);

// Function to free the memory allocated for the sorted array
void freeSortedArr(data_t **arr, int len);

// Function to perform a binary search on the array
int binSearch(data_t **arr, int left, int right, char *key, int *comps,
              int *sComps);

#endif  // End of conditional compilation
