#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "reader.h"
#include "hashTable.h"
#include "parser.h"

int execSubprocess(char *argv[]);
char *getPath(char *binWithPath);
char *getBin(char *binWithPath);
void executeCommand(char *command, List *paths, nlist *variables[HASHSIZE]);
