#include "reader.h"
#include "command.h"
#include  <unistd.h>
#include <errno.h>
#include <ctype.h>

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

void flushBuffer(){
  int ch;
  while ((ch=getchar()) != EOF && ch != '\n')
        ;
}


int main(int argc, char **argv){
  /*   Initialise stuff here   */

  nlist *variableTable[HASHSIZE];
  
  FILE *fp = fopen(".profile", "r");
  if(fp == NULL){
    perror("No .profile file found ");
    char ans = '\0';
    while(ans != 'Y' && ans != 'N'){
      printf("Would you like to create a .profile file? (y/n)\n");
      ans = toupper(getchar());
      flushBuffer();
    }
    if(ans == 'N')
      return -1;
    else {
      char home[1024];
      printf("Enter home directory: ");
      scanf("%[^\n]", home);
      flushBuffer();

      char path[1024];
      printf("Enter path: ");
      scanf("%[^\n]", path);
      flushBuffer();
      
      FILE *otherFp = fopen(".profile", "w+");
      fprintf(otherFp, "HOME=%s\nPATH=%s\n",  home, path);
      fclose(otherFp);
      fp = fopen(".profile", "r");
    }
  }
  
  char **lines = readLines(fp); 
  fclose(fp);

  int len = arrayLength(lines);
  for(int i = 0; i < len; i++){
    variableStruct res = parseLine(lines[i]);
    if(res.errnum == 1){
      printf("Error parsing profile at line %i\n", i + 1);
    } else {
      install(res.variableName, res.variableValue, variableTable);
      free(lines[i]);
    }
  }
  free(lines);
  if(lookup("HOME", variableTable) == NULL){
    printf("HOME not defined\nEnter home directory: ");
    char home[1024];
    scanf("%[^\n]", home);
    flushBuffer();
    install("HOME", home, variableTable);
  }
  if(lookup("PATH", variableTable) == NULL){
    printf("PATH not defined\nEnter a path: ");
    char path[1024];
    scanf("%[^\n]", path);
    flushBuffer();
    install("PATH", path, variableTable);
  }
  List *pathList = createPathList(variableTable);
  chdir(lookup("HOME", variableTable)->value);

  while(1){
    char *cwd = getcwd(NULL, 0);
    printf("\n%s:\n$ ", cwd); 
    free(cwd);
    char userInput[1024];
    scanf("%[^\n]", userInput);
    flushBuffer();
    executeCommand(userInput, pathList, variableTable);
  }

  return 0;
}
