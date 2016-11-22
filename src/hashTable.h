#define HASHSIZE 50//Prime to reduce chance of collision???

typedef struct nlist { /* table entry: */
  /**
   * This hash table will create a linked list of values if there are collisions
   */
  struct nlist *next; /* next entry in chain */
  char *key; //variable name
  char *value; //variable value
} nlist;

unsigned hash(char *s);
nlist *lookup(char *s, nlist *hashTab[HASHSIZE]);
nlist* install(char *key, char *value, nlist *hashTab[HASHSIZE]);
