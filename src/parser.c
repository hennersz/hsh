#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

regmatch_t match(char *regexString, char *string){
  /**
   * Returns the first match in a string using a posix regex or the error struct if none are found
   * The error struct can be indentified by the fact that the start and end index of the match are both 0
   */
  regex_t regex;
  regmatch_t errorStruct;
  errorStruct.rm_so = 0;
  errorStruct.rm_eo = 0;
  
  if((regcomp(&regex, regexString, REG_EXTENDED)) != 0){
    printf("regcomp failed");
  }

  regmatch_t matches[MAX_MATCHES];
  if(regexec(&regex, string, MAX_MATCHES, matches, 0) == 0) {
    return matches[0];
  } else {
    return errorStruct;
  }
}

variableStruct parseLine(char *line){
  /**
   * Takes a line from a profile file and checks if it is an assignment operation of the form ALLCAPSVAR=varaiableValue
   */
  char *variableRegex = "[A-Z]+=";
  variableStruct result;
  regmatch_t varName = match(variableRegex, line);

  if(varName.rm_so == varName.rm_eo){//no matches, return an error
    result.errnum = 1;
    return result;
  } else {
    // get everything before the =
    int varNameLen = varName.rm_eo - varName.rm_so;
    result.variableName = malloc(sizeof(char) * (varNameLen));
    strncpy(result.variableName, line, varNameLen - 1);
    result.variableName[varNameLen] = '\0';
    
    //get everything after the =
    int valueLen = strlen(&line[varNameLen ]);
    result.variableValue = malloc(sizeof(char) * (valueLen + 1));
    strncpy(result.variableValue, &line[varNameLen], valueLen);
    result.variableValue[valueLen] = '\0';
    result.errnum = 0;
    return result;
  } 
}
