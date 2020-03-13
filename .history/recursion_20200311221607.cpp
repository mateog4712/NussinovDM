#include "recursion.hpp"
#include "baseline.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

uint16_t nussinovRecursion(std::string & sequence, int i, int j,uint16_t score)
{
    cout << i << "   " << j << endl;
    if(j==-1){
        j = sequence.size();
    }
    
    if(j <= i) 
    {
        return 0;
    }
    uint16_t m1 = nussinovRecursion(sequence, i, j-1, score);
    uint16_t m2 = nussinovRecursion(sequence, i+1, j, score);
    uint16_t m3 = 0;
    bool mB = match(sequence,i,j);

    if (mB !=0) {
        uint16_t m3 = nussinovRecursion(sequence, i+1, j-1, score + mB);
    }
    uint16_t m4 = 0;
    for (int k = i+1; k < j; k++) 
    {
        uint16_t temp = nussinovRecursion(sequence,i,k,score) + nussinovRecursion(sequence,k+1,j,score);
        if (temp > m4) {
            m4 = temp;
        }
    }
    return max(m1,max(m2,max(m3,m4)));
}