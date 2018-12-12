//
// Created by Bar The magical on 28/11/2018.
//

#ifndef MTM_1_CITY_H
#define MTM_1_CITY_H

#include "citizen.h"
#include "uniqueOrderedList/uniqueOrderedList.h"

typedef struct city_t* City;

typedef enum CityResult_t{
    CITY_SUCCESS,
    CITY_MEMORY_ERROR,
    CITY_NULL_ARGUMENT,
    CITY_NO_SUCH_CITIZEN,
    CITY_CITIZEN_ALREADY_EXISTS,
    CITY_ILLEGAL_ID,
    CITY_NO_SUCH_CANDIDATE,
    CITY_ILLEGAL_NUMBER_OF_YEARS,
    CITY_ILLEGAL_AGE,
    CITY_AGE_NOT_APPROPRIATE,
    CITY_CITIZEN_ALREADY_CANDIDATE,
    CITY_CITIZEN_IS_NOT_A_CANDIDATE,
    CITY_ILLEGAL_PRIORITY,
    CITY_SUPPORT_EXISTS,
    CITY_CAN_NOT_SUPPORT,
    CITY_PRIORITY_EXISTS
}CityResult;

/** Basic Functions **/

City cityCreate(int cityId, char* cityName);

City cityCopy(City toCopy);

void cityDestroy(City toDestroy);

bool cityIsEqual(City firstToCompare, City secondToCompare);

bool cityIsGreater (City firstToCompare, City secondToCompare);

/** Citizen Management **/

bool cityContains (City toCheck, int citizenId);

CityResult cityAddCitizen (City addTo, int citizenId, char* citizenName,
        int yearsOfEducation, int age);

CityResult cityRemoveCitizen (City removeFrom, int citizenId);

CityResult cityMakeCandidate (City cityToMakeIn, int candidateId);

CityResult cityWithdrawCandidate (City toWithdrawIn, int candidateId);

/** Get Functions **/

CityResult cityGetId (City toGet, int* idPtr);

CityResult cityGetName (City toGet, char** namePtr);

CityResult getACitizenName (City toGetFrom, int citizenId, char** namePtr);

CityResult getACitizenAge (City toGetFrom, int citizenId, int* agePtr);

CityResult getACitizenEducation (City toGetFrom, int citizenId, int* educationPtr);

CityResult getACitizenHighestSupport (City toGetFrom, int citizenId, int* candidatePtr);

/** Election Functions **/

CityResult addSupport (City toAddIn, int citizenId, int candidateId, int priority);

CityResult clearSupport (City toClearIn, int citizenId, int candidateId);


#endif //MTM_1_CITY_H
