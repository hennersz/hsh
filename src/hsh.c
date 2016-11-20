#include "reader.h"
#include "hashTable.h"
#include "parser.h"
#include "linkedList.h"
#include "command.h"
#include  <unistd.h>

int arrayLength(char **array){
  int count = 0;
  while(array[count] != NULL){
    count++;
  }
  return count;
}

char *binPath(List *pathList, char *binary, char *currentDirectory){
  Node *currentNode = pathList->head;

  while(currentNode != NULL){
    if(lookupBinary(currentNode->path, binary) == 0){
      char *returnValue = malloc(sizeof(char) * (strlen(currentNode->path) + strlen(binary) + 2));
      strcpy(returnValue, currentNode->path);
      strcat(returnValue, "/");
      strcat(returnValue, binary);
      return returnValue;
    }
    currentNode = currentNode->next;
  }
  if(lookupBinary(currentDirectory, binary) == 0){
    char *returnValue = malloc(sizeof(char) * (strlen(currentDirectory) + strlen(binary) + 2));
    strcpy(returnValue, currentDirectory);
    strcat(returnValue, "/");
    strcat(returnValue, binary);
    return returnValue;
  }
  printf("Error: command not found: %s\n", binary);
  return NULL;
}

List *createPathList(nlist *variableTable[HASHSIZE]){
  List *pathList = createList();
  char **paths = splitString(lookup("PATH", variableTable)->value, ":");

  if(paths == NULL){
    fprintf(stderr, "Error: Path variable not set\n");
    return NULL;
  }

  int len = arrayLength(paths);
  for(int i = 0; i < len; i ++){
    pushNode(paths[i], pathList);
  }

  return pathList;
}

int main(int argc, char **argv){
  FILE *fp = fopen(".profile", "r");
  char **lines = readLines(fp); 
  int len = arrayLength(lines);
  nlist *variableTable[HASHSIZE];
  for(int i = 0; i < len; i++){
    variableStruct res = parseLine(lines[i]);
    if(res.errno == 1){
      printf("Error parsing profile at line %i\n", i + 1);
    } else {
      install(res.variableName, res.variableValue, variableTable);
    }
  }

  printf("%s\n", getBin("/user/bin/ls"));
  /* List *pathList = createPathList(variableTable); */
  /* chdir(lookup("HOME", variableTable)->value); */
  /* char *currentDirectory = lookup("HOME", variableTable)->value; */

  /* while(1){ */
  /*   char *cwd = getcwd(NULL, 0); */
  /*   printf("\n%s:\n$ ", cwd); */ 
  /*   free(cwd); */
  /*   char userInput[1024]; */
  /*   scanf("%[^\n]", userInput); */
  /*   int ch; */
  /*   while ((ch=getchar()) != EOF && ch != '\n') */
  /*         ; */
  /*   char **args = splitString(userInput, " "); */
  /*   char *binary = binPath(pathList, args[0], currentDirectory); */
  /*   args[0] = binary; */
  /*   pid_t pid = vfork(); */

  /*   if(pid == -1){ */
  /*     printf("Faliure\n"); */
  /*   } else if(pid > 0){ */
  /*     waitpid(-1, NULL, 0); */
  /*   } else { */
  /*     execv(args[0], args); */
  /*     exit(0); */
  /*   } */
  /* } */

  return 0;
}
