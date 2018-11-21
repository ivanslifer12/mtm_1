/*
 * mtm_print.h
 *
 *  Created on: Nov 6, 2018
 *      Author: ameeraam
 */

#ifndef MTM_PRINT_H_
#define MTM_PRINT_H_
#include <stdio.h>

void mtmPrintMayorDetails(int id, int age, int yearsOfEducation, char* name,
		char* city, int cityId, FILE* file);
void mtmPrintFileError(const char* filename, FILE* file);

#endif /* MTM_PRINT_H_ */
