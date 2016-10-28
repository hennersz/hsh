#include "reader.h"
#include "hashTable.h"

int main(int argc, char **argv){
  FILE *fp;
  if((fp=fopen("./data/test.txt", "r"))== NULL){
    printf("Error opening file");
    return 1;
  }
  printf("%i\n", countLines(fp));
  printf("%i\n", lineLength(fp));
  printf("%s\n", readLine(fp)); 
  
  return 0;
}
