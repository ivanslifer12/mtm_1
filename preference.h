//
// Created by Bar The magical on 30/11/2018.
//




#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef PREFERENCE_H
#define PREFERENCE_H

/** struct for holding the priority and appropriate candidate for the citizen struct */
typedef struct preference_t* Preference;

/** possible return values */
typedef enum PreferenceResult_t{
    PREFERENCE_NULL_ARGUMENT,
    PREFERENCE_MEMORY_ERROR,
    PREFERENCE_SUCCESS
}PreferenceResult;

/** creates a new preference
 * @param candidateName - the candidate's name
 * @param candidateId- the candidate's id
 * @param the priority of that preference
 * @returns a preference if successful, if not - NULL
 * if all parameters are ok NULL means memory error*/
Preference preferenceCreate (char* candidateName, int candidateId, int priority);

/** copies a preference
 * @param toCopy - a preference to copy
 * @returns an identical preference if successful, if not - NULL
 * if all parameters are ok NULL means memory error*/
Preference preferenceCopy (Preference toCopy);

/** destroys a preference by freeing all of its memory usage
 * @param toDestroy - preference to destroy*/
void preferenceDestroy (Preference toDestroy);

/** given a pointer, returns the candidate name of the preference in it
 * @param toGet - the preference from which we want to get the name
 * @param namePtr - the pointer to hold the name
 * @returns PREFERENCE_NULL_ARGUMENT - if toGet or idPtr are NULL
 * Important - it is the caller's responsibility to allocate space for the actual pointer, NULL pointer's will not work*
 * @returns PREFERENCE_MEMORY_ERROR - if something went wrong with allocating memory for the name
 * @returns PREFERENCE_SUCCESS - if all went well */
PreferenceResult preferenceGetCandidateName (Preference toGet, char** namePtr);

/** given a pointer, returns the candidate id of the preference in it
 * @param toGet - the preference from which we want to get the id
 * @param idPtr - the pointer to hold the id
 * @returns PREFERENCE_NULL_ARGUMENT - if toGet or idPtr are NULL
 * Important - it is the caller's responsibility to allocate space for the actual pointer, NULL pointer's will not work*
 * @returns PREFERENCE_SUCCESS - if all went well */
PreferenceResult preferenceGetCandidateId (Preference toGet, int* idPtr);

 /** identical to preferenceGetCandidateId, only with priority */
PreferenceResult preferenceGetPriority (Preference toGet, int* priorityPtr);

/** compare function to be used by the list struct.
 * @param two preferences, order matters!
 * @returns a positive integer if second has a larger priority than first, 0 if they are equal, and negative int if second is smaller*/
int preferenceComparePriority (Preference firstToComp, Preference secondToComp);

/** same as compare by priority, only with candidate if */
int preferenceCompareId (Preference firstToComp, Preference secondToComp);


#endif //MTM_1_PREFERENCE_H
