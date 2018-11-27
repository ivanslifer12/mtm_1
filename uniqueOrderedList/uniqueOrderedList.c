//
// Created by lambda7 on 11/21/18.
//

#include "uniqueOrderedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define LIST_IS_NULL -1

static bool ListNullTest(UniqueOrderedList list,Element testPtr);

/* when iterator is 0 ignore the struct */
struct uniqueOrderedList_t {
    Element data;
    UniqueOrderedList next;
    copyElements copyNode;
    freeElements freeNode;
    elementsEquals equalsNode;
    elementGreaterThan greaterNode;
    long iterator;
};
//Test function for NULL errors
static bool ListNullTest(UniqueOrderedList list,Element testPtr){
    if(list==NULL||testPtr==NULL){
        return true;
}

}

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
    listPointer->  iterator=0;
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
        return LIST_IS_NULL;
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

bool uniqueOrderedListContains(UniqueOrderedList listToCheck, Element toCheck){

    if(ListNullTest(listToCheck,toCheck)){
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }
    if ((*listToCheck->equalsNode)(listToCheck->data,toCheck)) {
        return true;
    }

    return uniqueOrderedListContains(listToCheck->next,toCheck); // Rec till the end of list
}


UniqueOrderedListResult uniqueOrderedListInsert(UniqueOrderedList listToInsert, Element toInsert){
    if(ListNullTest(listToInsert,toInsert)){
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }

    if(uniqueOrderedListContains(listToInsert,toInsert)==true){
        return UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS;
    }

    UniqueOrderedList oneBeforeToInsert;
    while((*listToInsert->greaterNode)(listToInsert->data,toInsert)) {
        oneBeforeToInsert=listToInsert;
        listToInsert=listToInsert->next;
    } //checks where to place the new Element

    UniqueOrderedList newNodeInList;
    newNodeInList=uniqueOrderedListCreate(listToInsert->copyNode,listToInsert->freeNode,
            listToInsert->equalsNode,listToInsert->greaterNode); // new node

    newNodeInList->data=toInsert;
    newNodeInList->iterator=oneBeforeToInsert->iterator+1;
    oneBeforeToInsert->next=newNodeInList;// swaps the data and pointers
    newNodeInList->next=listToInsert;
    listToInsert->iterator++;
    while (listToInsert!=NULL){
        listToInsert=listToInsert->next;
        if(listToInsert!=NULL){
        listToInsert->iterator++;
    }}
    return UNIQUE_ORDERED_LIST_SUCCESS;
}


UniqueOrderedListResult uniqueOrderedListRemove(UniqueOrderedList listToRemove, Element toRemove) {
    if (ListNullTest(listToRemove, toRemove)) {
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }
    UniqueOrderedList oneBeforeToRemove;

    while ((*listToRemove->equalsNode)(listToRemove->data, toRemove)==false) {
        oneBeforeToRemove= listToRemove;
        listToRemove = listToRemove->next;
        if(listToRemove==NULL){
            return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST;
        }
    } //checks where to place the new Element

    oneBeforeToRemove->next=oneBeforeToRemove->next->next; //links past the node to remove
    oneBeforeToRemove->iterator=oneBeforeToRemove->next->iterator;//update the current iterator
    while(oneBeforeToRemove->iterator!=0){ //update the iterators to the new value
        oneBeforeToRemove=oneBeforeToRemove->next;
        oneBeforeToRemove->iterator--;
    }
    free(listToRemove); //releases memory for a list
    return UNIQUE_ORDERED_LIST_SUCCESS;

}

Element uniqueOrderedListGetLowest(UniqueOrderedList lowList){
    while(lowList->iterator!=0)
}

Element uniqueOrderedListGetGreatest(UniqueOrderedList highList){

}
Element uniqueOrderedListGetNext(UniqueOrderedList nextList){

}

void uniqueOrderedListClear(UniqueOrderedList listToClear){

}










