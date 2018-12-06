cmake_minimum_required(VERSION 3.10)
project(mtm_1 C)

set(CMAKE_C_STANDARD 99)

add_executable( citizen.h citizen.c preference.c preference.h preferenceDebug.c citizenDebug.c)