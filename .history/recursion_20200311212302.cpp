#include "recursion.hpp"
#include "baseline.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>



void nussinovRecursion(const std::string & sequence, int i=0, int j=sequence.size(),uint16_t score=0)
{
    if(j <= i) 
    {
        return 0;
    }
    uint16_t case1 = nussinovRecursion(i,j-1)

}