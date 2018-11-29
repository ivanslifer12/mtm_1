//
// Created by Bar The magical on 28/11/2018.
//

#ifndef MTM_1_CITIZEN_H
#define MTM_1_CITIZEN_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct citizen_t* Citizen;

/** citizen maker
 * this function assumes all parameters are ok, since they are
 * checked within mtm elections anyways in order to know the return value
 * the only error can be a memory error */
Citizen citizenCreate (const char* citizenName, int citizenId, int citizenAge, int yearsOfEducation, int cityId);

/** return a copy of the citizen's name through the pointer it gets*/
/** once again, this function assumes the citizen it gets is valid and has a name */
/** so the only error can be a memeory error */
void getName (Citizen citizenToGet, char** namePtr);

/** similar to getName, only returns the city */
void getCity (Citizen citizenToGet, int* cityIdPtr);

void getAge (Citizen citizenToGet, int* agePtr);

void getYearsOfEducation (Citizen citizenToGet, int* yearOfEducationPtr);

/** makes a citizen a candidate */
void makeCandidate (Citizen candidate);

/** deletes a citizen */
void citizenDestroy (Citizen citizenToDestroy);


#endif //MTM_1_CITIZEN_H
