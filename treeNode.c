#include "treeNode.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

// Function to create a new node
Node_t *createNode(int common_bits, char *prefix, Node_t *branchA,
                   Node_t *branchB, data_t *data, int isLeaf, int count) {
  // Allocate memory for a new node
  Node_t *node = (Node_t *)myMalloc(sizeof(Node_t));

  // Assign values to the properties of the node
  node->common_bits = common_bits;
  node->prefix = prefix;
  node->branchA = branchA;
  node->branchB = branchB;
  node->data = data;
  node->isLeaf = isLeaf;
  node->count = count;

  return node;
}

// Function to set a node's isLeaf property based on its children
void setLeaf(int *isLeaf, Node_t *node) {
  // If both children of the node are NULL, set isLeaf to 1
  if (node->branchA == NULL && node->branchB == NULL) {
    *isLeaf = 1;
    return;
  }
  // If either child of the node is NULL, set isLeaf to 1
  else if (node->branchA == NULL || node->branchB == NULL) {
    *isLeaf = 1;
    return;
  }
  // Otherwise, set isLeaf to 0
  *isLeaf = 0;
}

// Function to free a node in a radix tree.
void freeNode(Node_t *node) {
  free(node->prefix);
  if (node->isLeaf) {
    data_t *prev = node->data;
    data_t *next = prev->next;
    // Loop through data and free them
    for (int i = 0; i < node->count; i++) {
      freeData(prev);
      prev = next;
      if (i != node->count - 1) {
        next = next->next;
      }
    }
  }
  free(node);
}
