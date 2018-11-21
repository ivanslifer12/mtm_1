//
// Created by lambda7 on 11/21/18.
//

#include "uniqueOrderedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct uniqueOrderedList_t
{
    Element data;
    UniqueOrderedList next;
};

