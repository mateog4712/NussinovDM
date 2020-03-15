#ifndef SIMD
#define SIMD
#include <vector>
#include <string>

void nussinovSimd(std::string sequence);
void printTableI(std::vector< std::vector<int> > T, std::string s);
std::string tracebackS(std::vector< std::vector<int> > & table, int i, int j, std::string sequence);


#endif