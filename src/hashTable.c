#include "hashtable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * This entire thing is based on an implementation fomr  K&R book
 * Very simple and probably not that great but still good enough
 */

unsigned hash(char *s){
  /**
   * A simple hash function
   */
  unsigned hashVal;
  for(hashVal = 0; *s != '\0'; s++){
    hashVal = *s + 31 * hashVal; 
  } 
  return hashVal % HASHSIZE;
}

nlist *lookup(char *s, nlist *hashTab[HASHSIZE]){
  /**
   * Looks for the entry in the hash table given a key, Returns the node if found or NULL if not
   */
  nlist *np;

  for(np = hashTab[hash(s)]; np!=NULL; np = np->next){
    if(strcmp(s, np->key) == 0){
      return np;
    }
  }
  return NULL;
}

nlist* install(char *key, char *value, nlist *hashTab[HASHSIZE]){
  /**
   * Inserts a value into the table for a given key, will overwrite if there is already a value for the key
   * Returns the inserted node or NULL if it failed
   */
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


