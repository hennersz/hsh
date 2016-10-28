#include "hashtable.h"
#include <string.h>
#include <stdlib.h>

unsigned hash(char *s){
  unsigned hashVal;
  for(hashVal = 0; *s != '\0'; s++){
    hashVal = *s + 31 * hashVal; 
  } 
  return hashVal % HASHSIZE;
}

nlist *lookup(char *s, nlist *hashTab[HASHSIZE]){
  nlist *np;

  for(np = hashTab[hash(s)]; np!=NULL; np = np->next){
    if(strcmp(s, np->key) == 0){
      return np;
    }
  }
  return NULL;
}

nlist* install(char *key, char *value, nlist *hashTab[HASHSIZE]){
  nlist *np;
  unsigned hashVal;

  if((np = lookup(key, hashTab)) == NULL){
    np = malloc(sizeof(*np));
    if(np == NULL || (np->key = strdup(key)) == NULL)
      return NULL;
    hashVal = hash(key);
    np->next = hashTab[hashVal];
    hashTab[hashVal] = np;
  } else {
    free(np->value);
  }
  if((np->value = strdup(value)) == NULL)
    return NULL;
  return np;
}


