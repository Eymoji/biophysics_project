cmake_minimum_required(VERSION 3.26)
project(biophysics_project)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

#message(${CMAKE_CURRENT_LIST_DIR})

add_executable(biophysics_project main.cpp
        chemo.cpp
        chemo.h
        fonctions.cpp
        fonctions.h
        receptor.cpp
        receptor.h)