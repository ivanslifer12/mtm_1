//
// Created by Bar The magical on 28/11/2018.
//

#include "city.h"
#include "citizen.h"
#include "uniqueOrderedList/uniqueOrderedList.h"
#include <string.h>
#include <assert.h>

CityResult getACitizen (City toGetFrom, int citizenId, Citizen* citizenPtr);

struct city_t{
    int cityId;
    char* name;
    UniqueOrderedList citizens;
    UniqueOrderedList candidates;
};


City cityCreate(int cityId, char* cityName){
    if( cityId < 0 || !cityName){
        return NULL;
    } //mtm elections should check that

    City createdCity = malloc(sizeof(*createdCity));
    if(!createdCity){
        return NULL;
    }

    createdCity->cityId = cityId;
    createdCity->name = malloc(sizeof(*cityName));
    if(!createdCity->name){
        free(createdCity);
        return NULL;
    }

    strcpy(createdCity->name, cityName);

    createdCity->citizens =
            uniqueOrderedListCreate((void*(*)(void*))citizenCopy,
                    (void(*)(void*))citizenDestroy,
                    (bool(*)(void*, void*))citizenIsEqual,
                    (bool(*)(void*, void*))citizenIsGreater);
    createdCity->candidates = uniqueOrderedListCopy(createdCity->citizens);

    return createdCity;
}

City cityCopy(City toCopy){
    if(!toCopy){
        return NULL;
    }

    City createdCity = malloc(sizeof(*createdCity));
    if(!createdCity){
        return NULL;
    }

    createdCity->cityId = toCopy->cityId;
    createdCity->name = malloc(sizeof(*toCopy->name));
    if(!createdCity->name){
        free(createdCity);
        return NULL;
    }
    strcpy(createdCity->name, toCopy->name);
    createdCity->citizens = uniqueOrderedListCopy(toCopy->citizens);
    createdCity->candidates = uniqueOrderedListCopy(toCopy->candidates);
    return createdCity;
}

void cityDestroy(City toDestroy){
    if(!toDestroy){
        return;
    }

    free(toDestroy->name);
    uniqueOrderedListDestroy(toDestroy->citizens);
    uniqueOrderedListDestroy(toDestroy->candidates);
    free(toDestroy);
}

bool cityIsEqual(City firstToCompare, City secondToCompare){
    return (firstToCompare->cityId == secondToCompare->cityId);
}

bool cityIsGreater (City firstToCompare, City secondToCompare){
    return (strcmp(firstToCompare->name, secondToCompare->name) >0 ||
            (strcmp(firstToCompare->name, secondToCompare->name) == 0 &&
             firstToCompare->cityId < secondToCompare->cityId));
}

bool cityContains (City toCheck, int citizenId){

    if(!toCheck || citizenId <0){
        return false;
    }

    Citizen toFind;
    CityResult getResult = getACitizen(toCheck, citizenId, &toFind);

    if(getResult == CITY_SUCCESS){
        return true;
    }

    return false;

}


CityResult cityAddCitizen (City addTo, int citizenId, char* citizenName,
        int yearsOfEducation, int age){

    if(!addTo || !citizenName){
        return CITY_NULL_ARGUMENT;
    }
    if(citizenId <0){
        return CITY_ILLEGAL_ID;
    }
    if(yearsOfEducation < 0){
        return CITY_ILLEGAL_NUMBER_OF_YEARS;
    }
    if(age <= 0){
        return CITY_ILLEGAL_AGE;
    }
    if(cityContains(addTo, citizenId)){
        return CITY_CITIZEN_ALREADY_EXISTS;
    }

    Citizen citizenToAdd = citizenCreate(citizenId, citizenName,
            yearsOfEducation, age);
    if(!citizenToAdd){
        return CITY_MEMORY_ERROR;
    }

    UniqueOrderedListResult insertResult =
            uniqueOrderedListInsert(addTo->citizens, citizenToAdd);
    citizenDestroy(citizenToAdd);

    if(insertResult != UNIQUE_ORDERED_LIST_SUCCESS){
        return CITY_MEMORY_ERROR;
    }

    return CITY_SUCCESS;
}

CityResult cityRemoveCitizen (City removeFrom, int citizenId){
    if(!removeFrom){
        return CITY_NULL_ARGUMENT;
    }
    if(citizenId < 0){
        return CITY_ILLEGAL_ID;
    }

    Citizen toRemove;
    CityResult getResult = getACitizen(removeFrom, citizenId, &toRemove);
    if(getResult == CITY_NO_SUCH_CITIZEN){
        return CITY_NO_SUCH_CITIZEN;
    }
    assert(getResult == CITY_SUCCESS);

    bool isCandidate;
    CitizenResult boolResult = getCandidateStat(toRemove, &isCandidate);
    assert(boolResult == CITIZEN_SUCCESS);
    if(isCandidate){
        cityWithdrawCandidate(removeFrom, citizenId);
    }

    uniqueOrderedListRemove(removeFrom->citizens, toRemove);
    assert(!toRemove);
    return CITY_SUCCESS;

}

CityResult cityMakeCandidate (City cityToMakeIn, int candidateId){

    if(!cityToMakeIn){
        return CITY_NULL_ARGUMENT;
    }

    if(candidateId < 0){
        return CITY_ILLEGAL_ID;
    }


    Citizen candidate;
    CityResult getResult = getACitizen(cityToMakeIn, candidateId, &candidate);
    if(getResult == CITY_NO_SUCH_CITIZEN){
        return CITY_NO_SUCH_CITIZEN;
    }

    CitizenResult makeResult = makeCandidate(candidate);
    if(makeResult == CITIZEN_AGE_NOT_APPROPRIATE){
        return CITY_AGE_NOT_APPROPRIATE;
    }
    if(makeResult == CITIZEN_IS_ALREADY_CANDIDATE){
        return CITY_CITIZEN_ALREADY_CANDIDATE;
    }

    assert(makeResult == CITIZEN_SUCCESS);
    uniqueOrderedListInsert(cityToMakeIn->candidates, candidate);
    return CITY_SUCCESS;
}

CityResult cityWithdrawCandidate (City toWithdrawIn, int candidateId){
    if(!toWithdrawIn){
        return CITY_NULL_ARGUMENT;
    }
    if(candidateId <0){
        return CITY_ILLEGAL_ID;
    }

    Citizen candidateToWithdraw;
    CityResult getResult = getACitizen(toWithdrawIn, candidateId, &candidateToWithdraw);
    if(getResult == CITY_NO_SUCH_CITIZEN){
        return CITY_NO_SUCH_CANDIDATE;
    }
    bool isCandidate;
    CitizenResult candidateResult = getCandidateStat(candidateToWithdraw, &isCandidate);
    assert(candidateResult == CITIZEN_SUCCESS);
    if(!isCandidate){
        return CITY_CITIZEN_IS_NOT_A_CANDIDATE;
    }

    //found the candidate, we now go over all the citizens and clear their
    // preference
    Citizen citizenIterator =
            uniqueOrderedListGetLowest(toWithdrawIn->citizens);
    while(citizenIterator != NULL){
        CitizenResult clearResult =
                clearPreference(citizenIterator, candidateToWithdraw);
        assert(clearResult != CITIZEN_NULL_ARGUMENT);
        //its fine that we might use clearPreference on the candidate itself
        //or citizens that don't prefer the candidate, it won't work anyways
        citizenIterator = uniqueOrderedListGetNext(toWithdrawIn->citizens);
    }

    CitizenResult clearResult = clearCandidate(candidateToWithdraw);
    assert(clearResult == CITIZEN_SUCCESS);
    uniqueOrderedListRemove(toWithdrawIn->candidates, candidateToWithdraw);
    return CITY_SUCCESS;
}


CityResult cityGetId (City toGet, int* idPtr){
    if(!toGet || !idPtr){
        return CITY_NULL_ARGUMENT;
    }

    *idPtr = toGet->cityId;
    return CITY_SUCCESS;
}

CityResult cityGetName (City toGet, char** namePtr){
    if(!toGet || !namePtr){
        return CITY_NULL_ARGUMENT;
    }

    *namePtr = malloc(sizeof(strlen(toGet->name)+1));
    if(!*namePtr){
        return CITY_MEMORY_ERROR;
    }

    strcpy(*namePtr, toGet->name);
    return CITY_SUCCESS;
}

CityResult getACitizen (City toGetFrom, int citizenId, Citizen* citizenPtr){

    if(!toGetFrom || !citizenPtr){
        return CITY_NULL_ARGUMENT;
    }

    if(citizenId <0){
        return CITY_ILLEGAL_ID;
    }

    Citizen iterator = uniqueOrderedListGetLowest(toGetFrom->citizens);
    int iteratorId;
    while(iterator!=NULL){
        CitizenResult getResult = getId(iterator, &iteratorId);
        assert(getResult == CITIZEN_SUCCESS);
        if(iteratorId == citizenId){
            *citizenPtr = iterator;
        }
    }
    return CITY_NO_SUCH_CITIZEN;
}

CityResult getACitizenName (City toGetFrom, int citizenId, char** namePtr){
    if(!toGetFrom || namePtr){
        return CITY_NULL_ARGUMENT;
    }

    if(citizenId <0){
        return CITY_ILLEGAL_ID;
    }

    Citizen toGetName;
    CityResult getResult = getACitizen(toGetFrom, citizenId, &toGetName);
    if(getResult == CITY_NO_SUCH_CITIZEN) {
        return CITY_NO_SUCH_CITIZEN;
    }
    assert(getResult == CITY_SUCCESS);

    CitizenResult nameResult = getName(toGetName, namePtr);
    assert(nameResult == CITIZEN_SUCCESS);
    return CITY_SUCCESS;
}

CityResult getACitizenAge (City toGetFrom, int citizenId, int* agePtr){
    if(!toGetFrom || agePtr){
        return CITY_NULL_ARGUMENT;
    }

    if(citizenId <0){
        return CITY_ILLEGAL_ID;
    }

    Citizen toGetAge;
    CityResult getResult = getACitizen(toGetFrom, citizenId, &toGetAge);
    if(getResult == CITY_NO_SUCH_CITIZEN) {
        return CITY_NO_SUCH_CITIZEN;
    }
    assert(getResult == CITY_SUCCESS);

    CitizenResult ageResult = getAge(toGetAge, agePtr);
    assert(ageResult == CITIZEN_SUCCESS);
    return CITY_SUCCESS;
}

CityResult getACitizenEducation (City toGetFrom, int citizenId, int* educationPtr){
    if(!toGetFrom || educationPtr){
        return CITY_NULL_ARGUMENT;
    }

    if(citizenId <0){
        return CITY_ILLEGAL_ID;
    }

    Citizen toGetEducation;
    CityResult getResult = getACitizen(toGetFrom, citizenId, &toGetEducation);
    if(getResult == CITY_NO_SUCH_CITIZEN) {
        return CITY_NO_SUCH_CITIZEN;
    }
    assert(getResult == CITY_SUCCESS);

    CitizenResult educationResult = getEducation(toGetEducation, educationPtr);
    assert(educationResult == CITIZEN_SUCCESS);
    return CITY_SUCCESS;
}


CityResult addSupport (City toAddIn, int citizenId, int candidateId,int priority){
    if(!toAddIn){
        return CITY_NULL_ARGUMENT;
    }

    if(citizenId < 0 || candidateId <0){
        return CITY_ILLEGAL_ID;
    }

    if(priority <0){
        return CITY_ILLEGAL_PRIORITY;
    }

    if(!cityContains(toAddIn, candidateId)){
        return CITY_NO_SUCH_CANDIDATE;
    }

    if(!cityContains(toAddIn, citizenId)){
        return CITY_NO_SUCH_CITIZEN;
    }

    Citizen citizenToAddTo;
    Citizen candidate;
    CityResult getCitizen = getACitizen(toAddIn, citizenId, &citizenToAddTo);
    CityResult getCandidate = getACitizen(toAddIn, candidateId, &candidate);
    assert(getCitizen == CITY_SUCCESS);
    assert(getCandidate == CITY_SUCCESS);

    CitizenResult addResult = addPreference(citizenToAddTo, candidate, priority);
    if(addResult == CITIZEN_IS_NOT_CANDIDATE){
        return CITY_NO_SUCH_CANDIDATE;
    }

    if(addResult == CITIZEN_SUPPORT_EXISTS){
        return CITY_SUPPORT_EXISTS;
    }

    if(addResult == CITIZEN_CAN_NOT_SUPPORT){
        return CITY_CAN_NOT_SUPPORT;
    }

    if(addResult == CITIZEN_PRIORITY_EXISTS){
        return CITY_PRIORITY_EXISTS;
    }

    if(addResult == CITIZEN_MEMORY_ERROR){
        return CITY_MEMORY_ERROR;
    }

    assert(addResult == CITIZEN_SUCCESS);

    return CITY_SUCCESS;
}

CityResult clearSupport (City toClearIn, int citizenId, int candidateId){
    if(!toClearIn){
        return CITY_NULL_ARGUMENT;
    }
}