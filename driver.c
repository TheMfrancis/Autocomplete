#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "arr.h"
#include "tree.h"
#include "treeNode.h"

// Function dict2 reads data from an input file, sorts it, and then performs a
// binary search followed by a linear search to find matching keys. It prints
// the matching keys and the number of comparisons made.
void dict2(FILE *inFile, FILE *outFile) {
  // Allocate memory for an array of data_t pointers
  data_t **arr = myMalloc(INITIAL * sizeof(data_t *));
  int len, charComp, strComp, start, end, maxLen = INITIAL;
  // Build a sorted array from the input file
  arr = buildSortedArr(inFile, arr, &len, &maxLen);
  // Sort the array using quicksort
  qsort(arr, len, sizeof(data_t *), cmpfunc);
  char tradingName[MAX_FIELD_LEN];
  // Read trading names from standard input until EOF
  while (scanf("%[^\n]\n", tradingName) == 1) {
    charComp = 0;
    strComp = 0;
    fprintf(outFile, "%s\n", tradingName);
    // Perform a binary search for the trading name in the array
    int index = binSearch(arr, 0, len - 1, tradingName, &charComp, &strComp);
    // Perform a linear search for the trading name in the array around the
    // index found
    linearSearch(arr, len, index, tradingName, &charComp, &strComp, &start,
                 &end);
    // Print all keys in the array that match the trading name to the output
    // file
    printMatchingKeys(arr, tradingName, outFile, start, end);
    // Print the trading name and the number of comparisons made to standard
    // output
    printf("%s --> b%d c%d s%d\n", tradingName, charComp * BYTE_SIZE, charComp,
           strComp);
  }
  // Free the memory allocated for the array
  freeSortedArr(arr, len);
}

// Function dict3 builds a binary tree from an input file and then performs a
// search for each line read from standard input. It prints the matching keys
// and the number of comparisons made.
void dict3(FILE *inFile, FILE *outFile) {
  // Build a binary tree from the input file
  Node_t *root = buildTree(inFile);
  char line[MAX_LINE_LEN];
  // Read lines from standard input until EOF
  while (scanf("%[^\n]\n", line) == 1) {
    int cComps = 0;
    int bComps = 0;
    // Print the line to the output file
    fprintf(outFile, "%s\n", line);
    // Convert the line to binary
    char *binaryKey = string_to_binary(line);
    // Search for the binary key in the tree
    search(root, binaryKey, &cComps, &bComps, outFile);
    // Print the line and the number of comparisons made to standard output
    printf("%s --> b%d c%d s1\n", line, bComps, cComps);
    // Free the memory allocated for the binary key
    free(binaryKey);
  }
  // Free the memory allocated for the tree
  freeTree(root);
}

// The main function of the program opens input and output files, calls either
// dict2 or dict3 based on command line arguments, and then closes the files.
int main(int argc, char **argv) {
  // Open the input and output files
  FILE *inFile = safeOpen(argv[2], "r");
  FILE *outFile = safeOpen(argv[3], "w");
  if (!strcmp(argv[1], "2")) {
    dict2(inFile, outFile);
  } else if (!strcmp(argv[1], "3")) {
    dict3(inFile, outFile);
  }
  fclose(inFile);   // Close input file
  fclose(outFile);  // Close output file
  return 0;
}
