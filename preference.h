//
// Created by Bar The magical on 30/11/2018.
//

#ifndef MTM_1_PREFERENCE_H
#define MTM_1_PREFERENCE_H

typedef struct preference_t* Preference;

typedef enum PreferenceResult_t{
    PREFERENCE_MEMORY_ERROR,
    PREFERENCE_NULL_ARGUMENT,
    PREFERENCE_ILLEGAL_ID,
    PREFERECNE_ILLEGAL_PRIORITY,
    PREFERENCE_SUCCESS
}PreferenceResult;

PreferenceResult preferenceCreate (const char* candidateName, int candidateId, int priority);

PreferenceResult preferrenceCopy (Preference toCopy);

PreferenceResult preferenceDestroy (Preference toDestroy);

PreferenceResult preferenceGetCandidateName (Preference toGet);

PreferenceResult preferenceGetCandidateId (Preference toGet);

PreferenceResult preferenceGetPriority (Preference toGet);

int preferenceComparePriority (Preference firstToComp, Preference secondToComp);

int preferenceCompareId (Preference firstToComp, Preference secondToComp);


#endif //MTM_1_PREFERENCE_H
