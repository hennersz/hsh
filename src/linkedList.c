#include "linkedList.h"

Node* pushNode(char *path, List *list){

  Node *newNode = malloc(sizeof(Node));
  newNode->path = path;
  newNode->next = NULL;

  if(list->tail == NULL){
    list->head = newNode;
    list->tail = newNode;
  } else {
    list->tail->next = newNode;
    list->tail = newNode;
  }

  return newNode;
}

List *createList(){
  List *newList = malloc(sizeof(List));
  newList->head = NULL;
  newList->tail = NULL;
  return newList;
}
