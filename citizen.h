#ifndef CITIZEN_H
#define CITIZEN_H
#include "preference.h"

/** struct - citizen. meant to be used by the city struct as a way of holding
 * all citizen data */
typedef struct citizen_t* Citizen;

/** possible return values */
typedef enum CitizenResult_t{
    CITIZEN_SUCCESS,
    CITIZEN_NULL_ARGUMENT,
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

/** BASIC FUNCTIONS **/

/** creates a new citizen
 *@param citizenId - non-negative integer
 *@param citizenName - string that contains the name of the citizen
 *@param yearsOfEducation - non-negative integer
 *@param age - positive integer
 *@returns a citizen if all parameters are valid and there are no memory
 *issues, otherwise NULL*/
Citizen citizenCreate (int citizenId, const char* citizenName,
        int yearsOfEducation, int age);

/** creates a new citizen identical to the one given to it
 *@param toCopy - a citizen to copy
 *@returns a citizen if toCopy is not NULL and there are no memory issues,
 * otherwise NULL*/
Citizen citizenCopy (Citizen toCopy);

/** destroys a citizen by freeing all of its memory
 * @param toDestroy - a citizen to destroy*/
void citizenDestroy (Citizen toDestroy);

/** compares two citizens by their ids
 * @param firstToComp - a citizen to compare
 * @param secondToComp - second citizen to compare
 * @returns true if two citizens have the same id, otherwise false */
bool citizenIsEqual (Citizen firstToComp, Citizen secondToComp);

/**compares two citizens by their name's lexicographical value
 * @param firstToComp - a citizen to compare
 * @param secondToComp - second citizen to compare
 * @returns true if the first citizen's name lexicographical value is
 * larger than the second, otherwise false*/
bool citizenIsGreater (Citizen firstToComp, Citizen secondToComp);

/** GET FUNCTIONS **/

/** given an int pointer, puts the citizen's id into it
 * @param toGet - a citizen to get the id from
 * @param idPtr - a pointer to put the id into
 * @returns CITIZEN_SUCCESS - if all went well and the pointer now has
 * the citizen's id
 * @returns CITIZEN_NULL_ARGUMENT - if either the pointer or the citizen
 * are NULL*/
CitizenResult getId (Citizen toGet, int* idPtr);

/** same as getId, only puts the age into the pointer */
CitizenResult getAge (Citizen toGet, int* agePtr);

/** same as getId, only puts the years of education into the pointer */
CitizenResult getEducation (Citizen toGet, int* educationPtr);

/** given a string pointer, puts the citizen's name into it
 * @param toGet - a citizen to get the name from
 * @param namePtr - a pointer to put the name into
 * @returns CITIZEN_SUCCESS - if all went well and the pointer now has
 * the citizen's name
 * @returns CITIZEN_NULL_ARGUMENT - if either the pointer or the citizen
 * are NULL
 * @returns CITIZEN_MEMORY_ERROR - if the memory allocation for the name
 * failed */
CitizenResult getName (Citizen toGet, char** namePtr);

/** given a bool pointer, puts the citizen's candidacy status into it
 * parameters and return value same as getId */
CitizenResult getCandidateStat (Citizen toGet, bool* candidateStatPtr);

/** given an int pointer and a candidate, puts the citizen's priority for
 * the candidate into the pointer
 * @param toGet - a citizen to get the priority from
 * @param prioritizedCandidate - a citizen who is a candidate to get the
 * priority for
 * @param priorityPtr - a pointer to put the priority into
 * @returns CITIZEN_SUCCESS - if  all went well and the pointer now has the
 * priority for the given candidate
 * @returns CITIZEN_NULL_ARGUMENT - if either the citizen, the candidate,
 * or the pointer are NULL
 * @returns CITIZEN_MEMORY_ERROR - if anything went wrong with the memory
 * allocations in the function
 * @returns CITIZEN_SUPPORT_DOESNT_EXIST - if the citizen doesn't support
 * the given candidate
 * @returns CITIZEN_IS_NOT_A_CANDIDATE - if prioritizedCandidate is not an
 * actual candidate*/
CitizenResult getAPriority (Citizen toGet, Citizen prioritizedCandidate,
        int* priorityPtr);

/** ELECTIONS FUNCTIONS **/

/** given a citizen, a candidate and a priority gives the citizen a preference
 * for that candidate with the given priority
 * @param addTo - a citizen to add the preference to
 * @param candidate - a citizen who is a candidate to support
 * @param priority - a non-negative int for the priority - the lower the int
 * the more support
 * @returns CITIZEN_SUCCESS - if all went well and the citizen now supports
 * the given candidate
 * @returns CITIZEN_NULL_ARGUMENT - if either the citizen or the candidate
 * are NULL
 * @returns CITIZEN_ILLEGAL_PRIORITY - if the priority is negative
 * @returns CITIZEN_IS_NOT_A_CANDIDATE - if the candidate is not an actual
 * candidate
 * @returns CITIZEN_CAN_NOT_SUPPORT - if the citizen to add the preference
 * to is a candidate itself
 * @returns CITIZEN_PRIORITY_EXISTS - if the citizen already has a preference
 * with the same priority
 * @returns CITIZEN_SUPPORT_EXISTS - if the citizen already supports the
 * candidate
 * @returns CITIZEN_MEMORY_ERROR - if something wrong with the memory
 * allocation for the preference*/
CitizenResult addPreference (Citizen addTo, Citizen candidate, int priority);

/** given a citizen and a candidate whom the citizen supports, clears that
 * support
 * @param clearTo - a citizen to clear the preference to
 * @param candidate - a citizen who is a candidate to clear the preference
 * from
 * @returns CITIZEN_SUCCESS - if the preference was cleared successfully
 * @returns CITIZEN_NULL_ARGUMENT - if the parameters are NULL
 * @returns CITIZEN_IS_NOT_A_CANDIDATE - if the candidate given isn't an
 * actual candidate
 * @returns CITIZEN_MUST_SUPPORT - if the citizen to clear the preference
 * from is the candidate, meaning we are trying to clear the support of
 * the candidate to itself*/
CitizenResult clearPreference (Citizen clearTo, Citizen candidate);

/** given a citizen, makes it a candidate
 * @param toMake - a citizen to turn into a candidate
 * @returns CITIZEN_SUCCESS - if the citizen is now a candidate
 * @returns CITIZEN_NULL_ARGUMENT - if the citizen is NULL
 * @returns CITIZEN_IS_ALREADY_A_CANDIDATE - self explanatory
 * @returns CITIZEN_AGE_NOT_APPROPRIATE - if the citizen is under 21 years old*/
CitizenResult makeCandidate (Citizen toMake);

/** given a candidate, clears its candidacy
 * @param toClear - a citizen who is a candidate
 * @returns CITIZEN_SUCCESS - if the candidacy was cleared
 * @returns CITIZEN_NULL_ARGUMENT - if toClear is NULL
 * @returns CITIZEN_IS_NOT_A_CANDIDATE - if toClear is not a candidate*/
CitizenResult clearCandidate (Citizen toClear);

#endif