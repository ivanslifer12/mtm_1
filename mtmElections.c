//
// Created by Bar The magical on 28/11/2018.
//

#include <assert.h>
#include "mtm_elections.h"
#include "city.h"
#include "candidate.h"

struct mtm_elections_t{
    UniqueOrderedList cities;
};

MtmElections mtmElectionsCreate(){
    MtmElections createdElections = malloc(sizeof(*createdElections));
    if(!createdElections){
        return NULL;
    }

    createdElections->cities =
            uniqueOrderedListCreate((void*(*)(void*))cityCopy,
                    (void(*)(void*))cityDestroy,
                    (bool(*)(void*, void*))cityIsEqual,
                    (bool(*)(void*, void*))cityIsGreater);

    return createdElections;
}

void mtmElectionsDestroy(MtmElections mtmElections){
    if(!mtmElections){
        return;
    }

    uniqueOrderedListDestroy(mtmElections->cities);
    free(mtmElections);
}

MtmElectionsResult mtmElectionsAddCity(MtmElections mtmElections, const char* cityName, int cityId){
    if(!mtmElections || !cityName){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }

    if(cityId < 0){
        return MTM_ELECTIONS_ILLEGAL_ID;
    }

    City cityToAdd = cityCreate(cityId, cityName);
    if(!cityToAdd){
        return MTM_ELECTIONS_MEMORY_ERROR;
    }

    UniqueOrderedListResult insertResult = uniqueOrderedListInsert(mtmElections->cities, cityToAdd);
    cityDestroy(cityToAdd);

    if(insertResult == UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS){
        return MTM_ELECTIONS_CITY_ALREADY_EXISTS;
    }

    if(insertResult == UNIQUE_ORDERED_LIST_OUT_OF_MEMORY){
        return MTM_ELECTIONS_MEMORY_ERROR;
    }

    assert(insertResult == UNIQUE_ORDERED_LIST_SUCCESS);
    return MTM_ELECTIONS_SUCCESS;
}

MtmElectionsResult mtmElectionsAddCitizen(MtmElections mtmElections, const char* citizenName,
                                          int citizenId, int citizenAge, int yearsOfEducation, int cityId);

MtmElectionsResult MtmElectionsCitizenGetName(MtmElections mtmElections, int citizenId, char** name);

MtmElectionsResult MtmElectionsCitizenGetCity(MtmElections mtmElections, int citizenId, int* cityId);

MtmElectionsResult MtmElectionsCitizenGetAge(MtmElections mtmElections, int citizenId, int* age);

MtmElectionsResult MtmElectionsCitizenGetEducation(MtmElections mtmElections, int citizenId, int* yearsOfEducation);

MtmElectionsResult mtmElectionsAddCandidate(MtmElections mtmElections, int candidateId, int cityId);

MtmElectionsResult mtmElectionsWithdrawCandidate(MtmElections mtmElections, int candidateId, int cityId);

MtmElectionsResult mtmElectionsSupportCandidate(MtmElections mtmElections, int citizenId, int candidateId, int priority);

MtmElectionsResult mtmElectionsCancelSupport(MtmElections mtmElections, int citizenId, int candidateId);

MtmElectionsResult mtmElectionsChangeAddress(MtmElections mtmElections, int citizenId, int cityId);

int mtmElectionsRankByAge(MtmElections mtmElections, int citizen, void* pAge);

UniqueOrderedList mtmElectionsPerformElections(MtmElections mtmElections, RankFunc rank, void* auxilaryData, const char* filename);

MtmElectionsResult mtmElectionsMayorOfCity(MtmElections mtmElections, int cityId, int* mayor, const char* filename);



