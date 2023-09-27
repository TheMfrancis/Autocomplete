#include "tree.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "stringUtils.h"
#include "treeNode.h"
#define SPACE "00100000"

// Function that builds the prefix tree
Node_t *buildTree(FILE *f) {
  Node_t *root;
  // Gets rid of the header line from the CSV file
  fscanf(f, "%*[^\n]\n");
  char line[MAX_LINE_LEN];
  int first = 1;
  // Then get data for each establishment while the file still has lines to read
  while (fgets(line, MAX_LINE_LEN, f) != NULL) {
    data_t *data = (data_t *)myMalloc(sizeof(data_t));
    getData(line, data);
    if (first) {
      root = createTree(data);
      first = 0;
    } else {
      char *binaryName = string_to_binary(data->trading_name);
      findInsertionNode(root, binaryName, data);
      free(binaryName);
    }
  }
  return root;
}

// Function to find the node where a new node should be inserted in a radix tree
Node_t *findInsertionNode(Node_t *root, char *pref, data_t *data) {
  // Create a buffer to store the common prefix
  char commonPrefix[MAX_BITS];
  // Find the common prefix between the root's prefix and the new prefix
  findCommonPrefix(root->prefix, pref, commonPrefix);
  // Create a buffer to store the first 8 characters of the common prefix
  char first8CF[BYTE_SIZE + 1];
  get_first_8_chars(commonPrefix, first8CF);
  // If the first 8 characters of the common prefix is a space
  if (strcmp(first8CF, SPACE)) {
    // Create a buffer to store the last byte of the common prefix
    char lastByte[BYTE_SIZE + 1];
    // Cut off the last byte from the common prefix
    cut_last_n_chars(commonPrefix, lastByte,
                     strlen(commonPrefix) % BYTE_SIZE + BYTE_SIZE);
    // Check if the last byte is a space
    if (!strcmp(lastByte, SPACE)) {
      // Recursively find the insertion node for the root's prefix and data
      findInsertionNode(root, root->prefix, root->data);
      root->data = NULL;
    }
  }
  // Create a dummy variable for strcmp_count
  int dummy = 0;
  // If the common prefix is equal to pref but not equal to root's prefix
  if (strcmp(commonPrefix, pref) == 0 && strcmp(root->prefix, pref) != 0) {
    // Create a buffer to store the initial root's prefix
    char initialRootPrefix[MAX_BITS];
    strcpy(initialRootPrefix, root->prefix);
    free(root->prefix);
    root->prefix = strdup(commonPrefix);
    root->common_bits = strlen(root->prefix);
    root->count = 1;
    // Recursively find the insertion node for initialRootPrefix and root's data
    findInsertionNode(root, initialRootPrefix, root->data);
    root->data = data;
    return root;
  }
  // If commonPrefix is not equal to root's prefix
  else if (strcmp(commonPrefix, root->prefix)) {
    // Insert a new node with pref, commonPrefix and data into root
    insert(root, pref, commonPrefix, data);
    return root;
  }
  // If pref is equal to commonPrefix
  else if (!strcmp(pref, commonPrefix)) {
    // Get a pointer to the last data item in root's data list
    data_t *p = root->data;
    for (int i = 0; i < root->count - 1; i++) {
      p = p->next;
    }
    // Append data to the end of root's data list
    p->next = data;
    // Increment root's count by 1
    root->count++;
    return root;
  }
  // Else if pref is an extension of the root's prefix
  else if (!strcmp_count(commonPrefix, pref, &dummy)) {
    // Insert a new node with pref, commonPrefix and data into root
    insert(root, pref, commonPrefix, data);
    return root;
  }
  // Create a buffer to store newPref which is pref without its common part with
  // commonPrefix
  char newPref[MAX_BITS];
  cutString(commonPrefix, pref, newPref);
  if (newPref[FIRST_CHAR] == '0') {
    // Recursively find the insertion node in branchA with newPref and data
    return findInsertionNode(root->branchA, newPref, data);
  } else {
    // Recursively find the insertion node in branchB with newPref and data
    return findInsertionNode(root->branchB, newPref, data);
  }
}

// Function to create a radix tree starting with data for the root
Node_t *createTree(data_t *data) {
  char *binary = string_to_binary(data->trading_name);
  Node_t *root =
      createNode(strlen(binary), strdup(binary), NULL, NULL, data, 1, 1);
  free(binary);
  return root;
}

// Function to free a node and all its descendants in a radix tree
void freeTree(Node_t *node) {
  // If the node is NULL, return without doing anything
  if (node == NULL) return;
  // Recursively free the left child of the node
  freeTree(node->branchA);
  // Recursively free the right child of the node
  freeTree(node->branchB);
  // Free the node itself
  freeNode(node);
}

// Function to insert a new node into a radix tree
void insert(Node_t *root, char *pref, char *commonPrefix, data_t *data) {
  // Create a buffer to store the prefix of the first child
  char firstChildPrefix[MAX_BITS];
  // Cut the common part from root's prefix and store the result in
  // firstChildPrefix
  cutString(commonPrefix, root->prefix, firstChildPrefix);
  // Create a buffer to store the prefix of the second child
  char secondChildPrefix[MAX_BITS];
  // Cut the common part from pref and store the result in secondChildPrefix
  cutString(commonPrefix, pref, secondChildPrefix);
  free(root->prefix);
  root->prefix = strdup(commonPrefix);
  root->common_bits = strlen(root->prefix);
  int isLeaf;
  // If firstChildPrefix is an empty string, it means that root's prefix is
  // equal to commonPrefix
  if (firstChildPrefix[FIRST_CHAR] == '\0') {
    // Set root's isLeaf property based on its children
    setLeaf(&root->isLeaf, root);
    // If secondChildPrefix starts with '0', it means that we need to insert a
    // new node into branchA
    if (secondChildPrefix[FIRST_CHAR] == '0') {
      if (root->branchA == NULL) {
        root->branchA =
            createNode(strlen(secondChildPrefix), strdup(secondChildPrefix),
                       NULL, NULL, data, 1, 1);
      }
      // If branchA is not NULL, find the insertion node in branchA and insert
      // the new node there
      else {
        findInsertionNode(root->branchA, secondChildPrefix, data);
      }
    }
    // Else try to insert into branchB and repeat
    else {
      if (root->branchB == NULL) {
        root->branchB =
            createNode(strlen(secondChildPrefix), strdup(secondChildPrefix),
                       NULL, NULL, data, 1, 1);
      } else {
        findInsertionNode(root->branchB, secondChildPrefix, data);
      }
    }
  }
  // If firstChildPrefix starts with '0', it means that we need to move the
  // current node to branchA and insert a new node into branchB
  else if (firstChildPrefix[FIRST_CHAR] == '0') {
    // If branchA is NULL, move the current node to branchA
    if (root->branchA == NULL) {
      setLeaf(&isLeaf, root);
      root->branchA = createNode(
          strlen(firstChildPrefix), strdup(firstChildPrefix), root->branchA,
          root->branchB, root->data, isLeaf, root->count);
    }
    // If branchA is not NULL, create a new node with the same properties as the
    // current node and set it as branchA
    else {
      setLeaf(&isLeaf, root);
      Node_t *newNode = createNode(
          strlen(firstChildPrefix), strdup(firstChildPrefix), root->branchA,
          root->branchB, root->data, isLeaf, root->count);
      root->branchA = newNode;
    }
    root->data = NULL;
    root->branchB = NULL;
    root->branchB =
        createNode(strlen(secondChildPrefix), strdup(secondChildPrefix), NULL,
                   NULL, data, 1, 1);
    root->isLeaf = 0;
  }
  // Else try to move the current node to branchB and repeat
  else {
    if (root->branchB == NULL) {
      setLeaf(&isLeaf, root);
      root->branchB = createNode(
          strlen(firstChildPrefix), strdup(firstChildPrefix), root->branchA,
          root->branchB, root->data, isLeaf, root->count);
    } else {
      setLeaf(&isLeaf, root);
      Node_t *newNode = createNode(
          strlen(firstChildPrefix), strdup(firstChildPrefix), root->branchA,
          root->branchB, root->data, isLeaf, root->count);
      root->branchB = newNode;
    }
    root->data = NULL;
    root->branchA = NULL;
    root->branchA =
        createNode(strlen(secondChildPrefix), strdup(secondChildPrefix), NULL,
                   NULL, data, 1, 1);
    root->isLeaf = 0;
  }
}

// Function to print all entries in a radix tree to a file
void printEntries(Node_t *root, FILE *f) {
  // If the node is NULL, return without doing anything
  if (root == NULL) {
    return;
  }
  // Check if the node is a leaf and has data
  if (root->isLeaf && root->data) {
    data_t *p = root->data;
    // Loop over all data items in the node's data list
    for (int i = 0; i < root->count; i++) {
      // Print the current data item to the file
      printData(f, p);
      p = p->next;
    }
  }
  // Recursively print all entries in branchA
  printEntries(root->branchA, f);
  // Recursively print all entries in branchB
  printEntries(root->branchB, f);
}

// Function to search for a key in a radix tree and print all matching entries
// to a file
void search(Node_t *root, char *key, int *cComps, int *bComps, FILE *f) {
  int len = strlen(key);
  // Check if we have found the key
  if (root->common_bits >= len) {
    // Check if the commonPrefix is a multiple of 8
    // to prevent printing out words just because of a few similar bits
    char commonPrefix[MAX_BITS];
    findCommonPrefix(root->prefix,key,commonPrefix);
    if((*bComps+strlen(commonPrefix))%BYTE_SIZE != 0){
      return;
    }
    // Increment cComps and bComps by the length of the key
    *bComps += len;
    if (len % BYTE_SIZE == 0) {
      *cComps += len / BYTE_SIZE;
    } else {
      *cComps += len / BYTE_SIZE + 1;
    }
    // Print all entries of the node and its children to the file
    printEntries(root, f);
    return;
  }
  *bComps += root->common_bits;
  if (root->common_bits % BYTE_SIZE == 0) {
    *cComps += (root->common_bits) / BYTE_SIZE;
  } else {
    *cComps += (root->common_bits) / BYTE_SIZE + 1;
  }
  // Create a buffer to store newKey which is key without its common part with
  // root's prefix
  char newKey[MAX_BITS];
  cutString(root->prefix, key, newKey);
  // If newKey starts with '0', recursively search for newKey in branchA
  if (newKey[FIRST_CHAR] == '0') {
    search(root->branchA, newKey, cComps, bComps, f);
  }
  // If newKey starts with '1', recursively search for newKey in branchB
  else {
    search(root->branchB, newKey, cComps, bComps, f);
  }
}
