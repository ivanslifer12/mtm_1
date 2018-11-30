//
// Created by Bar The magical on 28/11/2018.
//

#include "citizen.h"
#include "list.h"


struct citizen_t{
    const char* citizenName;
    int age;
    int citizenId;
    int yearsOfEducation;
    bool isCandidate;
    List prefrences; /** list of a struct called "prefrence" */

};
