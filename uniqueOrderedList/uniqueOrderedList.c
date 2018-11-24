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
    //Checking malloc
    if(listPointer == NULL) {
        return NULL;
    }

    listPointer -> copyNode = copyNode;
    listPointer -> freeNode = freeNode;
    listPointer -> equalsNode = equalsNode;
    listPointer -> greaterNode = greaterNode;

    return listPointer;
}
//uniqueOrderedList destroyer
void uniqueOrderedListDestroy(UniqueOrderedList listToDestroy) {
    //if the list is already null no need to destroy it
    if (!listToDestroy) {
        return;
    }

    while (uniqueOrderedListSize(listToDestroy)) { //size is bigger than 0;
        uniqueOrderedListRemove(listToDestroy, uniqueOrderedListGetLowest(listToDestroy));
        //in each iteration we remove the lowest element in the list until there are no elements
    }

    free(listToDestroy);

    return;
}

// copies an existing list into a new one.
// this function assumes the list to copy is already sorted otherwise will not work
UniqueOrderedList uniqueOrderedListCopy(UniqueOrderedList listToCopy){

    //copying null
    if (!listToCopy){
        return NULL;
    }

    UniqueOrderedList copiedList = malloc(sizeof(*copiedList));
    if(!copiedList){
        return NULL;
    }

    UniqueOrderedList* listToCopyPtr = &listToCopy;

    while(listToCopyPtr){
        // inserts the first element in the listToCopy to the copiedList
        uniqueOrderedListInsert(copiedList, uniqueOrderedListGetLowest(*listToCopyPtr));

        //TODO - this is probably buggy af
        //advances the pointer to the next element
        listToCopyPtr = &((*listToCopyPtr)->next);
    }

    return copiedList;

}

int uniqueOrderedListSize(UniqueOrderedList listToSize){

    if(!listToSize){
        return -1;
    }

    int size = 0;
    UniqueOrderedList* listToSizePtr = &listToSize;

    while(listToSizePtr){

        if(uniqueOrderedListGetLowest(*listToSizePtr)){ //first element isn't NULL
            size++;
        }

        listToSizePtr = &((*listToSizePtr)->next);
    }

    return size;

}

