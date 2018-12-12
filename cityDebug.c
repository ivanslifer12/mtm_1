//
// Created by Bar The magical on 12/12/2018.
//
#include "assert.h"
#include "city.h"

int main(){
    City newYork = cityCreate(111, "New York");
    assert(newYork);

    City newYorkCopy = cityCopy(newYork);
    assert(newYorkCopy);

    assert(cityIsEqual(newYork, newYorkCopy));

    cityDestroy(newYorkCopy);

    CityResult addResult = cityAddCitizen(newYork, 0, "Moshe", 12, 25);
    assert(addResult == CITY_SUCCESS);
    cityAddCitizen(newYork, 1, "Shimon", 12, 19);

    CityResult addResult2 = cityAddCitizen(newYork, 0, "Something", 12, 25);
    assert(addResult2 == CITY_CITIZEN_ALREADY_EXISTS);

    CityResult candidateResult = cityMakeCandidate(newYork, 0);
    assert(candidateResult == CITY_SUCCESS);

    CityResult candidateResult2 = cityMakeCandidate(newYork, 0);
    assert(candidateResult2 == CITY_CITIZEN_ALREADY_CANDIDATE);

    CityResult candidateResult3 = cityMakeCandidate(newYork, 1);
    assert(candidateResult3 == CITY_AGE_NOT_APPROPRIATE);

    CityResult preferenceResult1 = addSupport(newYork, 1, 0, 0);
    assert(preferenceResult1 == CITY_SUCCESS);

    CityResult preferenceResult2 = addSupport(newYork, 1, 0, 0);
    assert(preferenceResult2 == CITY_SUPPORT_EXISTS);

    int candidateId;
    CityResult getResult = getACitizenHighestSupport(newYork, 0, &candidateId);
    assert(getResult == CITY_SUCCESS);
    assert(candidateId == 0);

    CityResult removeResult = cityRemoveCitizen(newYork, 0);
    assert(removeResult == CITY_SUCCESS);

    cityDestroy(newYork);
    printf("Done");

    return 0;
}