#!/usr/bin/env bash
g++ -o0 -std=c++17 baseline.cpp bottomUp.cpp main.cpp optimized.cpp parallel.cpp  removedBranching.cpp transp2mtr.cpp variation.cpp recursion.cpp recursionParallel.cpp simd.cpp optPar.cpp -o prog -w
./prog