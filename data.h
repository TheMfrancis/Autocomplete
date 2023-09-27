#ifndef _DATA_H_
#define _DATA_H_

#include "myUtils.h"

// Constant definitions
#define COMMA ","
#define QUOTATION '\"'
#define NULL_TERMINATOR '\0'
#define MAX_FIELD_LEN 128 + 1
#define MAX_LINE_LEN 512 + 1
#define FIRST_CHAR 0
#define BYTE_SIZE 8

// A struct containing all the data about a particular establishment
typedef struct data data_t;
struct data {
  int index;
  int census_year;
  int block_id;
  int property_id;
  int base_property_id;
  int industry_code;
  int number_of_seats;
  // All strings are pointers to maximise memory efficiency
  char *building_address;
  char *clue_small_area;
  char *business_address;
  char *trading_name;
  char *industry_description;
  char *seating_type;
  double longitude;
  double latitude;
  data_t *next;
};

// This function is similar to strcmp but counts the number of character
// comparisons and also returns 0 if s1 is a prefix of s2
int strcmp_count(const char *s1, const char *s2, int *comparisons);

void printData(FILE *f, data_t *data);

// This function extracts longitude and latitude values from a CSV record.
void getCoordinates(char *token, data_t *p);

// This function extracts the next string field from a CSV record.
char *getStr(char *token);

// This function extracts data from a CSV record and stores it in an
// establishment_t structure.
void getData(char *line, data_t *p);

// This function frees the memory allocated for an establishment_t structure and
// its associated data.
void freeData(data_t *data);

// This function extracts the next integer field from a CSV record.
int getInt(char *token);

#endif
