#ifndef MTM_1_PREFERENCE_H
#define MTM_1_PREFERENCE_H

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
    CITIZEN_PREFERENCE_EXISTS
}CitizenResult;

Citizen citizenCreate (int citizenId, const char* citizenName, int yearsOfEducation, int age);
Citizen citizenCopy (Citizen toCopy);
void citizenDestroy (Citizen toDestroy);
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

#endif