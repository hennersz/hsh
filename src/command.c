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
    while(currentNode != NULL){
      if(lookupBinary(currentNode->path, binary) == 0){
        char *returnValue = malloc(sizeof(char) * (strlen(currentNode->path) + strlen(binary) + 2)); //include space for extra slash and null char
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
      free(path);
      free(bin);
      printf("Error: no such file or directory: %s\n", binary);
    }
  }

  return NULL;
}

int isAssignment(char *command){
  char *variableRegex = "\\$[A-Z]+=";
  regmatch_t res = match(variableRegex, command);
  return !(res.rm_so == res.rm_eo);
}

void assignVariable(char *assignment, nlist *variables[HASHSIZE]){
  char *variableRegex = "\\$[A-Z]+=";
  regmatch_t varName = match(variableRegex, assignment);
  int varNameLen = varName.rm_eo - varName.rm_so - 2; //dont include = or $ in length 

  char *variableName = malloc(sizeof(char) * (varNameLen));
  strncpy(variableName, (assignment + 1), varNameLen);
  variableName[varNameLen] = '\0';
  //$HOME=/users/henrymortimer

  char *variableValue = malloc(sizeof(char) * (strlen((assignment + varNameLen + 2)) + 1));
  strcpy(variableValue, (assignment + varNameLen + 2));

  install(variableName, variableValue, variables);
  free(variableName);
  free(variableValue);
}

void executeCommand(char *command, List *paths, nlist *variables[HASHSIZE]){
  char **args = splitString(command, " ");
  if(strcmp(args[0], "cd") == 0){
    if(args[1] == NULL){
      chdir(lookup("HOME", variables)->value);
    } else{
      if(chdir(args[1]) == -1){
        perror("Error ");
      }
    }
  } else if(isAssignment(args[0])) {
    assignVariable(args[0], variables);
  } else {
    char *binary = searchForBinary(paths, args[0]);
    if(binary == NULL){
      return;
    } else {
      args[0] = binary;
      execSubprocess(args);
    }
  }

}
