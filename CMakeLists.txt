cmake_minimum_required(VERSION 3.12)
project(mtm_1 C)

set(CMAKE_C_STANDARD 99)

add_executable(mtm_1 main.c uniqueOrderedList/uniqueOrderedList.c mtmElections.c city.c city.h citizen.c citizen.h)