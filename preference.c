//
// Created by Bar The magical on 30/11/2018.
//

#include "preference.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

struct preference_t{
    char* candidateName;
    int candidateId;
    int priority;
};

Preference preferenceCreate (char* candidateNameToCopy, int candidateId, int priority){

    if(!candidateNameToCopy) {
        return NULL;
    }

    if(candidateId <0){
        return NULL;
    }

    if (priority <0) {
        return NULL;
    } //add all this to citizen

    Preference createdPreference = malloc(sizeof(*createdPreference));
    if (!createdPreference){
        return NULL; //in citizen - if we get a null return a memory error
    }

    createdPreference -> candidateId = candidateId;
    createdPreference -> priority = priority;

    createdPreference -> candidateName = malloc(sizeof(*candidateNameToCopy));
    if (!(createdPreference->candidateName)) {
        free (createdPreference);
        return NULL;
    }

    //TODO check if _strdup works as I intended
    createdPreference ->candidateName = _strdup(candidateNameToCopy);
    assert(createdPreference -> candidateName);

    return createdPreference;

}

Preference preferenceCopy (Preference toCopy){

    if(!toCopy){
        return NULL;
    } //this should be in the calling function

    Preference createdPreference = malloc(sizeof((*toCopy)));
    if(!createdPreference){
        return NULL;
    }

    createdPreference->priority = preferenceGetPriority(toCopy);
    createdPreference->candidateId = preferenceGetCandidateId((toCopy));
    if(createdPreference->priority < 0|| createdPreference->candidateId <0|| !(createdPreference->candidateName)){
        return NULL;
    }



    return createdPreference;

}

void preferenceDestroy (Preference toDestroy){

    if(!toDestroy){
        return;
    }
    free (toDestroy -> candidateName);
    free(toDestroy);
}

PreferenceResult preferenceGetCandidateName (Preference toGet, char** namePtr){
    if(!toGet)
      return PREFERENCE_NULL_ARGUMENT;

    namePtr = malloc(sizeof(*namePtr));
    if(!namePtr){
        return PREFERENCE_MEMORY_ERROR;
    }
    *namePtr = malloc(sizeof(strlen(toGet->candidateName)+1));
    if(!*namePtr) {
        return PREFERENCE_MEMORY_ERROR;
    }
    strcpy(*namePtr, toGet->candidateName);
    return PREFERENCE_SUCCESS;

}

PreferenceResult preferenceGetCandidateId (Preference toGet, int* idPtr){
    if(!toGet)
        return PREFERENCE_NULL_ARGUMENT;

    idPtr = malloc(sizeof(int));
    if(!idPtr)
        return PREFERENCE_MEMORY_ERROR;

    *idPtr = toGet->candidateId;
    return PREFERENCE_SUCCESS;
}

PreferenceResult preferenceGetPriority (Preference toGet, int* priorityPtr){
    if(!toGet)
        return PREFERENCE_NULL_ARGUMENT;

    priorityPtr = malloc(sizeof(int));
    if(!priorityPtr)
        return PREFERENCE_MEMORY_ERROR;

    *priorityPtr = toGet->priority;
    return PREFERENCE_SUCCESS;
}

int preferenceComparePriority (Preference firstToComp, Preference secondToComp){
    return ((secondToComp ->priority) - (firstToComp -> priority));
}

int preferenceCompareId (Preference firstToComp, Preference secondToComp){
    return ((secondToComp ->candidateId) - (firstToComp -> candidateId));
}

