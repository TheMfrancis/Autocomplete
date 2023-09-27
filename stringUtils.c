#include "stringUtils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

// Function to cut a string from another string
void cutString(char *toCut, char *toBeCut, char *newStr) {
  int i = 0;
  char *p = toBeCut;
  // Iterate over the characters in the strings until they are different or end
  // of string is reached
  while (*toCut && (*toCut == *p)) {
    i++;
    toCut++;
    p++;
  }
  // Move the remaining characters in 'toBeCut' to 'newStr'
  memmove(newStr, p, strlen(toBeCut) - i + 1);
  newStr[strlen(toBeCut) - i + 1] = '\0';
}

// Function to find the common prefix between two strings
void findCommonPrefix(char *s1, char *s2, char *commonPref) {
  int i = 0;
  // Iterate over the characters in the strings until they are different or end
  // of string is reached
  while (s1[i] && s2[i] && s1[i] == s2[i]) {
    commonPref[i] = s1[i];
    i++;
  }
  commonPref[i] = '\0';
}

// Function to convert a character to binary
char *char_to_binary(char c) {
  char *binary = (char *)myMalloc((BYTE_SIZE + 1) * sizeof(char));
  // Convert each bit of character to binary and store it in 'binary'
  int seven = BYTE_SIZE - 1;
  for (int i = seven; i >= 0; i--) {
    binary[seven - i] = ((c >> i) & 1) + '0';
  }
  binary[BYTE_SIZE] = '\0';
  return binary;
}

// Function to convert a string to binary representation
char *string_to_binary(char *str) {
  if (str == NULL) return NULL;
  int len = strlen(str);
  char *binary = myMalloc(len * BYTE_SIZE + 1);
  // Initialize the binary string to an empty string
  binary[0] = '\0';
  // Loop over each character in the string
  for (int i = 0; i < len; ++i) {
    char ch = str[i];
    // Convert each character to its binary representation
    for (int j = BYTE_SIZE - 1; j >= 0; --j) {
      if (ch & (1 << j)) {
        strcat(binary, "1");
      } else {
        strcat(binary, "0");
      }
    }
  }
  return binary;
}

// Function to get the first 8 characters of a string
void get_first_8_chars(char *str, char *result) {
  // If the input string or result is NULL, return without doing anything
  if (str == NULL || result == NULL) return;
  // Copy the first 8 characters from the input string to the result
  strncpy(result, str, BYTE_SIZE);
  result[BYTE_SIZE] = '\0';
}

// Function to get the last 8 characters of a string
void get_last_8_chars(char *str, char *result) {
  // If the input string or result is NULL, return without doing anything
  if (str == NULL || result == NULL) return;
  // Get the length of the input string
  size_t len = strlen(str);
  // If the length of the input string is less than 8,
  // copy the entire input string to the result
  if (len < BYTE_SIZE) {
    strcpy(result, str);
  } else {
    // Otherwise, copy the last 8 characters from the input string to the result
    strncpy(result, str + len - BYTE_SIZE, BYTE_SIZE);
    result[BYTE_SIZE] = '\0';
  }
}

// Function to cut off the last n characters from a string
void cut_last_n_chars(char *str, char *result, int n) {
  // If the input string or result is NULL, return without doing anything
  if (str == NULL || result == NULL) return;
  // Get the length of the input string
  int len = strlen(str);
  // If n is greater than or equal to the length of the input string,
  // set the result to an empty string
  if (len < n) {
    result[FIRST_CHAR] = '\0';
  } else {
    // Otherwise, copy all but the last n characters from the input string to
    // the result
    strncpy(result, str, len - n);
    // Null-terminate the result string
    result[len - n] = '\0';
  }
}
