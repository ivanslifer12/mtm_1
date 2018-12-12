//
// Created by Bar The magical on 30/11/2018.
//

#include "candidate.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

struct candidate_t{
    int candidateId;
    char* name;
};

Candidate candidateCreate (int candidateId, char* name){

    if(candidateId < 0 || name == NULL) {
        return NULL;
    }

    Candidate createdCandidate = malloc(sizeof(*createdCandidate));
    if (!createdCandidate){
        return NULL;
    }

    createdCandidate -> candidateId = candidateId;
    createdCandidate -> name = malloc(sizeof(strlen(name)+1));
    if(!createdCandidate -> name){
        free(createdCandidate);
        return NULL;
    }

    strcpy(createdCandidate->name, name);

    return createdCandidate;

}

Candidate candidateCopy (Candidate toCopy){

    if(!toCopy){
        return NULL;
    } //this should be in the calling function

    Candidate createdCandidate = malloc(sizeof((*toCopy)));
    if(!createdCandidate){
        return NULL;
    }

    if(candidateGetCandidateId(toCopy, &(createdCandidate->candidateId)) != CANDIDATE_SUCCESS||
       candidateGetName(toCopy, &(createdCandidate->name))!=CANDIDATE_SUCCESS){
        free(createdCandidate);
        return NULL;
    }

    return createdCandidate;

}

void candidateDestroy (Candidate toDestroy){

    if(!toDestroy){
        return;
    }

    free(toDestroy->name);
    free(toDestroy);
}


CandidateResult candidateGetCandidateId (Candidate toGet, int* idPtr){
    if(!toGet || !idPtr) {
        return CANDIDATE_NULL_ARGUMENT;
    }

    *idPtr = toGet->candidateId;

    return CANDIDATE_SUCCESS;
}

CandidateResult candidateGetName (Candidate toGet, char** namePtr){
    if(!toGet || !namePtr)
        return CANDIDATE_NULL_ARGUMENT;

    *namePtr = malloc(sizeof(strlen(toGet->name)+1));
    if(!*namePtr){
        return CANDIDATE_MEMORY_ERROR;
    }

    strcpy(*namePtr, toGet->name);
    return CANDIDATE_SUCCESS;
}

bool candidateIsGreater (Candidate firstToComp, Candidate secondToComp){
    return (strcmp(firstToComp->name, secondToComp->name)>0||
            (strcmp((firstToComp)->name, secondToComp->name)==0 &&
            firstToComp->candidateId < secondToComp->candidateId));
}

bool candidateIsEqual (Candidate firstToComp, Candidate secondToComp){
    return (firstToComp->candidateId == secondToComp->candidateId);
}


