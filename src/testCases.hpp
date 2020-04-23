#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H
#include <vector>
#include <string>


extern std::vector<std::string> testData;

void createData(std::vector<std::string> &testData, int NOS, int maxSize);

std::string createDataS(int size);
#endif