#ifndef SIMD
#define SIMD
#include <vector>
#include <string>

void nussinovSimd(std::string sequence);
void printVector(std::vector<uint16_t> v);
std::string tracebackS(std::vector< std::vector<uint16_t> > & table, int i, int j, std::string sequence);


#endif