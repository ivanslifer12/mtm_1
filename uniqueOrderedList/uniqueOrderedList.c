//
// Created by lambda7 on 11/21/18.
//

#include "uniqueOrderedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define LIST_IS_NULL -1

typedef struct NodeInList_t *Node; //encapsulation for inner node

struct NodeInList_t{
    Element data;
    UniqueOrderedList next;
};

static bool ListNullTest(UniqueOrderedList list,Element testPtr);
static Node newNode(UniqueOrderedList next,Element data);
static Node nodeCopy(UniqueOrderedList list,Element data);


/*Linked list holds the current node and the first one*/
struct uniqueOrderedList_t {
    Node first;
    Node current;
    copyElements copyNode;
    freeElements freeNode;
    elementsEquals equalsNode;
    elementGreaterThan greaterNode;
    long size;

};
//Test function for NULL errors
static bool ListNullTest(UniqueOrderedList list,Element testPtr){
    if(list==NULL||testPtr==NULL){
        return true;
}
    return false;
}
//creates new node with the new data
static Node newNode(UniqueOrderedList next,Element data){
    if (!data||!next){
        return NULL;
    }
    Node newNodeForList = malloc(sizeof(*newNodeForList));
    if (newNodeForList==NULL){
        return NULL;
    }
    newNodeForList->data=data;
    newNodeForList->next=NULL;
    return newNodeForList;
}
//creates new node with copy
static Node nodeCopy(UniqueOrderedList list,Element data){
    if (!data||!list){
        return NULL;
    }
    Node newNodeForCopy=malloc(sizeof(*newNodeForCopy));
        if(newNodeForCopy==NULL){
            return NULL;
        }
    newNodeForCopy->next=NULL;
    newNodeForCopy->data=list->copyNode(list->current->data);
    return newNodeForCopy;

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
    listPointer -> size = 0;
    listPointer -> first = NULL;
    listPointer -> current = NULL;
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
}

// copies an existing list into a new one.
// this function assumes the list to copy is already sorted otherwise will not work
UniqueOrderedList uniqueOrderedListCopy(UniqueOrderedList listToCopy){

    //copying null
    if (!listToCopy){
        return NULL;
    }

    UniqueOrderedList copiedList = uniqueOrderedListCreate(listToCopy->copyNode,listToCopy->freeNode,
            listToCopy->equalsNode,listToCopy->greaterNode);
    if(!copiedList){
        return NULL;
    }

    if(listToCopy->size == 0) {
        return listToCopy;
    }
    copiedList->first=nodeCopy(listToCopy,listToCopy->first);
    copiedList->current=listToCopy->first;
    listToCopy->current=listToCopy->first->next;
    while(!listToCopy->current){
        Node tempNode= nodeCopy(listToCopy,listToCopy->first);
        copiedList->current->next=tempNode;
        copiedList->current=copiedList->current->next;
        listToCopy->current=listToCopy->current->next;
    }
    return copiedList;

}

int uniqueOrderedListSize(UniqueOrderedList listToSize){

    if(!listToSize){
        return LIST_IS_NULL;
    }

    int size = 0;
    listToSize->current=listToSize->first;

    while(!listToSize->current){
        size++;
        listToSize->current=listToSize->current->next;
    }
    listToSize->size=size;
    return size;

}

bool uniqueOrderedListContains(UniqueOrderedList listToCheck, Element toCheck){

    if(ListNullTest(listToCheck,toCheck)){
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }
    listToCheck->current=listToCheck->first;
    while(listToCheck->current!=NULL) {
        if ((listToCheck->equalsNode)(listToCheck->current->data, toCheck)) {
            return true;
        }
        listToCheck->current=listToCheck->current->next;
    }
    return false;
}


UniqueOrderedListResult uniqueOrderedListInsert(UniqueOrderedList listToInsert, Element toInsert){
    if(ListNullTest(listToInsert,toInsert)){
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }

    if(uniqueOrderedListContains(listToInsert,toInsert)==true){
        return UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS;
    }
    if(listToInsert->first==NULL){
        listToInsert->first=newNode(listToInsert,toInsert);
        listToInsert->size++;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }
    Node oneStepBack = listToInsert->first;
    listToInsert->current=listToInsert->first;

    while(!listToInsert->current){
        if((listToInsert->greaterNode)(listToInsert->current->next,toInsert)==false){//true when next is bigger then ins
            oneStepBack=listToInsert->current;
            listToInsert->current=listToInsert->current->next;

    }
        else{//oneStepBack holds the address on the node that we need to link
            //listToInsert on the current node holds the address that will be linked to
            Node insertNode=newNode(listToInsert,toInsert);
            oneStepBack->next=insertNode;
            insertNode->next=listToInsert->current;
            listToInsert->size++;
            return UNIQUE_ORDERED_LIST_SUCCESS;

        }

    }

}


UniqueOrderedListResult uniqueOrderedListRemove(UniqueOrderedList listToRemove, Element toRemove) {//TODO:not working
    if (ListNullTest(listToRemove, toRemove)) {
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }

    if (uniqueOrderedListContains(listToRemove, toRemove) == false) {
        return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST;
    } else {

        Node oneStepBack = listToRemove->first;
        listToRemove->current = listToRemove->first;
        while (!listToRemove->current) {
            if(!(listToRemove->equalsNode)(listToRemove->current->data,toRemove)){
                oneStepBack = listToRemove->current;
                listToRemove->current=listToRemove->current->next;

            }
            else{//oneStepBack is the address we need to link the list after the removal
                oneStepBack->next=listToRemove->current->next;
                free(listToRemove->current);
                return UNIQUE_ORDERED_LIST_SUCCESS;
            }
        }


    }
}



Element uniqueOrderedListGetLowest(UniqueOrderedList lowList){
    if(!lowList||lowList->first==NULL){
        return NULL;
    }
    lowList->current=lowList->first;
    return lowList->current->data;
}

Element uniqueOrderedListGetGreatest(UniqueOrderedList highList){
    if(!highList){
        return NULL;
    }
    while(highList->current->next!=NULL){
        highList->current=highList->current->next;
    }
    return highList->current->data;

}
Element uniqueOrderedListGetNext(UniqueOrderedList nextList){
    if(!nextList|| !nextList->current->next){
        return NULL;
    }
    nextList->current=nextList->current->next;
    return nextList->current->data;

}

void uniqueOrderedListClear(UniqueOrderedList listToClear){
    if (!listToClear){
        return ;
    }
    listToClear->current=listToClear->first;
    Node prevNode;
    while (listToClear->current!=NULL){
        prevNode=listToClear->current;
        listToClear->current=listToClear->current->next;
        (listToClear->freeNode)(listToClear->current->data);
        free(prevNode);
    }
    listToClear->size=0;
    listToClear->first=NULL;
}











