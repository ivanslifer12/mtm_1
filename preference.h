//
// Created by Bar The magical on 30/11/2018.
//

#ifndef MTM_1_PREFERENCE_H
#define MTM_1_PREFERENCE_H

typedef struct preference_t* Preference;

typedef enum PreferenceResult_t{
    PREFERENCE_NULL_ARGUMENT,
    PREFERENCE_NOT_A_CANDIDATE,
    PREFERENCE_ILLEGAL_PRIORITY,
    PREFERENCE_MEMORY_ERROR,
    PREFERENCE_SUCCESS
}PreferenceResult;

Preference preferenceCreate (char* candidateName, int candidateId, int priority);
Preference preferenceCopy (Preference toCopy);
void preferenceDestroy (Preference toDestroy);
PreferenceResult preferenceGetCandidateName (Preference toGet, char** namePtr);
PreferenceResult preferenceGetCandidateId (Preference toGet, int* idPtr);
PreferenceResult preferenceGetPriority (Preference toGet, int* priorityPtr);
int preferenceComparePriority (Preference firstToComp, Preference secondToComp);
int preferenceCompareId (Preference firstToComp, Preference secondToComp);


#endif //MTM_1_PREFERENCE_H
