//
// Created by Bar The magical on 28/11/2018.
//

#include "citizen.h"
#include <stdlib.h>
#include <assert.h>


struct citizen_t{
    char* name;
    int age;
    int citizenId;
    int yearsOfEducation;
    bool isCandidate;
    List prefrences; /** list of a struct called "prefrence" */
};


CitizenResult citizenCreate (const char* citizenName, int citizenId, int citizenAge, int yearsOfEducation){

    if(!citizenName||!citizenId||!citizenAge||!yearsOfEducation){
        return CITIZEN_NULL_ARGUMENT;
    }
    if(citizenId < 0){
        return CITIZEN_ILLEGAL_ID;
    }
    if(citizenAge <= 0){
        return CITIZEN_ILLEGAL_AGE;
    }
    if(yearsOfEducation < 0){
        return CITIZEN_ILLEGAL_NUMBER_OF_YEARS;
    }
    Citizen createdCitizen = malloc(sizeof(*createdCitizen));
    if(!createdCitizen){
        return CITIZEN_MEMORY_ERROR;
    }

    createdCitizen -> citizenId = citizenId;
    createdCitizen -> age = citizenAge;
    createdCitizen -> isCandidate = false;
    createdCitizen -> yearsOfEducation = yearsOfEducation;

    //TODO - might be a problem with the functions pointers not being void*
    createdCitizen->prefrences = listCreate(&preferrenceCopy,&preferenceDestroy);
    if(!createdCitizen->prefrences){
        free(createdCitizen);
        return CITIZEN_MEMORY_ERROR;
    }

    createdCitizen->name = malloc(sizeof(*citizenName));
    if(!createdCitizen->name){
        free(createdCitizen);
        return CITIZEN_MEMORY_ERROR;
    }

    createdCitizen->name = _strdup(citizenName);
    assert(createdCitizen->name);

    return CITIZEN_SUCCESS;
}

CitizenResult citizenCopy (Citizen citizenToCopy){

    if(!citizenToCopy){
        return CITIZEN_NULL_ARGUMENT;
    }

    Citizen createdCitizen = malloc(sizeof(*createdCitizen));
    if(!createdCitizen){
        return CITIZEN_MEMORY_ERROR;
    }

    CitizenResult getId = citizenGetId(citizenToCopy, &(createdCitizen ->citizenId));
    if(getId != CITIZEN_SUCCESS){
        free(createdCitizen);
        return getId;
    }

    CitizenResult getAge = citizenGetAge(citizenToCopy, &(createdCitizen->age));
    if(getAge != CITIZEN_SUCCESS){
        free(createdCitizen);
        return getAge;
    }

    CitizenResult getYears = citizenGetYearsOfEducation(citizenToCopy, &(createdCitizen->yearsOfEducation));
    if(getYears != CITIZEN_SUCCESS){
        free(createdCitizen);
        return getYears;
    }

    CitizenResult getName = citizenGetName(citizenToCopy, &(createdCitizen->name));
    if(getName != CITIZEN_SUCCESS){
        free(createdCitizen);
        return getName;
    }

    CitizenResult getCandidate = citizenGetCandidate(citizenToCopy, &(createdCitizen->isCandidate));
    if(getCandidate != CITIZEN_SUCCESS){
        free(createdCitizen);
        return getCandidate;
    }

    CitizenResult getPreferences = citizenGetPreferencesList(citizenToCopy, &(createdCitizen->prefrences));
    if(getPreferences != CITIZEN_SUCCESS){
        free(createdCitizen);
        return getPreferences;
    }

    return CITIZEN_SUCCESS;

}

CitizenResult citizenGetName (Citizen citizenToGet, char** namePtr){
    //add validity chk

    namePtr = malloc(sizeof(*(citizenToGet->name)));
    if(!namePtr){
        return CITIZEN_MEMORY_ERROR;
    }

    *namePtr = _strdup(citizenToGet->name);
    return CITIZEN_SUCCESS;
}