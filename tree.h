#ifndef TREE_H
#define TREE_H

#include <stdlib.h>

#include "data.h"
#include "stringUtils.h"
#include "treeNode.h"

// Function to find the node where a new node should be inserted in a radix tree
Node_t *findInsertionNode(Node_t *root, char *pref, data_t *data);

// Function to create a radix tree starting with data for the root
Node_t *createTree(data_t *data);

// Function to free a node and all its descendants in a radix tree
void freeTree(Node_t *node);

// Function to insert a new node into a radix tree
void insert(Node_t *root, char *pref, char *commonPrefix, data_t *data);

// Function to print all entries in a radix tree to a file
void printEntries(Node_t *root, FILE *f);

// Function to search for a key in a radix tree and print all matching entries
// to a file
void search(Node_t *root, char *key, int *cComps, int *bComps, FILE *f);

// Function that builds the prefix tree
Node_t *buildTree(FILE *inFile);

#endif 
