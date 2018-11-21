/*
 * uniqueOrderedList.h
 *
 *  Created on: Oct 25, 2018
 *      Author: ameeraam
 */

#ifndef UNIQUEORDEREDLIST_H_
#define UNIQUEORDEREDLIST_H_
#include <stdbool.h>

/**
* Generic Unique Ordered List Container
*
* Implements a unique ordered list container type.
* which is a list with elements that are always sorted.
* The list contains no duplicates of equal elements.
* The list his an internal iterator for external use. For all functions
* where the state of the iterator after calling that function is not stated,
* it is undefined. That is you cannot assume anything about it.
*
* The following functions are available:
*
*   uniqueOrderedListCreate               - Creates a new empty list
*   uniqueOrderedListDestroy              - Deletes an existing list and frees all resources
*   uniqueOrderedListCopy                 - Copies an existing list
*   uniqueOrderedListSize                 - Returns the size of a given list
*   uniqueOrderedListContains             - Returns whether the list contains an element or not
*   uniqueOrderedListInsert               - Inserts an element in it's place according to the order
*                                           determined by the greater function.
*   uniqueOrderedListRemove               - Removes the element which is equal to the element passed
*                                           to the function according to the equals function.
*   uniqueOrderedListGetLowest            - Sets the internal iterator (also called current
*                                           element) to be the first element in the list and
*                                           return it, this should be the lowest element.
*   uniqueOrderedListGetGreatest          - Sets the internal iterator (also called current
*                                           element) to be the last element in the list and
*                                           return it, this should be the greatest element.
*   uniqueOrderedListGetNext              - Advances the list's iterator to the next element
*                                           and return it
*   uniqueOrderedListClear		      	  - Clears all the data from the list
*/

typedef struct uniqueOrderedList_t* UniqueOrderedList;

/** Result type which is used for returning error values
 * from unique ordered list functions */
typedef enum UniqueOrderedListResult_t {
	UNIQUE_ORDERED_LIST_SUCCESS,
	UNIQUE_ORDERED_LIST_OUT_OF_MEMORY,
	UNIQUE_ORDERED_LIST_NULL_ARGUMENT,
	UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST,
	UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS
} UniqueOrderedListResult;

/* Data type for the elements of the UniqueOrderedList container*/
typedef void* Element;

/** Type of function pointer which copies a data element of the UniqueOrderedList */
typedef Element (*copyElements)(Element);

/** Type of function pointer which releases a data element of the UniqueOrderedList */
typedef void (*freeElements)(Element);

/** Type of function pointer which checks if two elements of the UniqueOrderedList
 * are equal.
 * the function should return true when two elements are equal and false otherwise. */
typedef bool (*elementsEquals)(Element, Element);

/** Type of function pointer which checks if e1 element is greater than e2 element.
 * the function should return true when e1 is greater and false otherwise. */
typedef bool (*elementGreaterThan)(Element e1, Element e2);

/**
* Allocates a new List.
*
* Creates a new empty list. This function receives the functions which will be
* used for copying elements into the list, freeing them when needed,
* comparing if elements are greater from each other and comparing it elements
* are equal.
* ** note that the internal iterator is not valid after calling this function.
* return values:
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new List in case of success.
*/
UniqueOrderedList uniqueOrderedListCreate(copyElements, freeElements, elementsEquals, elementGreaterThan);

/**
* Deallocates an existing list. Clears all elements by using the
* stored free function.
*/
void uniqueOrderedListDestroy(UniqueOrderedList);

/**
* Creates a copy of target list.
*
* The new copy will contain all the elements from the source list in the same
* order and will use the same functions as the original list for copying,
* freeing and comparing elements.
* ** note that the internal iterator of the new copy
*     is not valid after calling this function.
* return values:
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A List containing the same elements with same order.
*/
UniqueOrderedList uniqueOrderedListCopy(UniqueOrderedList);

/**
* Returns the number of elements in a list.
* return values:
* 	-1 if the list passed is NULL.
* 	size of the list otherwise.
*/
int uniqueOrderedListSize(UniqueOrderedList);

/**
 * Checks if an element exists in the list. The element will be
 * considered in the list if one of the elements in the list it determined equal
 * using the comparison function used to initialize the list.
 * return values:
 * 	true - if there exists an element which is equal to the element passed to the function.
 * 	false - otherwise.
 */
bool uniqueOrderedListContains(UniqueOrderedList, Element);

/**
 * Inserts an element into the list if there is no element equals to it in the list.
 * the element is inserted in it's place according to the order determined by the
 * greater comparison function used to initialize the list.
 * return values:
 * UNIQUE_ORDERED_LIST_NULL_ARGUMENT - one of the arguments passed is NULL
 * UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS - there exists an element which is equal
 * 											 to the new element
 * UNIQUE_ORDERED_LIST_SUCCESS - in case of successful insertion.
 */
UniqueOrderedListResult uniqueOrderedListInsert(UniqueOrderedList, Element);

/**
 * removes an element from the list if there is an element equals to it in the list.
 * the element removed is the one equals to the passed element according to the equals
 * function passed when initializing the list.
 * ** note that the internal iterator is not valid after calling this function.
 * return values:
 * UNIQUE_ORDERED_LIST_NULL_ARGUMENT - one of the arguments passed is NULL
 * UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST - there is no element which is equal
 * 											 to the passed element
 * UNIQUE_ORDERED_LIST_SUCCESS - in case of successful removal.
 */
UniqueOrderedListResult uniqueOrderedListRemove(UniqueOrderedList, Element);

/**
* Sets the internal iterator to the first (lowest) element and retrieves it.
*
* The list has an internal iterator (also called current element) to allow
* iteration over the list's elements. This function sets the iterator to point
* to the first (lowest) element in the list and return it.
* Use this to start iteraing over the list, searching from the lowest element of
* the list and/or get the first (lowest) element in the list.
* (To continue iteration use uniqueOrderedListGetNext)
* example:
* void f(UniqueOrderedList list) {
*   Element first = uniqueOrderedListGetLowest(list);
*   printf("The lowest element is at address %x\n", first);
* }
*
* return values:
* NULL is a NULL pointer was sent or the list is empty.
* The lowest element of the list otherwise
 */
Element uniqueOrderedListGetLowest(UniqueOrderedList);

/**
* Sets the internal iterator to the last (greatest) element and retrieves it.
* the next element of the element returned should be NULL.
* example:
* void f(UniqueOrderedList list) {
*   Element last = uniqueOrderedListGetGreatest(list);
*   printf("The greatest element is at address %x\n", last);
* }
*
* return values:
* NULL is a NULL pointer was sent or the list is empty.
* The greatest element of the list otherwise
 */
Element uniqueOrderedListGetGreatest(UniqueOrderedList);

/**
* Advances the list's iterator to the next element and return it
*
* return values
* NULL if reached the end of the list, the iterator is at an invalid state or
* a NULL sent as argument
* The next element on the list in case of success
*/
Element uniqueOrderedListGetNext(UniqueOrderedList);

/**
* Removes all elements from target list.
*/
void uniqueOrderedListClear(UniqueOrderedList);

#endif /* UNIQUEORDEREDLIST_H_ */
