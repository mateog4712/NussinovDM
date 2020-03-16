project(NussinovDM)

cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(GCC_COVERAGE_COMPILE_FLAGS "-o0 -w -fopenmp")
#SET(GCC_COVERAGE_LINK_FLAGS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")


add_executable(main
baseline.cpp bottomUp.cpp optimized.cpp main.cpp 
recursion.cpp 
removedBranching.cpp simd.cpp testCases.cpp transp2mtr.cpp variation.cpp
)