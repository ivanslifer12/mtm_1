/*
 * exampleMain.c
 *
 *  Created on: Nov 10, 2018
 *      Author: ameeraam
 */


#include "mtm_elections.h"
#include <assert.h>

int main(){
	MtmElections mtmElections = mtmElectionsCreate();

	mtmElectionsAddCity(mtmElections,"Taub-Technion",1);

	mtmElectionsAddCitizen(mtmElections,"Khalid Manaa",1,35,16,1);
	mtmElectionsAddCitizen(mtmElections,"Samir Massad",2,45,16,1);
	mtmElectionsAddCitizen(mtmElections,"Sabri Fathi",3,24,16,1);
	mtmElectionsAddCitizen(mtmElections,"Ameer Aam",4,22,16,1);
	mtmElectionsAddCitizen(mtmElections,"Haitham Fadila",5,22,16,1);
	mtmElectionsAddCitizen(mtmElections,"Saleh Ali",6,22,16,1);
	mtmElectionsAddCitizen(mtmElections,"Atef Nassar",7,16,16,1);

	mtmElectionsAddCandidate(mtmElections,1,1);
	mtmElectionsAddCandidate(mtmElections,2,1);
	mtmElectionsAddCandidate(mtmElections,3,1);
	mtmElectionsAddCandidate(mtmElections,4,1);

	mtmElectionsSupportCandidate(mtmElections,5,1,1);
	mtmElectionsSupportCandidate(mtmElections,5,2,0);
	mtmElectionsSupportCandidate(mtmElections,5,4,2);

	mtmElectionsSupportCandidate(mtmElections,6,1,3);
	mtmElectionsSupportCandidate(mtmElections,6,2,4);
	mtmElectionsSupportCandidate(mtmElections,6,3,2);
	mtmElectionsSupportCandidate(mtmElections,6,4,5);

	mtmElectionsSupportCandidate(mtmElections,7,1,0);
	mtmElectionsSupportCandidate(mtmElections,7,2,9);
	mtmElectionsSupportCandidate(mtmElections,7,3,8);

	int mayor = -1;
	mtmElectionsMayorOfCity(mtmElections,1,&mayor,"example_out");
	assert(mayor == 3); // mayor is Sabri.

	mtmElectionsDestroy(mtmElections);
	return 0;
}
