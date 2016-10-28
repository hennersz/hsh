#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "reader.h"

int countLines(FILE *fp){
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
  char c = '\0';
  int length = 0;
  c=fgetc(fp);

  while(c != '\n' && c != EOF){
    c=fgetc(fp);
    length++;
  }
  rewind(fp);
  return length;
}

char* readLine(FILE* fp){
  char *buffer;
  int bufferLength = lineLength(fp) + 1;
  int bufferIndex = 0;
  buffer = malloc(sizeof(char) * bufferLength );
  char c = '\0';
  
  while(c != '\n' && c != EOF){
    c = fgetc(fp);
    buffer[bufferIndex] = c;
    bufferIndex++;
  }
  buffer[bufferIndex] = '\0';//terminate string with null

  return buffer; 
}

char** readLines(FILE *fp){
  char **lines;
  int linesLength = countLines(fp) + 1;
  lines = malloc(sizeof(char *) * linesLength);
  int linesIndex = 0;
  char* line;
  while(!feof(fp)){ //check for end of file
    line = readLine(fp);   
    lines[linesIndex] = malloc((strlen(line) + 1) * sizeof(char));
    strcpy(lines[linesIndex], line);
    linesIndex++;
  }
  lines[linesIndex] = NULL; //end array with null reference for your looping convinience and pleasure
  return lines;
}

char** splitString(char* string, char *splitter){
  char **segments;
  int noOfSegments = 2;
  int segmentIndex = 0;
  segments = malloc(sizeof(char *) * noOfSegments);
  char *segment;
  segment = strtok(string, splitter);

  while(segment != NULL){
    segments[segmentIndex] = malloc(sizeof(char) * (strlen(segment) + 1));
    strcpy(segments[segmentIndex], segment);
    segmentIndex++;
    if(segmentIndex == noOfSegments - 1){
      noOfSegments *= 2;
      segments = realloc(segments, sizeof(char *) * noOfSegments);
    }

    segment = strtok(NULL, splitter);
  }
  segments[segmentIndex] = NULL; //end array with null reference for your looping convinience and pleasure
  return segments;
}
