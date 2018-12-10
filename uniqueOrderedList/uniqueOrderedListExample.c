/*
 * uniqueOrderedListExample.c
 *
 *  Created on: Nov 17, 2018
 *      Author: ameeraam
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "uniqueOrderedList.h"
//#include "uniqueOrderedList.o"
#define ARRAY_LENGTH(array) sizeof(array)/sizeof(*array)


Element copyInt(Element e){
	int* newInt = malloc(sizeof(*newInt));
	if(!newInt){
		return NULL;
	}
	*newInt = *((int*)e);
	return newInt;
}

void freeInt(Element e){
	free(e);
}

bool intEquals(Element e1, Element e2){
	return *((int*)e1) == *((int*)e2);
}

bool intGreaterThan(Element e1, Element e2){
	return *((int*)e1) > *((int*)e2);
}

int uniqueOrderedListUsage(){
	int arr[] = {3,2,1,-4,1,2};
	UniqueOrderedList uol = uniqueOrderedListCreate(copyInt, freeInt, intEquals, intGreaterThan);
	if(!uol){
		printf("Allocation Error!");
		return 1;
	}

	for(int i = 0; i < ARRAY_LENGTH(arr) ; i++){
		uniqueOrderedListInsert(uol, arr + i);
	}

	assert(*((int*)uniqueOrderedListGetLowest(uol)) == -4);

	int sortedArr[] = {-4,1,2,3};
	for(int i = 0, *number = uniqueOrderedListGetLowest(uol);
			i < ARRAY_LENGTH(sortedArr);
			i++, number = uniqueOrderedListGetNext(uol)){
		assert(number);
		assert(*number == sortedArr[i]);
	}
	assert(uniqueOrderedListGetNext(uol) == NULL);

	int toRemove = 3;
	UniqueOrderedList uol2 = uniqueOrderedListCopy(uol);
	uniqueOrderedListRemove(uol, &toRemove);
	assert(!uniqueOrderedListContains(uol, &toRemove));
	assert(uniqueOrderedListContains(uol2, &toRemove));
        

        int sortedArr2[] = {-4,1,2};
        for(int i = 0, *number = uniqueOrderedListGetLowest(uol);
                        i < ARRAY_LENGTH(sortedArr2);
                        i++, number = uniqueOrderedListGetNext(uol)){
                assert(number);
                assert(*number == sortedArr2[i]);
        }
        assert(uniqueOrderedListGetNext(uol) == NULL);
        printf("removing %d\n", -4);


        toRemove = -4;
        uniqueOrderedListRemove(uol, &toRemove);
        
        int sortedArr3[] = {1,2};
        for(int i = 0, *number = uniqueOrderedListGetLowest(uol);
                        i < ARRAY_LENGTH(sortedArr3);
                        i++, number = uniqueOrderedListGetNext(uol)){
                assert(number);
                assert(*number == sortedArr3[i]);
        }
        assert(uniqueOrderedListGetNext(uol) == NULL);
        printf("removing %d\n", 2);

        toRemove = 2;
        uniqueOrderedListRemove(uol, &toRemove);

        int sortedArr4[] = {1};
        for(int i = 0, *number = uniqueOrderedListGetLowest(uol);
                        i < ARRAY_LENGTH(sortedArr4);
                        i++, number = uniqueOrderedListGetNext(uol)){
                assert(number);
                assert(*number == sortedArr4[i]);
        }
        assert(uniqueOrderedListGetNext(uol) == NULL);
        printf("removing %dn\n", 1);
        	
        toRemove = 1;
        uniqueOrderedListRemove(uol, &toRemove);

        assert(uniqueOrderedListGetLowest(uol) == NULL);
        printf("removing done\n");


	uniqueOrderedListDestroy(uol);
	uniqueOrderedListDestroy(uol2);
	printf("SUCCESS\n");
	return 0;
}

int main(){
	uniqueOrderedListUsage();
	return 0;
}

