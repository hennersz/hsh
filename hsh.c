#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printBuffer(char *buffer, int length){
  for(int i = 0; i < length; i++){
    printf("%c",buffer[i]);
  }
  printf("\n");
}

char* readLine(FILE* fp){
  char *buffer;
  int bufferLength = 100;
  int bufferIndex = 0;
  buffer = malloc(sizeof(char) * bufferLength);
  memset(buffer, '\0', sizeof(char) * bufferLength); //initialise to nulls
  char c = '\0';
  
  while(c != '\n' && c != EOF){
    c = fgetc(fp);
    buffer[bufferIndex] = c;
    bufferIndex++;
    if(bufferIndex == bufferLength - 1){ //double buffer size if input is too large
      bufferLength *= 2;
      buffer = realloc(buffer, sizeof(char) * bufferLength);
    }
  }
  buffer[bufferIndex] = '\0';//terminate string with null

  return buffer; 
}

char** readLines(const char* file){
  FILE * fp;
  fp = fopen (file, "r");
  char **lines;
  int linesLength = 2;
  lines = malloc(sizeof(char *) * linesLength);
  int linesIndex = 0;
  char* line;
  int i = 0;
  while(!feof(fp)){ //check for end of file
    line = readLine(fp);   
    lines[linesIndex] = malloc((strlen(line) + 1) * sizeof(char));
    strcpy(lines[linesIndex], line);
    linesIndex++;
    if(linesIndex == linesLength-1){
      linesLength *= 2;
      lines = realloc(lines, sizeof(char *) * linesLength);
    }
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

int main(int argc, char** argv){
  char *str = "a:b:c:d:e";
  char *str2 = malloc(sizeof(char) * (strlen(str) + 1));
  strcpy(str2, str);
  char **strings = splitString(str2, ":");
  int i = 0;
  while(strings[i] != NULL){
    printf("%s\n", strings[i]);
    i++;
  }
  return 0;
}
