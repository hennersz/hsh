/*
  _   _  _____ _   _                                 
| | | |/  ___| | | |                                
| |_| |\ `--.| |_| |                                
|  _  | `--. \  _  |                                
| | | |/\__/ / | | |                                
\_| |_/\____/\_| |_/                                
                                                    
                                                    
 _   _                         _____ _   _      _ _ 
| | | |                       /  ___| | | |    | | |
| |_| | ___ _ __  _ __ _   _  \ `--.| |_| | ___| | |
|  _  |/ _ \ '_ \| '__| | | |  `--. \  _  |/ _ \ | |
| | | |  __/ | | | |  | |_| | /\__/ / | | |  __/ | |
\_| |_/\___|_| |_|_|   \__, | \____/\_| |_/\___|_|_|
                        __/ |                       
                       |___/                        

The MVP shell
*/
#include "reader.h"
#include "command.h"
#include  <unistd.h>
#include <errno.h>
#include <ctype.h>

int arrayLength(char **array){
  /**
   * Counts length of array of strings.
   * Assumes array ends with NULL pointer,
   */
  int count = 0;
  while(array[count] != NULL){
    count++;
  }
  return count;
}

List *createPathList(nlist *variableTable[HASHSIZE]){
  /**
   * Gets path variable from the hash table and creates a linked list of the directories, delimited by :
   */
  List *pathList = createList();
  char **paths = splitString(lookup("PATH", variableTable)->value, ":");

  if(paths == NULL){
    fprintf(stderr, "Error: Path variable not set\n");
    return NULL;
  }

  int len = arrayLength(paths);
  int i;
  for(i = 0; i < len; i ++){
    pushNode(paths[i], pathList);
  }

  return pathList;
}

void flushBuffer(){
  /**
   * Gets rid of any extra characters that may be hanging around in the input buffer
   */
  int ch;
  while ((ch=getchar()) != EOF && ch != '\n')
        ;
}

char *readString(const char *message){
  /**
   * Prints a prompt then reads user input until a newline is reached
   */
  printf("%s", message);
  char *value = malloc(sizeof(char) * 1024);
  scanf("%[^\n]", value);
  flushBuffer();
  return value;
}

void checkForMissingVariable(char *var, nlist *variables[HASHSIZE]){
  /**
   * Checks the hash table to see if a variable exists
   * If not prompt the user for it then save it to the hash table and write to the profile file
   */
  if(lookup(var, variables) == NULL){
    printf("%s not defined\nPlease enter a value:", var);
    char *data = readString("");
    install(var, data, variables);
    FILE *fp = fopen(".profile", "a");
    fprintf(fp, "%s=%s\n", var, data);
    fclose(fp);
    free(data);
  }
}

void createProfile(){
  /**
   * Propmts the user for a home directory and a path then writes it to a new .profile file
   */
  perror("No .profile file found ");
  char ans = '\0';
  while(ans != 'Y' && ans != 'N'){
    printf("Would you like to create a .profile file? (y/n)\n");//no pressure
    ans = toupper(getchar());
    flushBuffer();
  }
  if(ans == 'N')
    exit(-1);
  else {
    char *home = readString("Enter home directory: ");
    char *path = readString("Enter path: ");
    
    FILE *fp = fopen(".profile", "w+");
    fprintf(fp, "HOME=%s\nPATH=%s\n",  home, path);
    fclose(fp);
    free(home);
    free(path);
  }
}

void importProfile(nlist *variables[HASHSIZE]){
  /**
   * Looks for a profile file and prompts the user to create one if none found
   * Then parses each line of the profile and inserts each correctly formed assignment into the hash table
   */
  FILE *fp = fopen(".profile", "r");
  if(fp == NULL){
    createProfile();
    fp = fopen(".profile", "r");
  }
  
  char **lines = readLines(fp); 
  fclose(fp);

  int len = arrayLength(lines);
  int i;
  for(i = 0; i < len; i++){
    variableStruct res = parseLine(lines[i]);
    if(res.errnum == 1){
      printf("Error parsing profile at line %i\n", i + 1);
    } else {
      install(res.variableName, res.variableValue, variables);
      free(lines[i]);
    }
  }
  free(lines);
}


int main(int argc, char **argv){
  nlist *variableTable[HASHSIZE];
  importProfile(variableTable);
  checkForMissingVariable("HOME", variableTable);
  checkForMissingVariable("PATH", variableTable);

  List *pathList = createPathList(variableTable);
  chdir(lookup("HOME", variableTable)->value);

  char *currentPath = lookup("PATH", variableTable)->value;
  char *oldPath = lookup("PATH", variableTable)->value;

  while(1){
    currentPath = lookup("PATH", variableTable)->value;
    if(strcmp(currentPath, oldPath) != 0){//path has changed
      freeList(pathList);
      pathList = createPathList(variableTable);
    }
    oldPath = currentPath;
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
