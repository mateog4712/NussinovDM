#ifndef OPTIMIZED
#define OPTIMIZED
#include <vector>
#include <string>


bool match5(std::string &sequence,uint16_t i,uint16_t j);

void nussinov5(std::string sequence);


std::string traceback5(std::vector< std::vector<uint16_t> > &table, uint16_t i, uint16_t j, std::string sequence);


#endif