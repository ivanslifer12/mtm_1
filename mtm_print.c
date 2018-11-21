/*
 * mtm_print.c
 *
 *  Created on: Nov 6, 2018
 *      Author: ameeraam
 */

#include "mtm_print.h"
void mtmPrintMayorDetails(int mayorId, int age, int yearsOfEducation, char* mayorName,
		char* cityName, int cityId, FILE* file){
	fprintf(file,"****************************************\n");
	fprintf(file,"The Winner of the Elections at: %s %d is:\n", cityName, cityId);
	fprintf(file,"Mayor: \n");
	fprintf(file,"	Name: %s\n", mayorName);
	fprintf(file,"	ID:   %d\n", mayorId);
	fprintf(file,"	Age:  %d\n", age);
	fprintf(file,"	Years of education:  %d\n", yearsOfEducation);
	fprintf(file,"****************************************\n");
}

void mtmPrintFileError(const char* filename, FILE* file){
	fprintf(file,"Error: cannot open file: %s\n",filename);
}
