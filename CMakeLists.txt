cmake_minimum_required(VERSION 3.26)
project(biophysics_project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS -ffast-math)

add_executable(biophysics_project main.cpp
        chemo.cpp
        chemo.h
        fonctions.cpp
        fonctions.h
        receptor.cpp
        receptor.h)
