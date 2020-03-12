#include "optimized.hpp"
#include "baseline.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>
#include "nmmintrin.h" // for SSE4.2
#include "immintrin.h" // for AVX


using namespace std;

void nussinovSimd(string sequence){

	//take len of the sequence for further use
	uint16_t len = sequence.length();
    __m256 row1;
    __m256 row2;
	

	//first we initialize the matrics D
	vector< vector<uint16_t> > table(len,vector<uint16_t>(len));
	
	
	for (uint16_t i =len-1;i>=0;i--)
	{
		for(uint16_t j=i;j<len;j++)
		{
			if (i<j)
			{
				uint16_t m1 = table[i][j-1];
				uint16_t m2 = table[i+1][j];


				auto mB = match(sequence,i,j);
				uint16_t m3 = 0;
				
				if(mB)
					m3 = table[i+1][j-1] + mB;
				
				uint16_t m4 = 0;
				for (uint16_t k = i+1;k<j;k+=8)
				{
                    row1 = _mm_load_ps(table[i][k],table[i][k+1],table[i][k+2],table[i][k+3],table[i][k+4], table[i][k+5], table[i][k+6], table[i][k+7]);


					if (table[i][k] + table[j][k+1] > m4)
						m4 = table[i][k] + table[j][k+1];
				}
				
				uint16_t ins = max(m1,max(m2,max(m3,m4)));
				table[i][j] = ins;
				table[j][i] = ins;

			}
		}
		if(i==0)
		{
		    break;
		} 
	}
	string structure = "";
	uint16_t energy = table[0][len-1];
	structure = traceback(table, 0, len-1, sequence);
}