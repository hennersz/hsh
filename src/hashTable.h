#define HASHSIZE 256

typedef struct nlist { /* table entry: */
    struct nlist *next; /* next entry in chain */
    char *key; //variable name
    char *value; //variable value
} nlist;

unsigned hash(char *s);
nlist *lookup(char *s, nlist *hashTab[HASHSIZE]);
nlist* install(char *key, char *value, nlist *hashTab[HASHSIZE]);
