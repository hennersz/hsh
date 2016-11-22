#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "reader.h"

int countLines(FILE *fp){
  /**
   * Counts the number of lines in a file
   */
  int count = 0;
  char c;
  while((c = fgetc(fp))!=EOF){
    if(c == '\n'){
      count++;
    }
  }
  rewind(fp);
  return count;
}

int lineLength(FILE *fp){
  /**
   * Counts the length of the current line in file
   */
  fpos_t pos;//save current position
  fgetpos(fp, &pos);
  char c = '\0';
  int length = 0;
  c=fgetc(fp);

  while(c != '\n' && c != EOF){
    c=fgetc(fp);
    length++;
  }
  fsetpos(fp, &pos);//move file pointer back to where is was in the file at beggining 
  return length;
}

char* readLine(FILE* fp){
  /**
   * Reads one line from a file and allocates the exact amount of memory for it
   */
  char *buffer;
  int bufferLength = lineLength(fp) + 1;
  int bufferIndex = 0;
  buffer = malloc(sizeof(char) * bufferLength );
  char c = '\0';
  c = fgetc(fp);
  while(c != '\n' && c != EOF){
    buffer[bufferIndex]= c;
    bufferIndex++;
    c = fgetc(fp);
  }
  buffer[bufferIndex] = '\0';//terminate string with null

  return buffer; 
}

char** readLines(FILE *fp){
  /**
   * Reads all the lines in a file into an array of strings terminated by a NULL reference
   */
  char **lines;
  int linesLength = countLines(fp) + 1;
  lines =malloc(sizeof(char *) * linesLength);
  int linesIndex = 0;
  char *line = readLine(fp);
  while(strcmp(line, "\0")){ //check for end of file
    lines[linesIndex] = line;
    line = readLine(fp);
    linesIndex++;
  }
  lines[linesIndex] = NULL; //end array with null reference for your looping convinience and pleasure
  return lines;
}

char** splitString(char* string, char *splitter){
  /**
   * Splits a string into an array of strings by some string of characters
   */
  char **segments;
  int segmentIndex = 0;
  int count = 0;
  int i;
  for(i = 0; i < strlen(string); i++){
    if(string[i] == splitter[0]){
      count++;
    }
  }
  int noOfSegments = count + 2;
  segments = malloc(sizeof(char *) * noOfSegments);
  char *segment;
  segment = strtok(string, splitter);

  while(segment != NULL){
    segments[segmentIndex] = malloc(sizeof(char) * (strlen(segment) + 1));
    strcpy(segments[segmentIndex], segment);
    segmentIndex++;
    segment = strtok(NULL, splitter);
  }
  segments[segmentIndex] = NULL; //end array with null reference for your looping convinience and pleasure
  return segments;
}

int lookupBinary(char *directory, char *binary){
  /**
   * Checks a directory to see if a binary exists in it
   */
  DIR *dirp;
  struct dirent *dp;
  
  if((dirp = opendir(directory)) == NULL){
    return 1;
  }
  
  while((dp = readdir(dirp)) != NULL){
    if(strcmp(dp->d_name, binary) == 0){
      closedir(dirp);
      return 0;
    }
  }
  
  closedir(dirp);
  return 2;
}
