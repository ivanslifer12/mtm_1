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

PreferenceResult preferenceCreate (char* candidateNameToCopy, int candidateId, int priority){

    if(!candidateNameToCopy) {
        return PREFERENCE_NULL_ARGUMENT;
    }

    if(candidateId <0){
        return PREFERENCE_ILLEGAL_ID;
    }

    if (priority <0) {
        return PREFERENCE_ILLEGAL_PRIORITY;
    }

    Preference createdPreference = malloc(sizeof(*createdPreference));
    if (!createdPreference){
        return PREFERENCE_MEMORY_ERROR;
    }

    createdPreference -> candidateId = candidateId;
    createdPreference -> priority = priority;
    createdPreference -> candidateName = malloc(sizeof(*candidateNameToCopy));

    if (!createdPreference->candidateName) {
        free (createdPreference);
        return PREFERENCE_MEMORY_ERROR;
    }

    //TODO check if _strdup works as I intended
    createdPreference ->candidateName = _strdup(candidateNameToCopy);
    assert(createdPreference -> candidateName);

    return PREFERENCE_SUCCESS;

}

PreferenceResult preferenceCopy (Preference toCopy){

    if(!toCopy){
        return PREFERENCE_NULL_ARGUMENT;
    }

    Preference createdPreference = malloc(sizeof(*createdPreference));
    if(!createdPreference){
        return PREFERENCE_MEMORY_ERROR;
    }

    PreferenceResult getPriority = preferenceGetPriority(toCopy, &(createdPreference->priority));
    if(getPriority!= PREFERENCE_SUCCESS){
        free (createdPreference);
        return getPriority;
    }

    PreferenceResult getId = preferenceGetCandidateId(toCopy, &(createdPreference->candidateId));
    if(getId != PREFERENCE_SUCCESS){
        free (createdPreference);
        return getId;
    }

    PreferenceResult getName = preferenceGetCandidateName(toCopy, &(createdPreference->candidateName));
    if(getName != PREFERENCE_SUCCESS){
        free (createdPreference);
        return getName;
    }

    return PREFERENCE_SUCCESS;

}

PreferenceResult preferenceDestroy (Preference toDestroy){

    if(!toDestroy){
        return PREFERENCE_NULL_ARGUMENT;
    }

    free (toDestroy -> candidateName);

    free(toDestroy);

    return PREFERENCE_SUCCESS;
}

PreferenceResult preferenceGetCandidateName (Preference toGet, char** namePtr){
    if(!toGet || !namePtr)
        return PREFERENCE_NULL_ARGUMENT;

    namePtr = malloc(sizeof(*(toGet->candidateName));
    if(!namePtr){
        return PREFERENCE_MEMORY_ERROR;
    }
    *namePtr = _strdup(toGet->candidateName);

    return PREFERENCE_SUCCESS;
}

PreferenceResult preferenceGetCandidateId (Preference toGet, int* idPtr){
    if(!toGet || !idPtr)
        return PREFERENCE_NULL_ARGUMENT;

    idPtr = malloc(sizeof(int));
    if(!idPtr){
        return PREFERENCE_MEMORY_ERROR;
    }

    *idPtr = toGet ->candidateId;

    return PREFERENCE_SUCCESS;
}

PreferenceResult preferenceGetPriority (Preference toGet, int* priorityPtr){
    if(!toGet || !priorityPtr){
        return PREFERENCE_NULL_ARGUMENT;
    }

    priorityPtr = malloc(sizeof(int));
    if(!priorityPtr){
        return PREFERENCE_MEMORY_ERROR;
    }
    *priorityPtr = toGet ->priority;

    return PREFERENCE_SUCCESS;
}

int preferenceComparePriority (Preference firstToComp, Preference secondToComp){
    return ((secondToComp ->priority) - (firstToComp -> priority));
}

int preferenceCompareId (Preference firstToComp, Preference secondToComp){
    return ((secondToComp ->candidateId) - (firstToComp -> candidateId));
}

