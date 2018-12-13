//
// Created by lambda7 on 11/21/18.
//

#include "uniqueOrderedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node_t* Node;

struct node_t{
    Element data;
    Node next;
    copyElements copyData;
    freeElements freeData;
    elementsEquals equalsData;
};


struct uniqueOrderedList_t {
    Node head;
    Node* iterator;
    copyElements copyNode;
    freeElements freeNode;
    elementsEquals equalsNode;
    elementGreaterThan greaterNode;
    int size;
};

Node nodeCreate (Element, copyElements, freeElements, elementsEquals);
Node nodeCopy (Node);
void nodeDestroyAll (Node);
bool contains (Node, Element);

Node nodeCreate (Element data, copyElements copyData, freeElements freeData, elementsEquals equalsData){
    if(!data || !copyData || !freeData || !equalsData){
        return NULL;
    }

    Node createdNode = malloc(sizeof(*createdNode));
    if(!createdNode){
        return NULL;
    }

    createdNode->copyData = copyData;
    createdNode->freeData = freeData;
    createdNode->equalsData = equalsData;
    createdNode->data = (*copyData)(data);
    createdNode->next = NULL;

    return createdNode;
}

Node nodeCopy (Node toCopy){
    if(!toCopy){
        return NULL;
    }

    Node createdNode = malloc(sizeof(*createdNode));
    if(!createdNode){
        return NULL;
    }

    createdNode->data = (*(toCopy->copyData))(toCopy->data);
    createdNode->copyData = toCopy->copyData;
    createdNode->freeData = toCopy->freeData;
    createdNode->equalsData = toCopy->equalsData;
    createdNode->next = nodeCopy(toCopy->next);

    return createdNode;
}

//Frees a node and ALL OF THE LIST FOLLOWING IT
void nodeDestroyAll (Node toDestroy){
    if(!toDestroy){
        return;
    }
    Node current = toDestroy;
    Node next;
    while(current != NULL){
        next = current->next;
        (*(current->freeData))(current->data);
        free(current);
        current = next;
    }
}


bool contains (Node head, Element compareTo){
    if(!compareTo){
        return false;
    }

    Node iterator = head;
    while(iterator != NULL){
        if((*(iterator->equalsData))(iterator->data, compareTo)){
            return true;
        }

        iterator = iterator->next;
    }
    return false;
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


    createdList -> copyNode = copyNode;
    createdList -> freeNode = freeNode;
    createdList -> equalsNode = equalsNode;
    createdList -> greaterNode = greaterNode;
    createdList->size = 0;
    createdList->head = NULL;
    createdList->iterator = &(createdList->head);

    return createdList;
}

//uniqueOrderedList destroyer
void uniqueOrderedListDestroy(UniqueOrderedList listToDestroy) {
    //if the list is already null no need to destroy it
    if (!listToDestroy) {
        return;
    }

    nodeDestroyAll(listToDestroy->head);
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

    return copiedList;

}

int uniqueOrderedListSize(UniqueOrderedList listToSize){

    if(!listToSize){
        return -1;
    }

    return listToSize->size;
}

bool uniqueOrderedListContains(UniqueOrderedList toCheck, Element toFind){
    if(!toCheck || !toCheck->head)
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
        insertTo->head = nodeCreate(toAdd, insertTo->copyNode, insertTo->freeNode, insertTo->equalsNode);
        insertTo->iterator = &insertTo->head;
        insertTo->size++;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }

    //the element we want to add is smaller than the head
    if((*(insertTo->greaterNode))(oneStepBack->data, toAdd)){

        Node temp = insertTo->head;
        insertTo->head = nodeCreate(toAdd, insertTo->copyNode, insertTo->freeNode, insertTo->equalsNode);
        insertTo->head->next = temp;
        insertTo->iterator = &(insertTo->head);
        insertTo->size++;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }

    //the element we want to add is equal to the head
    if((*(oneStepBack->equalsData))(oneStepBack->data, toAdd)){
        return UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS;
    }

    Node iterator = insertTo->head->next;
    while(iterator!=NULL){
        if((*(insertTo->greaterNode))(iterator->data, toAdd)){
            //the current element is bigger than the one we want to add
            Node nodeToAdd =  nodeCreate(toAdd, insertTo->copyNode, insertTo->freeNode, insertTo->equalsNode);
            nodeToAdd->next = oneStepBack->next;
            oneStepBack->next = nodeToAdd;
            insertTo->size++;
            return UNIQUE_ORDERED_LIST_SUCCESS;
        }

        if((*(iterator->equalsData))(iterator->data, toAdd)){
            //there is an element like this in the list
            return UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS;
        }
        oneStepBack = iterator;
        iterator = iterator->next;
    }

    // all the elements in the list are smaller than the one we want to add, so we add it in the end
    Node nodeToAdd =  nodeCreate(toAdd, insertTo->copyNode, insertTo->freeNode, insertTo->equalsNode);
    nodeToAdd->next = oneStepBack->next;
    oneStepBack->next = nodeToAdd;
    insertTo->size++;

    return UNIQUE_ORDERED_LIST_SUCCESS;
}

UniqueOrderedListResult uniqueOrderedListRemove(UniqueOrderedList removeFrom, Element toRemove){

    if(!removeFrom||!toRemove){
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }

    Node current = removeFrom->head;
    if(current != NULL && (*(current->equalsData))(current->data, toRemove)){
        //if the head itself is the node to remove
        removeFrom->head = current->next;
        (*(current->freeData))(current->data);
        free(current);
        removeFrom->size--;
        return UNIQUE_ORDERED_LIST_SUCCESS;
    }


    Node previous = current;
    //search for the node to be deleted
    while(current != NULL && !(*(current->equalsData))(current->data, toRemove)){
        previous = current;
        current = current->next;
    }

    if(current == NULL){
        return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST;
    }

    previous->next = current->next;
    (*(current->freeData))(current->data);
    free(current);
    removeFrom->size--;
    return UNIQUE_ORDERED_LIST_SUCCESS;
}

Element uniqueOrderedListGetLowest(UniqueOrderedList toGet){
    if(!toGet || !(toGet->head) || !(toGet->iterator))
        return NULL;
    toGet->iterator = &(toGet->head);
    return (*(toGet->iterator))->data;
}

Element uniqueOrderedListGetGreatest(UniqueOrderedList toGet){
    if(!toGet)
        return NULL;
    toGet->iterator = &(toGet->head);
    if(!toGet->iterator || !*(toGet->iterator)){
        return NULL;
    }
    while((*(toGet->iterator))->next !=NULL){
        toGet->iterator = &((*(toGet->iterator))->next);
    }

    return (*(toGet->iterator))->data;
}

Element uniqueOrderedListGetNext(UniqueOrderedList toGet){
    if(!toGet || !(toGet->iterator) || !(*(toGet->iterator)))
        return NULL;

    toGet->iterator = &((*(toGet->iterator))->next);

    if(!toGet->iterator||!(*(toGet->iterator))){
        return NULL;
    }

    return (*(toGet->iterator))->data;
}

void uniqueOrderedListClear(UniqueOrderedList toClear){

    nodeDestroyAll(toClear->head);
    toClear->head = NULL;
    toClear->iterator = &(toClear->head);
}

