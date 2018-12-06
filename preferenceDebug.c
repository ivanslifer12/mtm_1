//
// Created by Bar The magical on 04/12/2018.
//

#include "preference.h"
#include <stdio.h>
#include <stdlib.h>


int main() {
    Preference Moshe = preferenceCreate("MOSHE", 1000, 1);
    if(!Moshe){
        printf("Problem with preferenceCreate");
        return 0;
    }

    int* id = malloc(sizeof(int));
    PreferenceResult getId = preferenceGetCandidateId(Moshe, id);
    if(getId!=PREFERENCE_SUCCESS){
        printf("problem in get id 1");
        return 0;
    }
    if(*id != 1000){
        printf("problem in get id 2");
        return 0;
    }
    free(id);

    char** name = malloc(sizeof(*name));
    PreferenceResult getName = preferenceGetCandidateName(Moshe, name);
    if(getName!=PREFERENCE_SUCCESS){
        printf("problem in get name 1");
        return 0;
    }
    if (strncmp(*name, "MOSHE", 5)!=0){
        printf("problem in get name 2");
        return 0;
    }
    free(*name);
    free(name);

    Preference Moshe2 = preferenceCopy(Moshe);
    if(!Moshe2){
        printf("Problem with preferenceCopy");
        return 0;
    }

    if(preferenceComparePriority(Moshe, Moshe2)){
        printf("problem with comparing priorities");
        return 0;
    }

    if(preferenceCompareId(Moshe, Moshe2)){
    printf("problem with comparing ids");
    return 0;
    }


    preferenceDestroy(Moshe2);
    preferenceDestroy(Moshe);

    printf("All works!");
    return 0;
}

