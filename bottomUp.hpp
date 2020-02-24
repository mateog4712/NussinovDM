#ifndef BOTTOMUP
#define BOTTOMUP
#include <vector>
#include <string>



bool match2(std::string &sequence,uint16_t i,uint16_t j);

void nussinov2(std::string sequence);

std::string traceback2(std::vector< std::vector<uint16_t> > &table, uint16_t i, uint16_t j, std::string sequence);



#endif