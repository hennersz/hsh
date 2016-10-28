#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int countLines(FILE *fp);
int lineLength(FILE *fp);
char* readLine(FILE* fp);
char** readLines(FILE *fp);
char** splitString(char* string, char *splitter);
