//
// Created by lambda7 on 11/21/18.
//

#include "uniqueOrderedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct node_t{
    Element data;
    struct node_t* next;
    copyElements copyData;
    freeElements freeData;
    elementsEquals equalsData;
    elementGreaterThan greaterData;
};

typedef struct node_t* Node;

struct uniqueOrderedList_t {
    Node head;
    Node* iterator;
    copyElements copyNode;
    freeElements freeNode;
    elementsEquals equalsNode;
    elementGreaterThan greaterNode;
    int size;
};


Node nodeCreate (Element data, copyElements copyData, freeElements freeData, elementsEquals equalsData, elementGreaterThan greaterData){
    if(!data || !copyData || !freeData || !equalsData || !greaterData){
        return NULL;
    }

    Node createdNode = malloc(sizeof(*createdNode));
    if(!createdNode){
        return NULL;
    }

    createdNode->copyData = copyData;
    createdNode->freeData = freeData;
    createdNode->equalsData = equalsData;
    createdNode ->greaterData = greaterData;
    createdNode->data = (*copyData)(data);
    createdNode->next = NULL;
}

Node nodeCopy (Node toCopy){
    if(!toCopy){
        return NULL;
    }

    Node createdNode = malloc(sizeof(*createdNode));
    if(!createdNode){
        return NULL;
    }
    createdNode->copyData = toCopy->copyData;
    createdNode->freeData = toCopy->freeData;
    createdNode->equalsData = toCopy->equalsData;
    createdNode ->greaterData = toCopy->greaterData;
    createdNode->data = (*(createdNode->copyData))(toCopy->data);

    while(toCopy->next != NULL){
        createdNode->next = nodeCopy(toCopy->next);
    }

    return createdNode;

}

//Frees a node and ALL OF THE LIST FOLLOWING IT
void nodeDestroy (Node toDestroy){
    if(!toDestroy){
        return;
    }

    if(toDestroy->next != NULL){
        nodeDestroy((toDestroy->next));
    }

    (*(toDestroy->freeData))(toDestroy->data);
    free(toDestroy);
}

Node getNext (Node toGetFor){
    return toGetFor->next;
}

//if the node to set for already has a next, this function will make the current next the next for the new next
void insertAfter (Node toSetFor, Node toInsert){
    toInsert->next = toSetFor->next;
    toSetFor->next = toInsert;
}

bool contains (Node head, Element compareTo){
    if(!head||!compareTo)
        return false;
    if((*(head->equalsData))(head->data, compareTo)){
        return true;
    }
    contains(head->next, compareTo);
}

//Unique ordered list constructor
UniqueOrderedList uniqueOrderedListCreate(copyElements copyNode, freeElements freeNode,
                                          elementsEquals equalsNode, elementGreaterThan greaterNode) {
    //Checking none of the functions given are null
    if( !copyNode || !freeNode || !equalsNode || !greaterNode) {
        return NULL;
    }


    UniqueOrderedList createdList = malloc(sizeof(*createdList));
    //Checking malloc
    if(!createdList) {
        return NULL;
    }


    createdList ->iterator = malloc(sizeof(*(createdList->iterator)));
    if(!createdList->iterator){
        free(createdList->head);
        free(createdList);
        return NULL;
    }

    createdList -> copyNode = copyNode;
    createdList -> freeNode = freeNode;
    createdList -> equalsNode = equalsNode;
    createdList -> greaterNode = greaterNode;
    createdList->size = 0;

    return createdList;
}

//uniqueOrderedList destroyer
void uniqueOrderedListDestroy(UniqueOrderedList listToDestroy) {
    //if the list is already null no need to destroy it
    if (!listToDestroy) {
        return;
    }

    nodeDestroy(listToDestroy->head);
    free(listToDestroy->iterator);
    free(listToDestroy);

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

    copiedList->head = nodeCopy(listToCopy->head);
    copiedList->iterator = &(copiedList->head);
    copiedList->size = listToCopy->size;
    copiedList->copyNode = listToCopy->copyNode;
    copiedList->freeNode = listToCopy->freeNode;
    copiedList->equalsNode = listToCopy->equalsNode;
    copiedList->greaterNode = listToCopy->greaterNode;

}

int uniqueOrderedListSize(UniqueOrderedList listToSize){

    if(!listToSize){
        return -1;
    }

    return listToSize->size;
}

bool uniqueOrderedListContains(UniqueOrderedList toCheck, Element toFind){
    if(!toCheck)
        return false;
    return contains(toCheck->head, toFind);
}

UniqueOrderedListResult uniqueOrderedListInsert(UniqueOrderedList insertTo, Element toAdd){
    if(!insertTo||!toAdd){
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }

    Node oneStepBack = insertTo->head;
    if(!oneStepBack){
        //the list has no head, we insert the element as the head
        insertTo->head = nodeCreate(toAdd, insertTo->copyNode, insertTo->freeNode, insertTo->equalsNode, insertTo->greaterNode);
        *(insertTo->iterator) = insertTo->head;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }

    Node iterator = getNext(insertTo->head);
    while(iterator!=NULL){
        if((*(iterator->greaterData))(iterator->data, toAdd)){
            //the current element is bigger than the one we want to add
            insertAfter(oneStepBack, nodeCreate(toAdd, insertTo->copyNode, insertTo->freeNode, insertTo->equalsNode, insertTo->greaterNode));
            return UNIQUE_ORDERED_LIST_SUCCESS;
        }

        if((*(iterator->equalsData))(iterator->data, toAdd)){
            //there is an element like this in the list
            return UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS;
        }
        oneStepBack = iterator;
        iterator = getNext(iterator);
    }

    // all the elements in the list are smaller than the one we want to add, so we add it in the end
    insertAfter(oneStepBack, nodeCreate(toAdd,  insertTo->copyNode, insertTo->freeNode, insertTo->equalsNode, insertTo->greaterNode));
    return UNIQUE_ORDERED_LIST_SUCCESS;
}

UniqueOrderedListResult uniqueOrderedListRemove(UniqueOrderedList removeFrom, Element toRemove){
    if(!removeFrom||!toRemove){
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }

    if(!(removeFrom->head)|| !(contains(removeFrom->head, toRemove))){
        return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST;
    }

    Node oneStepBack = removeFrom->head;
    Node iterator = oneStepBack->next;
    while(iterator != NULL){
        if((*(iterator->equalsData))(iterator->data, toRemove)){
            insertAfter(oneStepBack, nodeCopy(iterator->next));
            nodeDestroy(iterator);
            return UNIQUE_ORDERED_LIST_SUCCESS;
        }

        oneStepBack = iterator;
        iterator = getNext(iterator);
    }

    return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST;
}

Element uniqueOrderedListGetLowest(UniqueOrderedList toGet){
    if(!toGet || !(toGet->head) || !(toGet->iterator))
        return NULL;
    *(toGet->iterator) = (toGet->head);
    return (*(toGet->iterator))->data;
}

Element uniqueOrderedListGetGreatest(UniqueOrderedList toGet){
    if(!toGet || !(toGet->head))
        return NULL;

    Node oneStepBack = toGet->head;
    Node iterator = getNext(oneStepBack);
    while(iterator!=NULL){
        oneStepBack = iterator;
        iterator = getNext(oneStepBack);
    }

    *(toGet->iterator) = oneStepBack;
    return (*(toGet->iterator))->data;
}

Element uniqueOrderedListGetNext(UniqueOrderedList toGet){
    if(!toGet || !(toGet->iterator))
        return NULL;

    *(toGet->iterator) = getNext(*(toGet->iterator));

    return (*(toGet->iterator))->data;
}

void uniqueOrderedListClear(UniqueOrderedList toClear){
    nodeDestroy(toClear->head);
}

