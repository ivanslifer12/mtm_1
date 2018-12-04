#ifndef CITIZEN_H
#define CITIZEN_H

#include "preference.h"
#include "list.h"

typedef struct citizen_t* Citizen;

typedef enum CitizenResult_t{
    CITIZEN_SUCCESS,
    CITIZEN_NULL_ARGUMENT,
    CITIZEN_ILLEGAL_ID,
    CITIZEN_ILLEGAL_AGE,
    CITIZEN_ILLEGAL_NUMBER_OF_YEARS,
    CITIZEN_IS_ALREADY_CANDIDATE,
    CITIZEN_SUPPORT_EXISTS,
    CITIZEN_AGE_NOT_APPROPRIATE,
    CITIZEN_CAN_NOT_SUPPORT,
    CITIZEN_MUST_SUPPORT,
    CITIZEN_SUPPORT_DOESNT_EXIST,
    CITIZEN_PRIORITY_EXISTS,
    CITIZEN_MEMORY_ERROR,
    CITIZEN_ILLEGAL_PRIORITY,
    CITIZEN_IS_NOT_CANDIDATE
}CitizenResult;

Citizen citizenCreate (int citizenId, const char* citizenName, int yearsOfEducation, int age);
Citizen citizenCopy (Citizen toCopy);
void citizenDestroy (Citizen toDestroy);
int citizenCompare (Citizen first, Citizen second);

CitizenResult getId (Citizen toGet, int* idPtr);
CitizenResult getAge (Citizen toGet, int* agePtr);
CitizenResult getEducation (Citizen toGet, int* educationPtr);
CitizenResult getName (Citizen toGet, char** namePtr);
CitizenResult getCandidateStat (Citizen toGet, bool* candidateStatPtr);
CitizenResult getPreferenceList (Citizen toGet, List* preferencesPtr);

CitizenResult addPreference (Citizen addTo, Citizen candidate, int priority);
CitizenResult clearPreference (Citizen clearTo, Citizen candidate);
CitizenResult makeCandidate (Citizen toMake);
CitizenResult clearCandidate (Citizen toClear);

#endif