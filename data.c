#include "data.h"

#include <stdlib.h>
#include <string.h>

// This function frees the memory allocated for an establishment_t structure and
// its associated data.
void freeData(data_t *data) {
  // Free the memory allocated for the strings in the data_t structure.
  free(data->building_address);
  free(data->clue_small_area);
  free(data->business_address);
  free(data->trading_name);
  free(data->industry_description);
  free(data->seating_type);
  // Free the memory allocated for the data_t structure.
  free(data);
  // Free the memory allocated for the establishment_t structure.
}

// This function is similar to strcmp but counts the number of character
// comparisons and also returns 0 if s1 is a prefix of s2
int strcmp_count(const char *s1, const char *s2, int *comparisons) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
    (*comparisons)++;
  }
  (*comparisons)++;
  if (!(*s1) && *s2) {
    return 0;
  }
  return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

// This function extracts the next string field from a CSV record.
char *getStr(char *token) {
  // Get the next token from the CSV record.
  token = strtok(NULL, COMMA);
  int sizeTaken = 0;
  char *str;
  // Check if the token starts with a quotation mark.
  if (token[FIRST_CHAR] == QUOTATION) {
    // Create a field buffer.
    char field[MAX_FIELD_LEN];
    int len = strlen(token);
    int commaLen = strlen(COMMA);
    // Concatenate tokens to the field buffer until a token ending with a
    // quotation mark is found.
    while (token[len - 1] != QUOTATION) {
      sprintf(field, "%s,", token);
      sizeTaken += (len + commaLen);
      token = strtok(NULL, COMMA);
      len = strlen(token);
    }
    sizeTaken += len;
    // Remove the last quotation mark from the token and concatenate it to the
    // field buffer.
    token[len - 1] = NULL_TERMINATOR;
    strcat(field, token);
    // Allocate memory for the result string and copy the contents of the field
    // buffer to it.
    str = (char *)myCalloc(sizeTaken - 1, sizeof(char));
    strcat(str, field + 1);
  } else {
    // If the token does not start with a quotation mark, simply copy it to the
    // result string.
    sizeTaken = strlen(token);
    str = (char *)myCalloc(sizeTaken + 1, sizeof(char));
    strcat(str, token);
  }
  return str;
}

// This function extracts the next integer field from a CSV record.
int getInt(char *token) {
  // Get the next token from the CSV record and convert it to an integer.
  token = strtok(NULL, COMMA);
  return atoi(token);
}

// This function extracts longitude and latitude values from a CSV record.
void getCoordinates(char *token, data_t *p) {
  // Get the next two tokens from the CSV record and parse them as
  // floating-point numbers.
  token = strtok(NULL, COMMA);
  sscanf(token, "%lf", &(p->longitude));
  token = strtok(NULL, COMMA);
  sscanf(token, "%lf", &(p->latitude));
}

void printData(FILE *f, data_t *data) {
  fprintf(f,
          "--> census_year: %d || block_id: %d || property_id: %d || "
          "base_property_id: %d || building_address: %s || clue_small_area: %s "
          "|| business_address: %s || trading_name: %s || industry_code: %d || "
          "industry_description: %s || seating_type: %s || number_of_seats: %d "
          "|| longitude: %.5lf || latitude: %.5lf || \n",
          data->census_year, data->block_id, data->property_id,
          data->base_property_id, data->building_address, data->clue_small_area,
          data->business_address, data->trading_name, data->industry_code,
          data->industry_description, data->seating_type, data->number_of_seats,
          data->longitude, data->latitude);
}

// This function extracts data from a CSV record and stores it in an
// establishment_t structure.
void getData(char *line, data_t *p) {
  // Get a pointer to the data_t structure within the establishment_t structure.
  // data_t *p = establishment->data;
  // Split the CSV record into tokens using strtok().
  char *token = strtok(line, COMMA);
  // Extract each field from the CSV record and store it.
  p->census_year = atoi(token);             // census_year is an integer
  p->block_id = getInt(token);              // block_id is an integer
  p->property_id = getInt(token);           // property_id is an integer
  p->base_property_id = getInt(token);      // base_property_id is an integer
  p->building_address = getStr(token);      // building_address is a string
  p->clue_small_area = getStr(token);       // clue_small_area is a string
  p->business_address = getStr(token);      // business_address is a string
  p->trading_name = getStr(token);          // trading_name is a string
  p->industry_code = getInt(token);         // industry_code is an integer
  p->industry_description = getStr(token);  // industry_description is a string
  p->seating_type = getStr(token);          // seating_type is a string
  p->number_of_seats = getInt(token);       // number_of_seats is an integer
  getCoordinates(token,p);  // longitude and latitude are floating-point numbers
}
