#include <stdlib.h>

typedef struct Node{
  char *path;
  struct Node *next;
}Node;

typedef struct List{
  Node *head;
  Node *tail;
}List;

Node* pushNode(char *path, List *list);
List *createList();
void freeList(List *list);
