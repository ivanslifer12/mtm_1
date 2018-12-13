//
// Created by Bar The magical on 28/11/2018.
//

#include "city.h"
#include "candidate.h"
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


City cityCreate(int cityId, const char* cityName){
    if( cityId < 0 || !cityName){
        return NULL;
    } //mtm elections should check that

    City createdCity = malloc(sizeof(*createdCity));
    if(!createdCity){
        return NULL;
    }

    createdCity->cityId = cityId;
    createdCity->name = strdup(cityName);


    createdCity->citizens =
            uniqueOrderedListCreate((void*(*)(void*))citizenCopy,
                    (void(*)(void*))citizenDestroy,
                    (bool(*)(void*, void*))citizenIsEqual,
                    (bool(*)(void*, void*))citizenIsGreater);
    createdCity->candidates =
            uniqueOrderedListCreate((void*(*)(void*))candidateCopy,
                    (void(*)(void*))candidateDestroy,
                    (bool(*)(void*, void*))candidateIsEqual,
                    (bool(*)(void*, void*))candidateIsGreater);

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
    createdCity->name = strdup(toCopy->name);
    createdCity->citizens = uniqueOrderedListCopy(toCopy->citizens);
    createdCity->candidates = uniqueOrderedListCopy(toCopy->candidates);
    return createdCity;
}

void cityDestroy(City toDestroy){
    if(!toDestroy){
        return;
    }

    assert(toDestroy->name);
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


CityResult cityAddCitizen (City addTo, int citizenId, const char* citizenName,
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

    char* candidateName;
    CitizenResult nameResult = getName(candidate, &candidateName);
    if(nameResult != CITIZEN_SUCCESS){
        return CITY_MEMORY_ERROR;
    }

    Candidate candidateToAdd = candidateCreate(candidateId, candidateName);
    if(!candidateToAdd){
        free(candidateName);
        return CITY_MEMORY_ERROR;
    }

    CitizenResult makeResult = makeCandidate(candidate);
    if(makeResult == CITIZEN_AGE_NOT_APPROPRIATE){
        candidateDestroy(candidateToAdd);
        free(candidateName);
        return CITY_AGE_NOT_APPROPRIATE;
    }
    if(makeResult == CITIZEN_IS_ALREADY_CANDIDATE){
        candidateDestroy(candidateToAdd);
        free(candidateName);
        return CITY_CITIZEN_ALREADY_CANDIDATE;
    }

    assert(makeResult == CITIZEN_SUCCESS);

    uniqueOrderedListInsert(cityToMakeIn->candidates, candidateToAdd);

    candidateDestroy(candidateToAdd);
    free(candidateName);
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
                clearPreference(citizenIterator, candidateId);
        assert(clearResult != CITIZEN_NULL_ARGUMENT);
        //its fine that we might use clearPreference on the candidate itself
        //or citizens that don't prefer the candidate, it won't work anyways
        citizenIterator = uniqueOrderedListGetNext(toWithdrawIn->citizens);
    }

    CitizenResult clearResult = clearCandidate(candidateToWithdraw);
    assert(clearResult == CITIZEN_SUCCESS);

    Candidate candidate = candidateCreate(candidateId,"PLACEHOLDER");
    uniqueOrderedListRemove(toWithdrawIn->candidates, candidate);
    candidateDestroy(candidate);

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

    *namePtr = strdup(toGet->name);
    if(!*namePtr){
        return CITY_MEMORY_ERROR;
    }

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
            return CITY_SUCCESS;
        }

        iterator = uniqueOrderedListGetNext(toGetFrom->citizens);
    }
    return CITY_NO_SUCH_CITIZEN;
}

CityResult getACitizenName (City toGetFrom, int citizenId, char** namePtr){
    if(!toGetFrom || !namePtr){
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
    if(!toGetFrom || !agePtr){
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
    if(!toGetFrom || !educationPtr){
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

CityResult getACitizenHighestSupport (City toGetFrom, int citizenId, int* candidatePtr){
    if(!toGetFrom|| !candidatePtr){
        return CITY_NULL_ARGUMENT;
    }

    if(citizenId <0){
        return CITY_ILLEGAL_ID;
    }

    Citizen toGetFor;
    CityResult getCitizen = getACitizen(toGetFrom, citizenId, &toGetFor);
    if(getCitizen == CITY_NO_SUCH_CITIZEN){
        return CITY_NO_SUCH_CITIZEN;
    }

    assert(getCitizen == CITY_SUCCESS);

    CitizenResult getResult = getHighestSupport(toGetFor, candidatePtr);
    if(getResult == CITIZEN_SUPPORT_DOESNT_EXIST){
        return CITY_SUPPORT_DOESNT_EXIST;
    }

    assert(getResult == CITIZEN_SUCCESS);
    return CITY_SUCCESS;
}

CityResult getCandidateListCopy (City toGetFrom, UniqueOrderedList* listPtr){
    if(!toGetFrom || !listPtr){
        return CITY_NULL_ARGUMENT;
    }

    *listPtr = uniqueOrderedListCopy(toGetFrom->candidates);
    return CITY_SUCCESS;
}

CityResult getCitizenListCopy (City toGetFrom, UniqueOrderedList* listPtr){
    if(!toGetFrom || !listPtr){
        return CITY_NULL_ARGUMENT;
    }

    *listPtr = uniqueOrderedListCopy(toGetFrom->citizens);
    return CITY_SUCCESS;
}


CityResult addSupport (City toAddIn, int citizenId, int candidateId, int priority){
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

    bool candidateStat;
    getCandidateStat(candidate, &candidateStat);
    if(!candidateStat){
        return CITY_CITIZEN_IS_NOT_A_CANDIDATE;
    }

    CitizenResult addResult = addPreference(citizenToAddTo, candidateId, priority);

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

    if(citizenId <0 || candidateId <0){
        return CITY_ILLEGAL_ID;
    }


    Candidate toClearSupport = candidateCreate(candidateId, "PLACEHOLDER");
    //since the equals function for candidate only compares id it doesn't matter what name we give
    if(!toClearSupport){
        return CITY_MEMORY_ERROR;
    }

    if(!uniqueOrderedListContains(toClearIn->candidates, toClearSupport)){
        candidateDestroy(toClearSupport);
        return CITY_NO_SUCH_CANDIDATE;
    }

    candidateDestroy(toClearSupport);

    if(citizenId == candidateId){
        return CITY_MUST_SUPPORT;
    }

    Citizen toClearFrom;
    CityResult getResult = getACitizen(toClearIn, citizenId, &toClearFrom);
    if(getResult == CITY_NO_SUCH_CITIZEN){
        return CITY_NO_SUCH_CITIZEN;
    }
    assert(getResult == CITY_SUCCESS);

    CitizenResult clearResult = clearPreference(toClearFrom, candidateId);

    if(clearResult == CITIZEN_SUPPORT_DOESNT_EXIST){
        return CITY_SUPPORT_DOESNT_EXIST;
    }

    assert(clearResult == CITIZEN_SUCCESS);
    return CITY_SUCCESS;

}
