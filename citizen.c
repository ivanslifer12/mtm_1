#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "citizen.h"
#include "preference.h"
#include "uniqueOrderedList/uniqueOrderedList.h"

struct citizen_t {
    int citizenId;
    char *name;
    int age;
    int yearsOfEducation;
    bool isCandidate;
    UniqueOrderedList preferences;
};

Citizen citizenCreate(int citizenId, const char *citizenName,
                      int yearsOfEducation, int age) {
    if (citizenId < 0 || !citizenName || yearsOfEducation < 0 || age <= 0){
        return NULL;
    } //all those checks should be in city

    Citizen createdCitizen = malloc(sizeof(*createdCitizen));
    if (!createdCitizen) {
        return NULL;
    }

    createdCitizen->citizenId = citizenId;
    createdCitizen->yearsOfEducation = yearsOfEducation;
    createdCitizen->age = age;
    createdCitizen->isCandidate = false;

    createdCitizen->name = malloc(sizeof(strlen(citizenName) + 1));
    if (!(createdCitizen->name)) {
        free(createdCitizen);
        return NULL;
    }
    strcpy(createdCitizen->name, citizenName);
    assert(createdCitizen->name);

    createdCitizen->preferences =
        uniqueOrderedListCreate((void*(*)(void*))preferenceCopy,
                (void(*)(void*))preferenceDestroy,
                (bool(*)(void*, void*))preferenceIsEqual,
                (bool(*)(void*, void*))preferenceComparePriority);
    assert(createdCitizen->preferences);

    return createdCitizen;

}

Citizen citizenCopy(Citizen toCopy) {
    if (!toCopy) {
        return NULL;
    } //city should check that

    Citizen createdCitizen = malloc(sizeof(*createdCitizen));
    if (!createdCitizen) {
        return NULL;
    }

    createdCitizen->citizenId = toCopy->citizenId;
    createdCitizen->isCandidate = toCopy->isCandidate;
    createdCitizen->age = toCopy->age;
    createdCitizen->yearsOfEducation = toCopy ->yearsOfEducation;
    createdCitizen->preferences = uniqueOrderedListCopy(toCopy->preferences);
    createdCitizen->name = malloc(sizeof(strlen(toCopy->name)+1));
    createdCitizen->name = strcpy(createdCitizen->name, toCopy->name);

    return createdCitizen;
}

void citizenDestroy(Citizen toDestroy) {
    if (!toDestroy)
        return;

    uniqueOrderedListDestroy(toDestroy->preferences);
    free(toDestroy->name);
    free(toDestroy);
}

bool citizenIsEqual (Citizen firstToCompare, Citizen secondToCompare){
    return (firstToCompare->citizenId == secondToCompare->citizenId);
}

bool citizenIsGreater (Citizen firstToCompare, Citizen secondToCompare){
    return (strcmp(firstToCompare->name, secondToCompare->name) > 0 ||
    (strcmp(firstToCompare->name, secondToCompare->name) == 0 &&
    firstToCompare->citizenId < secondToCompare->citizenId));
}



CitizenResult getId(Citizen toGet, int* idPtr) {

    if (!toGet || !idPtr) {
        return CITIZEN_NULL_ARGUMENT;
    }

    *idPtr = toGet->citizenId;

    return CITIZEN_SUCCESS;
}

CitizenResult getAge(Citizen toGet, int *agePtr) {

    if (!toGet || !agePtr) {
        return CITIZEN_NULL_ARGUMENT;
    }


    *agePtr = toGet->age;
    return CITIZEN_SUCCESS;
}

CitizenResult getEducation(Citizen toGet, int *educationPtr) {
    if (!toGet || !educationPtr) {
        return CITIZEN_NULL_ARGUMENT;
    }
    *educationPtr = toGet->yearsOfEducation;
    return CITIZEN_SUCCESS;
}

CitizenResult getName(Citizen toGet, char **namePtr) {
    if (!toGet || !namePtr) {
        return CITIZEN_NULL_ARGUMENT;
    }
    *namePtr = malloc(sizeof(strlen(toGet->name) + 1));
    if (!*namePtr) {
        return CITIZEN_MEMORY_ERROR;
    }
    strcpy(*namePtr, toGet->name);
    return CITIZEN_SUCCESS;
}

CitizenResult getCandidateStat(Citizen toGet, bool *candidateStatPtr) {
    if (!toGet || !candidateStatPtr) {
        return CITIZEN_NULL_ARGUMENT;
    }


    *candidateStatPtr = toGet->isCandidate;
    return CITIZEN_SUCCESS;
}

CitizenResult getAPriority (Citizen toGet, int candidateId,
        int* priorityPtr){
    if(!toGet|| !priorityPtr){
        return CITIZEN_NULL_ARGUMENT;
    }

    if(candidateId <0 ){
        return CITIZEN_ILLEGAL_ID;
    }

    Preference iterator = uniqueOrderedListGetLowest(toGet->preferences);
    int idToComp;
    while(iterator!=NULL){
        PreferenceResult getId = preferenceGetCandidateId(iterator, &idToComp);
        if(getId!=PREFERENCE_SUCCESS){
            return CITIZEN_MEMORY_ERROR;
        }


        if(idToComp == (candidateId)){
            PreferenceResult getPriority =
                    preferenceGetPriority(iterator, priorityPtr);
            if(getPriority!=PREFERENCE_SUCCESS){
                return CITIZEN_MEMORY_ERROR;
            }

            return CITIZEN_SUCCESS;

        }
        iterator = uniqueOrderedListGetNext(toGet->preferences);
    }

    return CITIZEN_SUPPORT_DOESNT_EXIST;
}

CitizenResult getHighestSupport (Citizen toGet, int* candidateIdPtr){
    if(!toGet || !candidateIdPtr){
        return CITIZEN_NULL_ARGUMENT;
    }

    PreferenceResult getResult = preferenceGetCandidateId(
                    uniqueOrderedListGetLowest(toGet->preferences),
                    candidateIdPtr);
    if(getResult == PREFERENCE_NULL_ARGUMENT){
        return CITIZEN_SUPPORT_DOESNT_EXIST;
    }

    return CITIZEN_SUCCESS;
}

CitizenResult addPreference(Citizen addTo, int candidateId, int priority) {
    if (!addTo) {
        return CITIZEN_NULL_ARGUMENT;
    }

    if(candidateId < 0){
        return CITIZEN_ILLEGAL_ID;
    }

    if (priority < 0) {
        return CITIZEN_ILLEGAL_PRIORITY;
    }

    if(addTo->citizenId == candidateId && addTo->isCandidate){
        return CITIZEN_SUPPORT_EXISTS;
    }

    if (addTo->isCandidate) {
        return CITIZEN_CAN_NOT_SUPPORT;
    }

    Preference preferenceToAdd =
            preferenceCreate(candidateId, priority);
    UniqueOrderedListResult insertResult =
            uniqueOrderedListInsert(addTo->preferences, preferenceToAdd);
    if(insertResult == UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS){
        //there is a preference with the same priority or candidate id
        // we iterate to check which is it
        Preference iterator = uniqueOrderedListGetLowest(addTo->preferences);
        while(iterator!=NULL){
            if(preferenceComparePriority(iterator, preferenceToAdd)){
                preferenceDestroy(preferenceToAdd);
                return CITIZEN_PRIORITY_EXISTS;
            }
            iterator = uniqueOrderedListGetNext(addTo->preferences);
        }
        preferenceDestroy(preferenceToAdd);
        return CITIZEN_SUPPORT_EXISTS;
    }

    if(insertResult == UNIQUE_ORDERED_LIST_OUT_OF_MEMORY){
        preferenceDestroy(preferenceToAdd);
        return CITIZEN_MEMORY_ERROR;
    }

    preferenceDestroy(preferenceToAdd);
    return CITIZEN_SUCCESS;

}

CitizenResult clearPreference(Citizen clearTo, int candidateId) {

    if(!clearTo){
        return CITIZEN_NULL_ARGUMENT;
    }

    if(candidateId < 0){
        return CITIZEN_ILLEGAL_ID;
    }
    if(clearTo->citizenId == candidateId && clearTo->isCandidate){
        return CITIZEN_MUST_SUPPORT;
    }

    Preference iterator = uniqueOrderedListGetLowest(clearTo->preferences);
    while(iterator != NULL){

        int candidateIdToCompare;
        preferenceGetCandidateId(iterator, &candidateIdToCompare);
        if(candidateIdToCompare == candidateId){
            UniqueOrderedListResult removeResult =
                    uniqueOrderedListRemove(clearTo->preferences, iterator);
            assert(removeResult == UNIQUE_ORDERED_LIST_SUCCESS);
            return CITIZEN_SUCCESS;
        }

        iterator = uniqueOrderedListGetNext(clearTo->preferences);
    }

    return CITIZEN_SUPPORT_DOESNT_EXIST;
}

CitizenResult makeCandidate(Citizen toMake) {
    if(!toMake){
        return CITIZEN_NULL_ARGUMENT;
    }
    if(toMake->isCandidate){
        return CITIZEN_IS_ALREADY_CANDIDATE;
    }

    if(toMake->age <21){
        return CITIZEN_AGE_NOT_APPROPRIATE;
    }

    uniqueOrderedListClear(toMake->preferences);
    toMake->isCandidate = true;
    Preference toInsert = preferenceCreate(toMake->citizenId,0);
    uniqueOrderedListInsert(toMake->preferences, toInsert);
    preferenceDestroy(toInsert);

    return CITIZEN_SUCCESS;
}

CitizenResult clearCandidate(Citizen toClear) {
    if(!toClear){
        return CITIZEN_NULL_ARGUMENT;
    }

    if(!(toClear->isCandidate)){
        return CITIZEN_IS_NOT_CANDIDATE;
    }

    toClear->isCandidate = false;
    uniqueOrderedListClear(toClear->preferences);
    return CITIZEN_SUCCESS;
}
