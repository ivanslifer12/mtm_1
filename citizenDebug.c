//
// Created by Bar The magical on 05/12/2018.
//

#include <assert.h>
#include "citizen.h"

int main() {

    Citizen Moshe = citizenCreate(0, "Moshe", 12, 25);
    Citizen Shimon = citizenCreate(1, "Shimon", 12, 19);
    assert(Moshe);
    assert(Shimon);


    Citizen MosheClone = citizenCopy(Moshe);
    assert(MosheClone);
    assert(citizenIsEqual(MosheClone, Moshe));

    citizenDestroy(MosheClone);

    int idGet;
    CitizenResult getChk = getId(Moshe, &idGet);
    assert(getChk == CITIZEN_SUCCESS);
    assert(idGet == 0);

    char* nameGet;
    getChk = getName(Moshe, &nameGet);
    assert(getChk == CITIZEN_SUCCESS);
    assert(strcmp(nameGet, "Moshe") == 0);
    free(nameGet);

    CitizenResult candidancy = makeCandidate(Moshe);
    assert(candidancy == CITIZEN_SUCCESS);

    bool statGet;
    getChk = getCandidateStat(Moshe, &statGet);
    assert(getChk == CITIZEN_SUCCESS);
    assert(statGet);

    int priority;
    getChk = getAPriority(Moshe,Moshe, &priority);
    assert(getChk == CITIZEN_SUCCESS);
    assert(priority == 0);

    CitizenResult preferenceAdd = addPreference(Shimon, Moshe,2);
    assert(preferenceAdd == CITIZEN_SUCCESS);
    int shimonPriority;
    assert(getAPriority(Shimon, Moshe, &shimonPriority) == CITIZEN_SUCCESS);
    assert(shimonPriority == 2);

    CitizenResult preferenceClear = clearPreference(Shimon, Moshe);
    assert(preferenceClear == CITIZEN_SUCCESS);
    int noPriority;
    assert(getAPriority(Shimon, Moshe, &noPriority) == CITIZEN_SUPPORT_DOESNT_EXIST);

    CitizenResult candidateClear = clearCandidate(Moshe);
    assert(candidateClear == CITIZEN_SUCCESS);
    bool clearCand;
    getCandidateStat(Moshe, &clearCand);
    assert(!clearCand);

    citizenDestroy(Moshe);
    citizenDestroy(Shimon);
    printf("ALL WORKS");
    return 0;
}
