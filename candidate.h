//
// Created by Bar The magical on 12/12/2018.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef MTM_1_CANDIDATE_H
#define MTM_1_CANDIDATE_H

/** struct for holding the priority and appropriate candidate for the city 
 * and mtm_elections struct */
typedef struct candidate_t* Candidate;

/** possible return values */
typedef enum CandidateResult_t{
    CANDIDATE_NULL_ARGUMENT,
    CANDIDATE_SUCCESS,
    CANDIDATE_MEMORY_ERROR
}CandidateResult;

/** creates a new candidate
 * @param candidateId- the candidate's id
 * @param the name of that candidate
 * @returns a candidate if successful, if not - NULL
 * if all parameters are ok NULL means memory error*/
Candidate candidateCreate (int candidateId, char* name);

/** copies a candidate
 * @param toCopy - a candidate to copy
 * @returns an identical candidate if successful, if not - NULL
 * if all parameters are ok NULL means memory error*/
Candidate candidateCopy (Candidate toCopy);

/** destroys a candidate by freeing all of its memory usage
 * @param toDestroy - candidate to destroy*/
void candidateDestroy (Candidate toDestroy);


/** given a pointer, returns the candidate id of the candidate in it
 * @param toGet - the candidate from which we want to get the id
 * @param idPtr - the pointer to hold the id
 * @returns CANDIDATE_NULL_ARGUMENT - if toGet or idPtr are NULL
 * @returns CANDIDATE_SUCCESS - if all went well */
CandidateResult candidateGetCandidateId (Candidate toGet, int* idPtr);

/** identical to candidateGetCandidateId, only with name
 * @returns CANDIDATE_MEMORY_ERROR - if the memory allocation for the name failed*/
CandidateResult candidateGetName (Candidate toGet, char** namePtr);

/** compare function to be used by the uol struct.
 * @param two candidates, order matters!
 * @returns true if the first candidate has a larger priority, otherwise false*/
bool candidateIsGreater (Candidate firstToComp, Candidate secondToComp);


/** if  the candidate id is equal in both candidates
 * this returns true */
bool candidateIsEqual (Candidate firstToComp, Candidate secondToComp);


#endif //MTM_1_CANDIDATE_H
