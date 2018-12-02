//
// Created by Bar The magical on 28/11/2018.
//

#ifndef MTM_1_CITIZEN_H
#define MTM_1_CITIZEN_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "preference.h"
#include "list.h"

typedef struct citizen_t* Citizen;

typedef enum CitizenResult_t{
    CITIZEN_MEMORY_ERROR,
    CITIZEN_NULL_ARGUMENT,
    CITIZEN_ILLEGAL_ID,
    CITIZEN_ILLEGAL_AGE,
    CITIZEN_ILLEGAL_NUMBER_OF_YEARS,
   CITIZEN_IS_ALREADY_CANDIDATE,
    CITIZEN_SUPPORTS_EXISTS,
    CITIZEN_NOT_SUPPORTED,
    CITIZEN_CAN_NOT_SUPPORT,
    CITIZEN_MUST_SUPPORT,
    CITIZEN_AGE_NOT_APPROPRIATE,
    CITIZEN_PREFERENCE_EXISTS,
    CITIZEN_CANDIDATE_DOES_NOT_EXIST,
    CITIZEN_SUCCESS
}CitizenResult;

/** citizen maker */
CitizenResult citizenCreate (const char* citizenName, int citizenId, int citizenAge, int yearsOfEducation);

CitizenResult citizenCopy (Citizen citizenToCopy);

/** return a copy of the citizen's name through the pointer it gets*/
CitizenResult citizenGetName (Citizen citizenToGet, char** namePtr);

/** similar to getName, only returns age */
CitizenResult citizenGetAge (Citizen citizenToGet, int* agePtr);

CitizenResult citizenGetYearsOfEducation (Citizen citizenToGet, int* yearOfEducationPtr);

/** makes a citizen a candidate */
CitizenResult citizenSetCandidate (Citizen candidate);

/** removes a citizen candidacy */
CitizenResult citizenWithdrawCandidate (Citizen candidate);

/** returns whether a citizen is a candidate or not */
CitizenResult citizenGetCandidate (Citizen candidate, bool* candidatePtr);

/** returns a preference for a specific candidate */
CitizenResult citizenGetPreference (Citizen citizenToCheck, int candidateId, Preference* preferencePtr);

/** gets a copy of the list */
CitizenResult citizenGetPreferencesList (Citizen toGet, List* preferencesPtr);

/** adds a preference to a candidate */
CitizenResult citizenAddPreference (Citizen preferringCitizen, Citizen candidate, int priority);

CitizenResult citizenClearPreference (Citizen preferringCitizen, int candidateId);

/** deletes a citizen */
CitizenResult citizenDestroy (Citizen citizenToDestroy);

CitizenResult citizenGetId (Citizen citizenToGet, int* idPtr);

int citizenCompareId (Citizen firstToCompare, Citizen secondToCompare);

#endif //MTM_1_CITIZEN_H
