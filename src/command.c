#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "command.h"

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
  }
  return NULL;
}
