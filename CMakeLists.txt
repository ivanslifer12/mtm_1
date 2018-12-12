cmake_minimum_required(VERSION 3.10)
project(mtm_1 C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(mtm_1  city.h city.c candidate.c candidate.h citizen.h citizen.c citizenDebug.c preference.h preference.c uniqueOrderedList/uniqueorderedlist.c uniqueOrderedList/uniqueorderedlist.h candidate.c candidate.h)
target_link_libraries(mtm_1 ${CMAKE_SOURCE_DIR}/libmtm.a)
set(CMAKE_C_FLAGS "-std=c99 -Wall")