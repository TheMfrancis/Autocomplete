#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdlib.h>
#include <string.h>

// Function to cut a string from another string
void cutString(char *toCut, char *toBeCut, char *newStr);

// Function to find the common prefix between two strings
void findCommonPrefix(char *s1, char *s2, char *commonPref);

// Function to convert a character to binary
char *char_to_binary(char c);

// Function to convert a string to binary representation
char *string_to_binary(char *str);

// Function to get the first 8 characters of a string
void get_first_8_chars(char *str, char *result);

// Function to get the last 8 characters of a string
void get_last_8_chars(char *str, char *result);

// Function to cut off the last n characters from a string
void cut_last_n_chars(char *str, char *result, int n);

#endif 
