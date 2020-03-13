#include "optimized.hpp"
#include "baseline.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>
#include "nmmintrin.h" // for SSE4.2
#include <immintrin.h> // for AVX
#include <emmintrin.h>
#include "tmmintrin.h"
//#include "avx512vlbwintrin.h"



using namespace std;

void nussinovSimd(string sequence){

	//take len of the sequence for further use
	uint16_t len = sequence.length();
	//first we initialize the matrics D
	vector< vector<uint16_t> > table(len+16,vector<uint16_t>(len+16));
	
	
	
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
					__m128i const row1 = _mm_loadu_si128( (__m128i*) &table[i][k] );
					__m128i const row2 = _mm_loadu_si128( (__m128i*) &table[j][k] );
					__m128i result_values = _mm_add_epi16(row1,row2);
					for(int m=0;m<8;m++){ 

						
						uint16_t value = _mm_extract_epi16(result_values,0);
						if ( value > m4)
							m4 = value; 
					}				
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