#include "simd.hpp"
#include "baseline.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>
#include <immintrin.h> // for AVX
#include <type_traits>

using namespace std;


void printVector(vector<uint16_t> v){
	for(uint16_t i: v)
		cout << i << '\t';
	cout << endl;
}

void nussinovSimd(string sequence){

	//take len of the sequence for further use
	uint16_t len = sequence.length();
	//first we initialize the matrics D
	uint16_t var = 0;
	uint16_t tr1 = 8;
	uint16_t step = 8;
	vector< vector<uint16_t> > table(len+tr1,vector<uint16_t>(len+tr1,var));
	// Load a row of -1's unsigned to be used for xor
	vector<uint16_t> temp = {-1,-1,-1,-1,-1,-1,-1,-1};
	__m128i tempr = _mm_loadu_si128( (__m128i*) &temp[0] );
	
	for (int j = 1;j<len;j++)
	{
		auto j0 = j;
		// for(int i = 0;i<len-j0;i++)
		for(int i=j-1;i>=0;i--)
		{
			if (i<j)
			{
				uint16_t m1 = table[i][j-1];
				uint16_t m2 = table[i+1][j];

				auto mB = match(sequence,i,j);
				uint16_t m3 = var;
				
				if(mB)
					m3 = table[i+1][j-1] + mB;
				
				uint16_t m4 = var;

				for (uint16_t k = i+1;k<j;k+=step)
				{	// Load the two rows from the matrix
					__m128i const row1 = _mm_loadu_si128( (__m128i*) &table[i][k] );
					__m128i const row2 = _mm_loadu_si128( (__m128i*) &table[j][k + 1] );
					// add them
					__m128i result_values = _mm_add_epi16(row1,row2);
					
					// xor the results
					__m128i xor_result_values = _mm_xor_si128(result_values, tempr);
					// find the minimum in the results
					__m128i min = _mm_minpos_epu16 (xor_result_values);
					
					uint16_t rez = _mm_extract_epi16(xor_result_values,0);
					// xor the value back
					rez = rez ^ tempr[0];
					if ( rez > m4){
						m4 = rez;
					}	
						
				}
				// take the maximum of the four operations
				uint16_t ins = max(m1,max(m2,max(m3,m4)));
				table[i][j] = ins;
				table[j][i] = ins;
			}
			// j++;	
		}
		// j=j0;
	}
	string structure = "";
	uint16_t energy = table[0][len-1];
	structure = traceback(table, 0, len-1, sequence);
	// cout << energy << endl;
	// cout << structure << endl;
	// printTableFile(table,sequence, "Simd");
}