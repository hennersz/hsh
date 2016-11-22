#include "command.h"

int execSubprocess(char *argv[]){
  /**
   * Given a list of arguments this will do all the fork exec spiel to run the command in a new subprocess
   */
  pid_t pid = vfork();//we will exec immediately anyway so vfork is quicker due to not copying memory

  if(pid == -1){
    return -1;
  } else if(pid > 0){
    waitpid(-1, NULL, 0);//wait for child process to finish
  } else {
    execv(argv[0], argv); 
    exit(0);
  }
  return 0;
}

char *getPath(char *binWithPath){
  /**
   * Takes a path and will return everything up to and including the last slash, ie the directory the binary is in
   * e.g the input ./bin/shell will return ./bin/
   */
  int i = strlen(binWithPath)-1;
  while(binWithPath[i] != '/'){
    i--;
  }
  char *path = malloc(sizeof(char) * (i + 1));
  strncpy(path, binWithPath, i);
  //path[i] = '\0'; //add null character because strncpy won't

  return path;
}

char *getBin(char *binWithPath){
  /**
   * Takes a path and will return everything after the last slash, ie the binary
   *  e.g the input ./bin/shell will return shell
   */
  int i = strlen(binWithPath)-1;
  while(binWithPath[i] != '/' && i >= 0){
    i--;
  }
  char *bin = malloc(sizeof(char) * (strlen(binWithPath + i)));
  strcpy(bin, (binWithPath + i + 1));
  return bin;
}

char *searchForBinary(List *paths, char *binary){
  /**
   * Searches for a given binary. If there is a slash in binary treat it as a relative path else check the path variables
   */
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
  /**
   * Checks if a command is a variable assignment
   */
  char *variableRegex = "\\$[A-Z]+="; //regex to match anything starting with a $ then has 1 or more upper case letters then an = 
  regmatch_t res = match(variableRegex, command);
  return !(res.rm_so == res.rm_eo);//Nothing found if the start character and end character are the same
}

void assignVariable(char *assignment, nlist *variables[HASHSIZE]){
  /**
   * Takes an assignment, parses it and inserts key value pair into the hash table
   * e.g takes $HOME=/users/henrymortimer and adds HOME->/users/henrymoritmer into hash table
   */
  char *variableRegex = "\\$[A-Z]+=";//Same regex as above
  regmatch_t varName = match(variableRegex, assignment);
  int varNameLen = varName.rm_eo - varName.rm_so - 2; //dont include = or $ in length 

  char *variableName = malloc(sizeof(char) * (varNameLen));
  strncpy(variableName, (assignment + 1), varNameLen);
  variableName[varNameLen] = '\0';

  char *variableValue = malloc(sizeof(char) * (strlen((assignment + varNameLen + 2)) + 1));
  strcpy(variableValue, (assignment + varNameLen + 2));//Starts from first character after the =

  install(variableName, variableValue, variables);
  free(variableName);
  free(variableValue);
}

void executeCommand(char *command, List *paths, nlist *variables[HASHSIZE]){
  /**
   * Takes a command, splits on spaces then checks against some special cases before trying to find a binary to run
   */
  char **args = splitString(command, " ");
  if(strcmp(args[0], "cd") == 0){//Must change directory from the same process 
    if(args[1] == NULL){
      chdir(lookup("HOME", variables)->value);
    } else{
      if(chdir(args[1]) == -1){
        perror("Error ");
      }
    }
  } else if(isAssignment(args[0])) {
    assignVariable(args[0], variables);
  } else if(strcmp(args[0], "exit") == 0){//Kill the shell if user types exit
    exit(0);
  } else {
    char *binary = searchForBinary(paths, args[0]);
    if(binary == NULL){//binary not found
      return;
    } else {
      args[0] = binary;
      execSubprocess(args);
    }
  }

}
