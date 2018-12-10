//
// Created by Bar The magical on 30/11/2018.
//




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef PREFERENCE_H
#define PREFERENCE_H

/** struct for holding the priority and appropriate candidate for the citizen
 * struct */
typedef struct preference_t* Preference;

/** possible return values */
typedef enum PreferenceResult_t{
    PREFERENCE_NULL_ARGUMENT,
    PREFERENCE_SUCCESS
}PreferenceResult;

/** creates a new preference
 * @param candidateId- the candidate's id
 * @param the priority of that preference
 * @returns a preference if successful, if not - NULL
 * if all parameters are ok NULL means memory error*/
Preference preferenceCreate (int candidateId, int priority);

/** copies a preference
 * @param toCopy - a preference to copy
 * @returns an identical preference if successful, if not - NULL
 * if all parameters are ok NULL means memory error*/
Preference preferenceCopy (Preference toCopy);

/** destroys a preference by freeing all of its memory usage
 * @param toDestroy - preference to destroy*/
void preferenceDestroy (Preference toDestroy);


/** given a pointer, returns the candidate id of the preference in it
 * @param toGet - the preference from which we want to get the id
 * @param idPtr - the pointer to hold the id
 * @returns PREFERENCE_NULL_ARGUMENT - if toGet or idPtr are NULL
 * @returns PREFERENCE_SUCCESS - if all went well */
PreferenceResult preferenceGetCandidateId (Preference toGet, int* idPtr);

 /** identical to preferenceGetCandidateId, only with priority */
PreferenceResult preferenceGetPriority (Preference toGet, int* priorityPtr);

/** compare function to be used by the uol struct.
 * @param two preferences, order matters!
 * @returns true if the first preference has a larger priority, otherwise false*/
bool preferenceComparePriority (Preference firstToComp, Preference secondToComp);

/** same as compare by priority, only with candidateid */
bool preferenceCompareId (Preference firstToComp, Preference secondToComp);

/** if either the priority or the candidate id are equal in both preferences
 * this returns true */
bool preferenceIsEqual (Preference firstToComp, Preference secondToComp);


#endif //MTM_1_PREFERENCE_H
