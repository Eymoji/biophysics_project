cmake_minimum_required(VERSION 3.26)
project(biophysics_project)

# Add TBB include and link directories
#include_directories("C:/Program Files (x86)/Intel/oneAPI/tbb/latest/include")
#link_directories("C:/Program Files (x86)/Intel/oneAPI/tbb/latest/lib")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS -ffast-math)

add_executable(biophysics_project main.cpp
        chemo.cpp
        chemo.h
        fonctions.cpp
        fonctions.h
        receptor.cpp
        receptor.h)

# Link with the TBB library
#target_link_libraries(biophysics_project tbb)
