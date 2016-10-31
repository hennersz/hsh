#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

regmatch_t match(char *regexString, char *string){
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
  char *variableRegex = "\\$[^\\$=]+=";
  variableStruct result;
  regmatch_t varName = match(variableRegex, line);
  if(varName.rm_so == varName.rm_eo){
    result.errno = 1;
    return result;
  } else {
    int varNameLen = varName.rm_eo - varName.rm_so;
    result.variableName = malloc(sizeof(char) * (varNameLen));
    strncpy(result.variableName, line, varNameLen - 1);
    result.variableName[varNameLen] = '\0';
    int valueLen = strlen(&line[varNameLen + 1]);
    result.variableValue = malloc(sizeof(char) * (valueLen + 1));
    strncpy(result.variableValue, &line[varNameLen + 1], valueLen);
    result.variableValue[valueLen] = '\0';
    result.errno = 0;
    return result;
  } 
}
