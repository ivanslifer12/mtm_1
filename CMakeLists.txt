cmake_minimum_required(VERSION 3.10)
project(mtm_1 C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(mtm_1  mtm_elections.h mtmElections.c city.c city.h candidate.h candidate.c citizen.h citizen.c preference.h preference.c uniqueOrderedList/uniqueorderedlist.c uniqueOrderedList/uniqueorderedlist.h )

set(CMAKE_C_FLAGS "-std=c99 -Wall -Werror -pedantic-errors")