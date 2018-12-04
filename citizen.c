#include "citizen.h"
#include "list.h"
#include "preference.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct citizen_t{
    int citizenId;
    char* name;
    int age;
    int yearsOfEducation;
    bool isCandidate;
    List preferences;
};

Citizen citizenCreate (int citizenId, const char* citizenName, int yearsOfEducation, int age){
    if(citizenId < 0 || !citizenName || yearsOfEducation < 0 || age <= 0){
        return NULL;
    } //all those checks should be in city

    Citizen createdCitizen = malloc(sizeof(*createdCitizen));
    if(!createdCitizen){
        return NULL;
    }

    createdCitizen->citizenId = citizenId;
    createdCitizen->yearsOfEducation = yearsOfEducation;
    createdCitizen->age = age;
    createdCitizen->isCandidate = false;

    createdCitizen->name = malloc(sizeof(strlen(citizenName)+1));
    if(!(createdCitizen->name)){
        free(createdCitizen);
        return NULL;
    }
    strcpy(createdCitizen->name, citizenName);
    assert(createdCitizen->name);

    createdCitizen->preferences = listCreate(preferenceCopy,preferenceDestroy);
    assert(createdCitizen->preferences);

    return createdCitizen;

}

Citizen citizenCopy (Citizen toCopy){
    if(!toCopy){
        return NULL;
    } //city should check that

    Citizen createdCitizen = malloc(sizeof(*createdCitizen));
    if(!createdCitizen){
        return NULL;
    }

    createdCitizen->citizenId = getId(toCopy);
    createdCitizen->age = getAge(toCopy);
    createdCitizen->yearsOfEducation = getEducation(toCopy);
    createdCitizen->isCandidate = getCandidateStat(toCopy);

    createdCitizen->name = malloc(sizeof(strlen(getName(toCopy)+1)));
    if(!(createdCitizen->name)){
        free(createdCitizen);
        return NULL;
    }
    strcpy(createdCitizen->name, getName(toCopy));
    assert(createdCitizen->name);

    createdCitizen->preferences = listCopy(toCopy->preferences);
    assert(createdCitizen->preferences);

    return createdCitizen;
}

void citizenDestroy (Citizen toDestroy){
    if(!toDestroy)
        return;

    listDestroy(toDestroy->preferences);
    free(toDestroy->name);
    free(toDestroy);
}

int citizenCompare (Citizen first, Citizen second);

int getId (Citizen toGet);
int getAge (Citizen toGet);
int getEducation (Citizen toGet);
char* getName (Citizen toGet);
bool getCandidateStat (Citizen toGet);
List getPreferenceList (Citizen toGet);

CitizenResult addPreference (Citizen addTo, Citizen candidate, int priority);
CitizenResult clearPreference (Citizen clearTo, Citizen candidate);
CitizenResult makeCandidate (Citizen toMake);
CitizenResult clearCandidate (Citizen toClear);
