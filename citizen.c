#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "citizen.h"
#include "list.h"
#include "preference.h"

struct citizen_t {
    int citizenId;
    char *name;
    int age;
    int yearsOfEducation;
    bool isCandidate;
    List preferences;
};

Citizen citizenCreate(int citizenId, const char *citizenName, int yearsOfEducation, int age) {
    if (citizenId < 0 || !citizenName || yearsOfEducation < 0 || age <= 0) {
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
    strcpy_s(createdCitizen->name,strlen(citizenName)+1, citizenName);
    assert(createdCitizen->name);

    createdCitizen->preferences = listCreate((void*(*)(void*)) preferenceCopy, (void(*)(void*)) preferenceDestroy);
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

    if (((getId(toCopy, &(createdCitizen->citizenId)) != CITIZEN_SUCCESS)) ||
        (getAge(toCopy, &(createdCitizen->age)) != CITIZEN_SUCCESS) ||
        (getEducation(toCopy, &(createdCitizen->yearsOfEducation)) != CITIZEN_SUCCESS) ||
        (getCandidateStat(toCopy, &(createdCitizen->isCandidate)) != CITIZEN_SUCCESS)) {
        free(createdCitizen);
        return NULL;
    }

    createdCitizen->name = malloc(sizeof((strlen(toCopy->name)) + 1));
    if (!(createdCitizen->name)) {
        free(createdCitizen);
        return NULL;
    }
    strcpy_s(createdCitizen->name,strlen(toCopy->name)+1, toCopy->name);
    assert(createdCitizen->name);

    createdCitizen->preferences = listCopy(toCopy->preferences);
    assert(createdCitizen->preferences);

    return createdCitizen;
}

void citizenDestroy(Citizen toDestroy) {
    if (!toDestroy)
        return;

    listDestroy(toDestroy->preferences);
    free(toDestroy->name);
    free(toDestroy);
}

int citizenCompare(Citizen first, Citizen second) {
    return ((second->citizenId) - (first->citizenId));
}

CitizenResult getId(Citizen toGet, int *idPtr) {

    if (!toGet) {
        return CITIZEN_NULL_ARGUMENT;
    }

    if (!idPtr) {
        return CITIZEN_NULL_ARGUMENT;
    }

    *idPtr = toGet->citizenId;
    return CITIZEN_SUCCESS;
}

CitizenResult getAge(Citizen toGet, int *agePtr) {

    if (!toGet) {
        return CITIZEN_NULL_ARGUMENT;
    }

    if (!agePtr) {
        return CITIZEN_NULL_ARGUMENT;
    }

    *agePtr = toGet->age;
    return CITIZEN_SUCCESS;
}

CitizenResult getEducation(Citizen toGet, int *educationPtr) {
    if (!toGet) {
        return CITIZEN_NULL_ARGUMENT;
    }

    if (!educationPtr) {
        return CITIZEN_NULL_ARGUMENT;
    }

    *educationPtr = toGet->yearsOfEducation;
    return CITIZEN_SUCCESS;
}

CitizenResult getName(Citizen toGet, char **namePtr) {

    if (!toGet) {
        return CITIZEN_NULL_ARGUMENT;
    }

    if (!namePtr)
        return CITIZEN_NULL_ARGUMENT;

    *namePtr = malloc(sizeof(strlen(toGet->name) + 1));
    if (!*namePtr) {
        return CITIZEN_MEMORY_ERROR;
    }
    strcpy_s(*namePtr,strlen(toGet->name)+1, toGet->name);
    return CITIZEN_SUCCESS;
}

CitizenResult getCandidateStat(Citizen toGet, bool *candidateStatPtr) {
    if (!toGet) {
        return CITIZEN_NULL_ARGUMENT;
    }

    if (!candidateStatPtr) {
        return CITIZEN_NULL_ARGUMENT;
    }

    *candidateStatPtr = toGet->isCandidate;
    return CITIZEN_SUCCESS;
}

CitizenResult getPreferenceList(Citizen toGet, List* preferencesPtr) {
    if (!toGet) {
        return CITIZEN_NULL_ARGUMENT;
    }

    if (!preferencesPtr) {
        return CITIZEN_NULL_ARGUMENT;
    }

    *preferencesPtr = listCopy(toGet->preferences);
    return CITIZEN_SUCCESS;
}


CitizenResult addPreference(Citizen addTo, Citizen candidate, int priority) {

    if (!addTo || !candidate) {
        return CITIZEN_NULL_ARGUMENT;
    }

    if (priority < 0) {
        return CITIZEN_ILLEGAL_PRIORITY;
    }

    if (!candidate->isCandidate) {
        return CITIZEN_IS_NOT_CANDIDATE;
    }

    if (addTo->isCandidate) {
        return CITIZEN_CAN_NOT_SUPPORT;
    }

    Preference iterator = listGetFirst(addTo->preferences);
    int idToComp;
    int priorityToComp;
    while(iterator != NULL){
        PreferenceResult getId = preferenceGetCandidateId(iterator, &idToComp);
        if(getId == PREFERENCE_MEMORY_ERROR){
            return CITIZEN_MEMORY_ERROR;
        }

        PreferenceResult getPriority = preferenceGetPriority(iterator, &priorityToComp);
        if(getPriority== PREFERENCE_MEMORY_ERROR){
            return CITIZEN_MEMORY_ERROR;
        }

        if((idToComp) == candidate->citizenId){
            return CITIZEN_SUPPORT_EXISTS;
        }

        if((priorityToComp) == priority){
            return CITIZEN_PRIORITY_EXISTS;
        }

        iterator = listGetNext(addTo->preferences);
    }

    Preference preferenceToAdd = preferenceCreate(candidate->name, candidate->citizenId, priority);
    assert(preferenceToAdd);
    listInsertAfterCurrent(addTo->preferences, preferenceToAdd);
    return CITIZEN_SUCCESS;

}

CitizenResult clearPreference(Citizen clearTo, Citizen candidate) {

    if(!clearTo||!candidate){
        return CITIZEN_NULL_ARGUMENT;
    }
    if(!(candidate->isCandidate)){
        return CITIZEN_IS_NOT_CANDIDATE;
    }
    if(clearTo->citizenId == candidate->citizenId){
        return CITIZEN_MUST_SUPPORT;
    }

    Preference iterator = listGetFirst(clearTo->preferences);
    int idToCompare;
    while(iterator != NULL){
        PreferenceResult getId = preferenceGetCandidateId(iterator, &idToCompare);
        if(getId == PREFERENCE_MEMORY_ERROR){
            return CITIZEN_MEMORY_ERROR;
        }

        if(idToCompare== candidate->citizenId){
            listRemoveCurrent(clearTo->preferences);
            return CITIZEN_SUCCESS;
        }
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

    listClear(toMake->preferences);
    listInsertFirst(toMake->preferences, preferenceCreate(toMake->name, toMake->citizenId,0));
    toMake->isCandidate = true;

    return CITIZEN_SUCCESS;
}

CitizenResult clearCandidate(Citizen toClear) {
    //city is in charge of clearing support from all other citizens
    if(!toClear){
        return CITIZEN_NULL_ARGUMENT;
    }

    if(!(toClear->isCandidate)){
        return CITIZEN_IS_NOT_CANDIDATE;
    }

    toClear->isCandidate = false;
    listClear(toClear->preferences);
    return CITIZEN_SUCCESS;
}
