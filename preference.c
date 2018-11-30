//
// Created by Bar The magical on 30/11/2018.
//

#include "preference.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

struct preference_t{
    const char* candidateName;
    int candidateId;
    int priority;
};


PreferenceResult preferenceCreate (const char* candidateNameToCopy, int candidateId, int priority){

    if(!candidateNameToCopy) {
        return PREFERENCE_NULL_ARGUMENT;
    }

    if(candidateId <0){
        return PREFERENCE_ILLEGAL_ID;
    }

    if (priority <0) {
        return PREFERECNE_ILLEGAL_PRIORITY;
    }

    Preference createdPreference = malloc(sizeof(*createdPreference));
    if (!createdPreference){
        return PREFERENCE_MEMORY_ERROR;
    }

    createdPreference -> candidateId = candidateId;
    createdPreference -> priority = priority;
    createdPreference -> candidateName = malloc(sizeof(*candidateNameToCopy));

    if (!createdPreference->candidateName) {
        return PREFERENCE_MEMORY_ERROR;
    }

    //TODO - potiental problem with strcpy and const chat*
    strcpy_s(createdPreference -> candidateName, sizeof(*candidateNameToCopy), candidateNameToCopy);

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

    //TODO - functions need to return value to a ptr not the actual thing

    createdPreference -> candidateId = preferenceGetCandidateId(toCopy);
    createdPreference -> priority = preferenceGetPriority(toCopy);
    createdPreference -> candidateName = malloc(sizeof(preferenceGetCandidateName(toCopy)))
}