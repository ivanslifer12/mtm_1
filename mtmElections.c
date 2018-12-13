//
// Created by Bar The magical on 28/11/2018.
//

#include <assert.h>
#include "mtm_elections.h"
#include "city.h"
#include "candidate.h"
#include "citizen.h"
#include "mtm_print.h"

struct mtm_elections_t {
    UniqueOrderedList cities;
};

MtmElectionsResult getACity(MtmElections mtmElections, int cityId,
                            City *cityPtr);

MtmElectionsResult getMayorOfCity(MtmElections mtmElections,
                                                 int cityId, int *mayor,
                                                 RankFunc rank,
                                                 void *auxilaryData);

MtmElectionsResult printMayor (MtmElections mtmElections, int cityId,
        int mayorId, const char* filename);

MtmElections mtmElectionsCreate() {
    MtmElections createdElections = malloc(sizeof(*createdElections));
    if (!createdElections) {
        return NULL;
    }

    createdElections->cities =
            uniqueOrderedListCreate((void *(*)(void *)) cityCopy,
                                    (void (*)(void *)) cityDestroy,
                                    (bool (*)(void *, void *)) cityIsEqual,
                                    (bool (*)(void *, void *)) cityIsGreater);

    return createdElections;
}

void mtmElectionsDestroy(MtmElections mtmElections) {
    if (!mtmElections) {
        return;
    }

    uniqueOrderedListDestroy(mtmElections->cities);
    free(mtmElections);
}

MtmElectionsResult mtmElectionsAddCity(MtmElections mtmElections,
                                       const char *cityName, int cityId) {
    if (!mtmElections || !cityName) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }

    if (cityId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    City cityToAdd = cityCreate(cityId, cityName);
    if (!cityToAdd) {
        return MTM_ELECTIONS_MEMORY_ERROR;
    }

    UniqueOrderedListResult insertResult = uniqueOrderedListInsert(mtmElections->cities, cityToAdd);
    cityDestroy(cityToAdd);

    if (insertResult == UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS) {
        return MTM_ELECTIONS_CITY_ALREADY_EXISTS;
    }

    if (insertResult == UNIQUE_ORDERED_LIST_OUT_OF_MEMORY) {
        return MTM_ELECTIONS_MEMORY_ERROR;
    }

    assert(insertResult == UNIQUE_ORDERED_LIST_SUCCESS);
    return MTM_ELECTIONS_SUCCESS;
}

MtmElectionsResult getACity(MtmElections mtmElections, int cityId, City *cityPtr) {
    if (!mtmElections || !cityPtr) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }

    if (cityId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    City iterator = uniqueOrderedListGetLowest(mtmElections->cities);
    while (iterator != NULL) {
        int iteratorId;
        cityGetId(iterator, &iteratorId);
        if (iteratorId == cityId) {
            *cityPtr = iterator;
            return MTM_ELECTIONS_SUCCESS;
        }

        iterator = uniqueOrderedListGetNext(mtmElections->cities);
    }

    return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
}

MtmElectionsResult mtmElectionsAddCitizen(MtmElections mtmElections, const char *citizenName,
                                          int citizenId, int citizenAge, int yearsOfEducation, int cityId) {
    if (!citizenName || !mtmElections) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (citizenId < 0 || cityId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    if (citizenAge <= 0) {
        return MTM_ELECTIONS_ILLEGAL_AGE;
    }
    if (yearsOfEducation < 0) {
        return MTM_ELECTIONS_ILLEGAL_NUMBER_OF_YEARS;
    }

    City iterator = uniqueOrderedListGetLowest(mtmElections->cities);
    while (iterator != NULL) {
        if (cityContains(iterator, citizenId)) {
            return MTM_ELECTIONS_CITIZEN_ALREADY_EXISTS;
        }

        iterator = uniqueOrderedListGetNext(mtmElections->cities);
    }

    City toAddTo;
    MtmElectionsResult getResult = getACity(mtmElections, cityId, &toAddTo);
    if (getResult == MTM_ELECTIONS_CITY_DOES_NOT_EXIST) {
        return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
    }
    assert(getResult == MTM_ELECTIONS_SUCCESS);

    CityResult addResult = cityAddCitizen(toAddTo, citizenId, citizenName, yearsOfEducation, citizenAge);
    if (addResult == CITY_MEMORY_ERROR) {
        return MTM_ELECTIONS_MEMORY_ERROR;
    }
    assert(addResult == CITY_SUCCESS);
    return MTM_ELECTIONS_SUCCESS;
}

MtmElectionsResult MtmElectionsCitizenGetName(MtmElections mtmElections, int citizenId, char **name) {
    if (!mtmElections || !name) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }

    if (citizenId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    City iterator = uniqueOrderedListGetLowest(mtmElections->cities);
    while (iterator != NULL) {
        CityResult getResult = getACitizenName(iterator, citizenId, name);
        if (getResult == CITY_SUCCESS) {
            return MTM_ELECTIONS_SUCCESS;
        }

        iterator = uniqueOrderedListGetNext(mtmElections->cities);
    }

    return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
}

MtmElectionsResult MtmElectionsCitizenGetCity(MtmElections mtmElections, int citizenId, int *cityId) {
    if (!mtmElections || !cityId) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }

    if (citizenId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    City iterator = uniqueOrderedListGetLowest(mtmElections->cities);
    while (iterator != NULL) {
        if (cityContains(iterator, citizenId)) {
            CityResult getResult = cityGetId(iterator, cityId);
            assert(getResult == CITY_SUCCESS);
            return MTM_ELECTIONS_SUCCESS;
        }

        iterator = uniqueOrderedListGetNext(mtmElections->cities);
    }

    return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
}

MtmElectionsResult MtmElectionsCitizenGetAge(MtmElections mtmElections, int citizenId, int *age) {
    if (!mtmElections || !age) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }

    if (citizenId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    City iterator = uniqueOrderedListGetLowest(mtmElections->cities);
    while (iterator != NULL) {
        CityResult getResult = getACitizenAge(iterator, citizenId, age);
        if (getResult == CITY_SUCCESS) {
            return MTM_ELECTIONS_SUCCESS;
        }

        iterator = uniqueOrderedListGetNext(mtmElections->cities);
    }

    return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
}

MtmElectionsResult MtmElectionsCitizenGetEducation(MtmElections mtmElections, int citizenId, int *yearsOfEducation) {
    if (!mtmElections || !yearsOfEducation) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }

    if (citizenId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    City iterator = uniqueOrderedListGetLowest(mtmElections->cities);
    while (iterator != NULL) {
        CityResult getResult = getACitizenEducation(iterator, citizenId, yearsOfEducation);
        if (getResult == CITY_SUCCESS) {
            return MTM_ELECTIONS_SUCCESS;
        }

        iterator = uniqueOrderedListGetNext(mtmElections->cities);
    }

    return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
}

MtmElectionsResult mtmElectionsAddCandidate(MtmElections mtmElections, int candidateId, int cityId) {
    if (!mtmElections) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }

    if (candidateId < 0 || cityId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    City cityToMakeIn;
    MtmElectionsResult getResult = getACity(mtmElections, cityId, &cityToMakeIn);
    if (getResult == MTM_ELECTIONS_CITY_DOES_NOT_EXIST) {
        return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
    }

    CityResult makeResult = cityMakeCandidate(cityToMakeIn, candidateId);
    if (makeResult == CITY_CITIZEN_ALREADY_CANDIDATE) {
        return MTM_ELECTIONS_CANDIDATE_ALREADY_EXISTS;
    }

    if (makeResult == CITY_NO_SUCH_CITIZEN) {
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }

    if (makeResult == CITY_AGE_NOT_APPROPRIATE) {
        return MTM_ELECTIONS_AGE_NOT_APPROPRIATE;
    }

    if (makeResult == CITY_MEMORY_ERROR) {
        return MTM_ELECTIONS_MEMORY_ERROR;
    }

    assert(makeResult == CITY_SUCCESS);
    return MTM_ELECTIONS_SUCCESS;
}

MtmElectionsResult mtmElectionsWithdrawCandidate(MtmElections mtmElections, int candidateId, int cityId) {
    if (!mtmElections) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }

    if (candidateId < 0 || cityId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    City cityToMakeIn;
    MtmElectionsResult getResult = getACity(mtmElections, cityId, &cityToMakeIn);
    if (getResult == MTM_ELECTIONS_CITY_DOES_NOT_EXIST) {
        return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
    }


    CityResult withdrawResult = cityWithdrawCandidate(cityToMakeIn, candidateId);
    if (withdrawResult == CITY_CITIZEN_IS_NOT_A_CANDIDATE ||
        withdrawResult == CITY_NO_SUCH_CANDIDATE) {
        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
    }

    if (withdrawResult == CITY_MEMORY_ERROR) {
        return MTM_ELECTIONS_MEMORY_ERROR;
    }

    assert(withdrawResult == CITY_SUCCESS);
    return MTM_ELECTIONS_SUCCESS;
}

MtmElectionsResult
mtmElectionsSupportCandidate(MtmElections mtmElections, int citizenId, int candidateId, int priority) {

    if (!mtmElections) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (candidateId < 0 || citizenId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    if (priority < 0) {
        return MTM_ELECTIONS_ILLEGAL_PRIORITY;
    }

    City citizenCity;

    int citizenCityId;
    int candidateCityId;
    MtmElectionsResult citizenGet = MtmElectionsCitizenGetCity(mtmElections, citizenId, &citizenCityId);
    MtmElectionsResult candidateGet = MtmElectionsCitizenGetCity(mtmElections, candidateId, &candidateCityId);
    if (citizenGet == MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST) {
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
    if (candidateGet == MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST) {
        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
    }
    assert(citizenGet == MTM_ELECTIONS_SUCCESS);
    assert(candidateGet == MTM_ELECTIONS_SUCCESS);


    if (candidateCityId!=citizenCityId) {
        return MTM_ELECTIONS_NOT_SAME_CITY;
    }

    getACity(mtmElections, citizenCityId, &citizenCity);

    CityResult addResult = addSupport(citizenCity, citizenId, candidateId, priority);

    if (addResult == CITY_MEMORY_ERROR) {
        return MTM_ELECTIONS_MEMORY_ERROR;
    }
    if (addResult == CITY_NO_SUCH_CANDIDATE) {
        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
    }
    if (addResult == CITY_SUPPORT_EXISTS) {
        return MTM_ELECTIONS_ALREADY_SUPPORTED;
    }
    if (addResult == CITY_CAN_NOT_SUPPORT) {
        return MTM_ELECTIONS_CAN_NOT_SUPPORT;
    }
    if (addResult == CITY_PRIORITY_EXISTS) {
        return MTM_ELECTIONS_PRIORITY_EXISTS;
    }

    assert(addResult == CITY_SUCCESS);
    return MTM_ELECTIONS_SUCCESS;
}

MtmElectionsResult mtmElectionsCancelSupport(MtmElections mtmElections, int citizenId, int candidateId) {

    if (!mtmElections) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (candidateId < 0 || citizenId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    City citizenCity;
    City candidateCity;
    MtmElectionsResult citizenGet = getACity(mtmElections, citizenId, &citizenCity);
    MtmElectionsResult candidateGet = getACity(mtmElections, candidateId, &candidateCity);
    if (citizenGet == MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST) {
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
    if (candidateGet == MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST) {
        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
    }
    assert(citizenGet == MTM_ELECTIONS_SUCCESS);
    assert(candidateGet == MTM_ELECTIONS_SUCCESS);

    if (!cityIsEqual(citizenCity, candidateCity)) {
        return MTM_ELECTIONS_NOT_SAME_CITY;
    }

    CityResult clearResult = clearSupport(citizenCity, citizenId, candidateId);

    if (clearResult == CITY_MEMORY_ERROR) {
        return MTM_ELECTIONS_MEMORY_ERROR;
    }
    if (clearResult == CITY_NO_SUCH_CANDIDATE) {
        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
    }
    if (clearResult == CITY_SUPPORT_DOESNT_EXIST) {
        return MTM_ELECTIONS_NOT_SUPPORTED;
    }
    if (clearResult == CITY_MUST_SUPPORT) {
        return MTM_ELECTIONS_MUST_SUPPORT;
    }

    assert(clearResult == CITY_SUCCESS);
    return MTM_ELECTIONS_SUCCESS;
}

MtmElectionsResult mtmElectionsChangeAddress(MtmElections mtmElections, int citizenId, int cityId) {
    if (!mtmElections) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (citizenId < 0 || cityId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    int cityIdToMoveFrom;
    City toMoveFrom;
    City toMoveTo;
    MtmElectionsResult idGetResult = MtmElectionsCitizenGetCity(mtmElections, citizenId, &cityIdToMoveFrom);
    MtmElectionsResult cityGetResult = getACity(mtmElections, cityId, &toMoveTo);
    if (idGetResult == MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST) {
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
    if (cityGetResult == MTM_ELECTIONS_CITY_DOES_NOT_EXIST) {
        return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
    }
    assert(idGetResult == MTM_ELECTIONS_SUCCESS);
    assert(cityGetResult == MTM_ELECTIONS_SUCCESS);

    MtmElectionsResult getResult = getACity(mtmElections, cityIdToMoveFrom, &toMoveFrom);
    assert(getResult == MTM_ELECTIONS_SUCCESS);

    int citizenAge;
    int citizenEducation;
    char *citizenName;
    CityResult getEducation = getACitizenEducation(toMoveFrom, citizenId, &citizenEducation);
    CityResult getAge = getACitizenAge(toMoveFrom, citizenId, &citizenAge);
    CityResult getName = getACitizenName(toMoveFrom, citizenId, &citizenName);
    if (getName == CITY_MEMORY_ERROR) {
        return MTM_ELECTIONS_MEMORY_ERROR;
    }
    assert(getAge == CITY_SUCCESS);
    assert(getEducation == CITY_SUCCESS);
    assert(getName == CITY_SUCCESS);


    CityResult addResult = cityAddCitizen(toMoveTo, citizenId, citizenName, citizenEducation, citizenAge);
    if (addResult == CITY_MEMORY_ERROR) {
        return MTM_ELECTIONS_MEMORY_ERROR;
    }
    assert(addResult == CITY_SUCCESS);

    CityResult removeResult = cityRemoveCitizen(toMoveFrom, citizenId);
    assert(removeResult == CITY_SUCCESS);

    free(citizenName);

    return MTM_ELECTIONS_SUCCESS;
}

int mtmElectionsRankByAge(MtmElections mtmElections, int citizen, void *pAge) {
    if (!mtmElections || !pAge || citizen < 0) {
        return 0;
    }

    int citizenAge;
    MtmElectionsResult getResult = MtmElectionsCitizenGetAge(mtmElections, citizen, &citizenAge);
    if (getResult != MTM_ELECTIONS_SUCCESS) {
        return 0;
    }

    return (citizenAge / (*((int *) (pAge))));
}

UniqueOrderedList mtmElectionsPerformElections(MtmElections mtmElections,
                                               RankFunc rank, void *auxilaryData, const char *filename) {
    if (!filename || !mtmElections || !auxilaryData) {
        return NULL;
    }

    UniqueOrderedList winners =
            uniqueOrderedListCreate((void *(*)(void *)) candidateCopy,
                                    (void (*)(void *)) candidateDestroy,
                                    (bool (*)(void *, void *)) candidateIsEqual,
                                    (bool (*)(void *, void *)) candidateIsGreater);

    City iterator = uniqueOrderedListGetLowest(mtmElections->cities);
    while (iterator != NULL) {

        int mayorId;
        int cityId;
        cityGetId(iterator, &cityId);

        MtmElectionsResult electionsResult =
                getMayorOfCity(mtmElections, cityId, &mayorId, rank, auxilaryData);

        if (electionsResult == MTM_ELECTIONS_SUCCESS) {

            if(printMayor(mtmElections, cityId, mayorId, filename) == MTM_ELECTIONS_FILE_ERROR){
                mtmPrintFileError(filename, stderr);
            }

            char* mayorName;
            getACitizenName(iterator, mayorId, &mayorName);
            Candidate mayor = candidateCreate(mayorId, mayorName);
            uniqueOrderedListInsert(winners, mayor);
            candidateDestroy(mayor);
        }

        iterator = uniqueOrderedListGetNext(mtmElections->cities);
    }

    return winners;
}

MtmElectionsResult mtmElectionsMayorOfCity(MtmElections mtmElections, int cityId, int *mayor, const char* filename) {
    if(!filename){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    MtmElectionsResult result = getMayorOfCity(mtmElections, cityId, mayor, NULL, (void*) 1);
    if(result != MTM_ELECTIONS_SUCCESS){
        return result;
    }

    printMayor(mtmElections, cityId, *mayor, filename);
    return MTM_ELECTIONS_SUCCESS;
}

MtmElectionsResult printMayor (MtmElections mtmElections, int cityId, int mayorId, const char* filename){
    if(!mtmElections || !filename){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if(cityId <0 || mayorId <0){
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    FILE* stream = fopen(filename, "a");
    if(!stream){
        return MTM_ELECTIONS_FILE_ERROR;
    }

    City toElectIn;
    int age, yearsOfEducation;
    char* citizenName;
    char* cityName;
    getACity(mtmElections, cityId, &toElectIn);
    getACitizenName(toElectIn, mayorId, &citizenName);
    getACitizenAge(toElectIn, mayorId, &age);
    getACitizenEducation(toElectIn, mayorId, &yearsOfEducation);
    cityGetName(toElectIn, &cityName);

    mtmPrintMayorDetails(mayorId, age, yearsOfEducation, citizenName, cityName, cityId, stream);
    fclose(stream);

    free(cityName);
    free(citizenName);
    return MTM_ELECTIONS_SUCCESS;
}

MtmElectionsResult getMayorOfCity(MtmElections mtmElections,
                                                 int cityId, int* mayor, RankFunc rank, void *auxilaryData) {
    if(!mtmElections || !mayor || !auxilaryData){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if(cityId < 0){
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    City toElectIn;
    MtmElectionsResult getResult = getACity(mtmElections, cityId, &toElectIn);
    if(getResult == MTM_ELECTIONS_CITY_DOES_NOT_EXIST){
        return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
    }

    assert(getResult == MTM_ELECTIONS_SUCCESS); //TODO - delete this assert

    UniqueOrderedList candidateList, citizenList;
    getCandidateListCopy(toElectIn, &candidateList);

    int numberOfCandidates = uniqueOrderedListSize(candidateList);
    if(numberOfCandidates == 0){
        uniqueOrderedListDestroy(candidateList);
        return MTM_ELECTIONS_NO_CANDIDATES_IN_CITY;
    }

    getCitizenListCopy(toElectIn, &citizenList);
    Candidate candidateIterator = uniqueOrderedListGetLowest(candidateList);

    int mostVotes = -1;
    while(candidateIterator != NULL){
        int currentVotes = 0;

        int candidateId;
        candidateGetCandidateId(candidateIterator, &candidateId);

        Citizen citizenIterator = uniqueOrderedListGetLowest(citizenList);
        while(citizenIterator!=NULL){
            int citizenAge, citizenId, highestSupportId;

            CitizenResult supportResult = getHighestSupport(citizenIterator,
                    &highestSupportId);
            getAge(citizenIterator, &citizenAge);
            getId(citizenIterator, &citizenId);

            if(supportResult == CITIZEN_SUCCESS &&
            highestSupportId == candidateId){
                if(rank!=NULL){
                    currentVotes += (*(rank))(mtmElections, citizenId,
                            auxilaryData);
                }else if(citizenAge >= 17){
                    currentVotes += 1;
                }
            }
            citizenIterator = uniqueOrderedListGetNext(citizenList);
        }

        if(currentVotes > mostVotes){
            mostVotes = currentVotes;
            *mayor= candidateId;
        }

        candidateIterator = uniqueOrderedListGetNext(candidateList);
    }

    uniqueOrderedListDestroy(candidateList);
    uniqueOrderedListDestroy(citizenList);

    return MTM_ELECTIONS_SUCCESS;
}



