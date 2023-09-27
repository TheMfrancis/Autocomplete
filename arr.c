#include "arr.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


// Function to compare two trading_names of a data_t struct
int cmpfunc(const void *a, const void *b) {
  return strcmp((*(data_t **)a)->trading_name, (*(data_t **)b)->trading_name);
}

// Function to build a sorted array from a file
data_t **buildSortedArr(FILE *f, data_t **arr, int *len, int *maxLen) {
  int n = 0;
  // Gets rid of the header line from the CSV file
  fscanf(f, "%*[^\n]\n");
  char line[MAX_LINE_LEN];
  while (fgets(line, MAX_LINE_LEN, f) != NULL) {
    // Check if arr reaches its maxLen and reallocs memory if it does
    if (n == *maxLen) {
      *maxLen = *maxLen + *maxLen;
      arr = (data_t **)realloc(arr, (*maxLen) * sizeof(data_t *));
    }
    // Malloc space for a new element and get its data
    arr[n] = (data_t *)myMalloc(sizeof(data_t));
    arr[n]->index = n;
    getData(line, arr[n]);
    n++;
  }
  *len = n;
  return arr;
}

// Function to free the memory allocated for the sorted array
void freeSortedArr(data_t **arr, int len) {
  // Loop through array and free data
  for (int i = 0; i < len; i++) {
    freeData(arr[i]);
  }
  free(arr);
}

// Function to perform a linear search on the array
void linearSearch(data_t **arr, int len, int index, char *key, int *cComps,
                  int *sComps, int *start, int *end) {
  // The two ifs below handle edge cases
  if (index == len - 1) {
    *end = len;
  }
  if (index == 0) {
    *start = 0;
  }
  // Loop through left of index until the prefix does not match the key
  for (int i = index + 1; i < len; i++) {
    int comparisons = 0;
    int equality = strcmp_count(key, arr[i]->trading_name, &comparisons);
    // Increment string and char comparisons also
    (*sComps)++;
    *cComps += comparisons;
    if (equality != 0) {
      *end = i;
      break;
    }
  }
  // Loop through right of index until the prefix does not match the key
  for (int i = index - 1; i >= 0; i--) {
    int comparisons = 0;
    int equality = strcmp_count(key, arr[i]->trading_name, &comparisons);
    // Increment string and char comparisons also
    (*sComps)++;
    *cComps += comparisons;
    if (equality != 0) {
      *start = i + 1;
      break;
    }
  }
}

// Function to print the keys that match a given key
void printMatchingKeys(data_t **arr, char *key, FILE *f, int start, int end) {
  // Loop through each array element and print it
  for (int i = start; i < end; i++) {
    printData(f, arr[i]);
  }
}

// Function to perform a binary search on the array
int binSearch(data_t **arr, int left, int right, char *key, int *comps,
              int *sComps) {
  int mid = left + (right - left) / 2;
  // Base case if element not found
  if (left > right) {
    return NOT_FOUND;
  }
  int equality = strcmp_count(key, arr[mid]->trading_name, comps);
  (*sComps)++;
  // Cut off top half if mid element > key
  if (equality < 0) {
    return binSearch(arr, left, mid - 1, key, comps, sComps);
  }
  // Cut off bottom half if mid element < key
  else if (equality > 0) {
    return binSearch(arr, mid + 1, right, key, comps, sComps);
  }
  // Else return index of element;
  else {
    return mid;
  }
}
