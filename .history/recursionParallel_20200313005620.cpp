
#include "baseline.hpp"
#include "recursionParallel.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>
#include <omp.h>

using namespace std;

uint16_t nussinovRecursionParallel(std::string & sequence, int i, int j)
{
    
    if(j <= i) 
    {
        return 0;
    }
    #pragma omp parallel
    {
    #pragma omp single 
    
    {
        
        uint16_t m3 = 0;
        #pragma omp task
        {
            bool mB = match(sequence,i,j);

            if (mB !=0) {
            // cout << i << " " << j  <<endl;
                m3 = nussinovRecursion(sequence,i+1,j-1) + mB;
            }
        }
        
        uint16_t m1 = 0;

        #pragma omp task
        {
            uint16_t m1 = nussinovRecursion(sequence,i,j-1);
        }
        
        uint16_t m2 = 0;
        #pragma omp task
        {
            uint16_t m2 = nussinovRecursion(sequence,i+1,j);
        }
        
        uint16_t m4 = 0;
        for (int k = i+1; k < j;k++) 
        {
            uint16_t n1 = 0;
            uint16_t n2 = 0;
            #pragma omp task 
            {
                auto n1 = nussinovRecursion(sequence,i,k);
            }
            
            #pragma omp task
            {
                auto n2 = nussinovRecursion(sequence,k+1,j);
            }
            
            uint16_t temp = n1+ n2;
            if (temp > m4) {
                m4 = temp;
            }
        }
        return max(m1,max(m2,max(m3,m4)));
    }
    }

}