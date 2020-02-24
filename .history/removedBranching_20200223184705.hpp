#ifndef REMOVEDBRANCHING
#define REMOVEDBRANCHING
#include <vector>
#include <string>


bool match3(std::string &sequence,uint16_t i,uint16_t j);

void nussinov3(std::string sequence);


std::string traceback3(std::vector< std::vector<uint16_t> > &table, uint16_t i, uint16_t j, std::string sequence);



#endif