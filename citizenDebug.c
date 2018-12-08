//
// Created by Bar The magical on 05/12/2018.
//

#include "citizen.h"
#include "list.h"

int main() {

    Citizen Moshe = citizenCreate(0, "Moshe", 12, 25);

    Citizen Shimon = citizenCreate(1, "Shimon", 12, 19);

    if(!Moshe || !Shimon){
        printf("citizen create returned NULL");
        return 0;
    }


    Citizen MosheClone = citizenCopy(Moshe);

    if(!MosheClone){
        citizenDestroy(Moshe);
        citizenDestroy(Shimon);
        printf("citizen copy returned NULL");
        return 0;
    }

    citizenDestroy(MosheClone);

    int* idGet = NULL;
    CitizenResult getChk = getId(Moshe, idGet);
    if( getChk!=CITIZEN_SUCCESS){
        citizenDestroy(Moshe);
        citizenDestroy(Shimon);
        free(idGet);
        printf("problem with getID");
        return 0;
    }
    free(idGet);

    char** nameGet = NULL;
    getChk = getName(Moshe, nameGet);
    if( getChk!=CITIZEN_SUCCESS){
        citizenDestroy(Moshe);
        citizenDestroy(Shimon);
        free(nameGet);
        printf("problem with getName");
        return 0;
    }
    free(nameGet);

    CitizenResult candidancy = makeCandidate(Moshe);
    if(candidancy!=CITIZEN_SUCCESS){
        citizenDestroy(Moshe);
        citizenDestroy(Shimon);
        printf("problem with makeCandidate");
        return 0;
    }

    bool* statGet = NULL;
    getChk = getCandidateStat(Moshe, statGet);
    if(getChk!=CITIZEN_SUCCESS){
        citizenDestroy(Moshe);
        citizenDestroy(Shimon);
        free(statGet);
        printf("problem with candidate status");
        return 0;
    }
    free(statGet);

    int* priority = NULL;
    getChk = getAPriority(Moshe,Moshe, priority);
    if(getChk!=CITIZEN_SUCCESS){
        citizenDestroy(Moshe);
        citizenDestroy(Shimon);
        free(priority);
        printf("problem with priority get - for candidate");
        return 0;
    }
    free(priority);

    CitizenResult preferenceAdd = addPreference(Shimon, Moshe,2);
    if(preferenceAdd!=CITIZEN_SUCCESS){
        citizenDestroy(Moshe);
        citizenDestroy(Shimon);
        printf("problem with adding a preference");
        return 0;
    }

    CitizenResult preferenceClear = clearPreference(Shimon, Moshe);
    if(preferenceClear!=CITIZEN_SUCCESS){
        citizenDestroy(Moshe);
        citizenDestroy(Shimon);
        printf("problem with clearing a preference");
        return 0;
    }

    CitizenResult candidateClear = clearCandidate(Moshe);
    if(candidateClear !=CITIZEN_SUCCESS){
        citizenDestroy(Moshe);
        citizenDestroy(Shimon);
        printf("problem with clearing a candidate");
        return 0;
    }

    citizenDestroy(Moshe);
    citizenDestroy(Shimon);
    printf("ALL WORKS");
    return 0;
}
