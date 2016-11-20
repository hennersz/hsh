#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "command.h"
#include "reader.h"

int execSubprocess(char *argv[]){
  pid_t pid = vfork();

  if(pid == -1){
    return -1;
  } else if(pid > 0){
    waitpid(-1, NULL, 0);
  } else {
    execv(argv[0], argv);
    exit(0);
  }
  return 0;
}

char *getPath(char *binWithPath){
  int i = strlen(binWithPath)-1;
  while(binWithPath[i] != '/'){
    i--;
  }
  char *path = malloc(sizeof(char) * (i + 1));
  strncpy(path, binWithPath, i);

  return path;
}

char *getBin(char *binWithPath){
  int i = strlen(binWithPath)-1;
  while(binWithPath[i] != '/' && i >= 0){
    i--;
  }
  char *bin = malloc(sizeof(char) * (strlen(binWithPath + i)));
  strcpy(bin, (binWithPath + i + 1));
  return bin;
}

char *searchForBinary(List *paths, char *binary){
  if(strstr(binary, "/") == NULL){//check if there is a / in the name
    Node *currentNode = paths->head;
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
    printf("Error: command not found: %s\n", binary);
  } else {
    char *path = getPath(binary);
    char *bin = getBin(binary);
    if(lookupBinary(path, bin) == 0){
      free(path);
      free(bin);
      return binary;
    } else {
      printf("Error: no such file or directory: %s\n", binary);
    }
  }
  return NULL;
}
