#ifndef VARIATION
#define VARIATION
#include <vector>
#include <string>


bool matchOpt(std::string &sequence,uint16_t i,uint16_t j);

void nussinovOpt(std::string sequence);


std::string tracebackOpt(std::vector< std::vector<uint16_t> > &table, uint16_t i, uint16_t j, std::string sequence);

void printTable(auto T);


#endif