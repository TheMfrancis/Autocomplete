#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <stdlib.h>

#include "data.h"

#define MAX_BITS 128 * 8

// Define a structure for a node in a binary tree
typedef struct Node Node_t;
struct Node {
  int common_bits;  // The number of common bits in the prefix
  char *prefix;     // The prefix of the node
  Node_t *branchA;  // Pointer to the left child node
  Node_t *branchB;  // Pointer to the right child node
  data_t *data;     // Pointer to the data stored in the node
  int isLeaf;       // Flag to check if the node is a leaf node
  int count;        // The count of data elements in the node
};

// Function to create a new node
Node_t *createNode(int common_bits, char *prefix, Node_t *branchA,
                   Node_t *branchB, data_t *data, int isLeaf, int count);

// Function to set a node's isLeaf property based on its children
void setLeaf(int *isLeaf, Node_t *node);

// Function to free a node in a radix tree.
void freeNode(Node_t *node);

#endif
