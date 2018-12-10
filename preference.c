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

    if(!candidateNameToCopy || candidateId < 0 || priority <0) {
        return NULL;
    }
     //add all this to citizen

    Preference createdPreference = malloc(sizeof(*createdPreference));
    if (!createdPreference){
        return NULL; //in citizen - if we get a null return a memory error
    }

    createdPreference -> candidateId = candidateId;
    createdPreference -> priority = priority;
    createdPreference ->candidateName = strdup(candidateNameToCopy);
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

    if(preferenceGetCandidateId(toCopy, &(createdPreference->candidateId)) != PREFERENCE_SUCCESS||
    preferenceGetCandidateName(toCopy, &(createdPreference->candidateName))!=PREFERENCE_SUCCESS||
    preferenceGetPriority(toCopy, &(createdPreference->priority))!=PREFERENCE_SUCCESS){
        free(createdPreference);
        return NULL;
    }

    return createdPreference;

}

void preferenceDestroy (Preference toDestroy){

    if(!toDestroy){
        return;
    }

    free(toDestroy->candidateName);
    free(toDestroy);
}

PreferenceResult preferenceGetCandidateName (Preference toGet, char** namePtr){
    if(!toGet || !namePtr)
      return PREFERENCE_NULL_ARGUMENT;

    *namePtr = malloc(sizeof(strlen(toGet->candidateName)+1));
    if(!*namePtr) {
        return PREFERENCE_MEMORY_ERROR;
    }
    strcpy(*namePtr, toGet->candidateName);

    return PREFERENCE_SUCCESS;
}

PreferenceResult preferenceGetCandidateId (Preference toGet, int* idPtr){
    if(!toGet || !idPtr) {
        return PREFERENCE_NULL_ARGUMENT;
    }

    *idPtr = toGet->candidateId;

    return PREFERENCE_SUCCESS;
}

PreferenceResult preferenceGetPriority (Preference toGet, int* priorityPtr){
    if(!toGet || !priorityPtr)
        return PREFERENCE_NULL_ARGUMENT;

    *priorityPtr = toGet->priority;
    return PREFERENCE_SUCCESS;
}

bool preferenceComparePriority (Preference firstToComp, Preference secondToComp){
    return ((firstToComp -> priority) - (secondToComp ->priority) > 0);
}

bool preferenceCompareId (Preference firstToComp, Preference secondToComp){
    return ((firstToComp -> candidateId) - (secondToComp ->candidateId) > 0);
}

bool preferenceIsEqual (Preference firstToComp, Preference secondToComp){
    return (firstToComp->candidateId == secondToComp->candidateId
    ||firstToComp->priority == secondToComp->priority);
}

