#ifndef OPTIMIZED
#define OPTIMIZED
#include <vector>
#include <string>


bool match(std::string &sequence,uint16_t i,uint16_t j);

void nussinov(std::string sequence);

std::string traceback(std::vector< std::vector<uint16_t> > &table, uint16_t i, uint16_t j, std::string sequence);

void printTable(auto T);


#endif