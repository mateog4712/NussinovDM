#!/usr/bin/env bash

#g++ -o0 -std=c++17 baseline.cpp bottomUp.cpp main.cpp optimized.cpp parallel.cpp  removedBranching.cpp transp2mtr.cpp variation.cpp recursion.cpp recursionParallel.cpp simd.cpp optPar.cpp -o prog -w

# ./prog
#cmake -H. -Bbuild -DCMAKE_CXX_COMPILER=g++
cmake --build build
./build/main
# scp ~/Documents/NussinovDM/src/cuda.cu mgray7@13.88.243.86:/home/mgray7/
# ssh -i ~/.ssh/id_rsa mgray7@13.88.243.86


