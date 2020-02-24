#ifndef TRANSP2MTR
#define TRANSP2MTR
#include <vector>
#include <string>


bool match4(std::string &sequence,uint16_t i,uint16_t j);

void nussinov4(std::string sequence);


std::string traceback4(std::vector< std::vector<uint16_t> > &table, uint16_t i, uint16_t j, std::string sequence);


#endif