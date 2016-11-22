#include "linkedList.h"

/**
 * Very simple singly linked list implementation to keep the path in
 */
Node* pushNode(char *path, List *list){
  /**
   * Adds node to the linked list
   */
  Node *newNode = malloc(sizeof(Node));
  newNode->path = path;
  newNode->next = NULL;

  if(list->tail == NULL){//first node in the list
    list->head = newNode;
    list->tail = newNode;
  } else {
    list->tail->next = newNode;
    list->tail = newNode;
  }

  return newNode;
}

List *createList(){
  /**
   * Initialses a new list
   */
  List *newList = malloc(sizeof(List));
  newList->head = NULL;
  newList->tail = NULL;
  return newList;
}

void freeNode(Node *node){
  /**
   * Recursively frees children nodes until freeing the top node
   */
  if(node == NULL)
    return;
  freeNode(node->next);
  free(node->path);
}

void freeList(List *list){
  /**
   * Frees node before freeing list
   */
  freeNode(list->head);
  free(list);
}
