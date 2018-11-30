//
// Created by Bar The magical on 30/11/2018.
//

#ifndef MTM_1_PREFERENCE_H
#define MTM_1_PREFERENCE_H

typedef struct preference_t* Preference;

typedef enum CitizenResult_t{
    PREFERENCE_MEMORY_ERROR,
    PREFERENCE_NULL_ARGUMENT,
    PREFERENCE_NOT_A_CANDIDATE,
    PREFERECNE_ILLEGAL_PRIORITY,
    PREFERENCE_SUCCESS
}PreferenceResult;

#endif //MTM_1_PREFERENCE_H
