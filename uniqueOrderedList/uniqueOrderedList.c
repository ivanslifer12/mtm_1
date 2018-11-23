//
// Created by lambda7 on 11/21/18.
//

#include "uniqueOrderedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct uniqueOrderedList_t {
    Element data;
    UniqueOrderedList next;
    copyElements copyNode;
    freeElements freeNode;
    elementsEquals equalsNode;
    elementGreaterThan greaterNode;
};

//Unique ordered list constructor
UniqueOrderedList uniqueOrderedListCreate(copyElements copyNode, freeElements freeNode,
        elementsEquals equalsNode, elementGreaterThan greaterNode) {
  //Checking none of the functions given are null
    if( !copyNode || !freeNode || !equalsNode || !greaterNode) {
      return NULL;
  }

  UniqueOrderedList listPointer = malloc(sizeof(*listPointer));
    //Checking Malloc
  if(listPointer == NULL) {
      return NULL;
  }

  //allocating space for data
  listPointer-> data = malloc(sizeof(Element));
  if(listPointer-> data == NULL){
      free(listPointer);
      return NULL;
  }

  //allocating space for pointer to next element
  listPointer -> next = malloc(sizeof(Element*));
    if(listPointer-> next == NULL){
        free(listPointer);
        return NULL;
    }

    listPointer -> copyNode = copyNode;
    listPointer -> freeNode = freeNode;
    listPointer -> equalsNode = equalsNode;
    listPointer -> greaterNode = greaterNode;

    return listPointer;


}


