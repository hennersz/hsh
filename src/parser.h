#include <regex.h>

typedef struct variableStruct {
  char *variableName;
  char *variableValue;
  int errnum;
} variableStruct;

#define MAX_MATCHES 1
variableStruct parseLine(char *line);
regmatch_t match(char *regexString, char *string);
