/*
 * mtm_elections.h
 *
 *  Created on: Oct 24, 2018
 *      Author: ameeraam
 */

#ifndef MTM_ELECTIONS_H_
#define MTM_ELECTIONS_H_

#include "uniqueOrderedList/uniqueOrderedList.h"

typedef enum mtmElectionsResult_t{
	MTM_ELECTIONS_MEMORY_ERROR,
	MTM_ELECTIONS_NULL_ARGUMENT,
	MTM_ELECTIONS_ILLEGAL_ID,
	MTM_ELECTIONS_ILLEGAL_AGE,
	MTM_ELECTIONS_ILLEGAL_PRIORITY,
	MTM_ELECTIONS_ILLEGAL_NUMBER_OF_YEARS,
	MTM_ELECTIONS_CITIZEN_ALREADY_EXISTS,
	MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST,
	MTM_ELECTIONS_CANDIDATE_ALREADY_EXISTS,
	MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST,
	MTM_ELECTIONS_CITY_ALREADY_EXISTS,
	MTM_ELECTIONS_CITY_DOES_NOT_EXIST,
	MTM_ELECTIONS_NOT_SAME_CITY,
	MTM_ELECTIONS_ALREADY_SUPPORTED,
	MTM_ELECTIONS_NOT_SUPPORTED,
	MTM_ELECTIONS_CAN_NOT_SUPPORT,
	MTM_ELECTIONS_MUST_SUPPORT,
	MTM_ELECTIONS_AGE_NOT_APPROPRIATE,
	MTM_ELECTIONS_PRIORITY_EXISTS,
	MTM_ELECTIONS_NO_CANDIDATES_IN_CITY,
	MTM_ELECTIONS_FILE_ERROR,
	MTM_ELECTIONS_SUCCESS
}MtmElectionsResult;

typedef struct mtm_elections_t* MtmElections;

typedef int (*RankFunc)(MtmElections, int, void*);

MtmElections mtmElectionsCreate();

void mtmElectionsDestroy(MtmElections mtmElections);

MtmElectionsResult mtmElectionsAddCity(MtmElections mtmElections, const char* cityName, int cityId);

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




#endif /* MTM_ELECTIONS_H_ */
