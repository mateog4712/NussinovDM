#include "math.h" 	   // sqrtf()
#include <chrono>      // timing library
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <type_traits>


// Reuse statically generated data from previous lecture
#include "testCases.hpp"
//#include "../src/simd.hpp"
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
		#pragma omp parallel for 
		for(int i = 0;i<len-j0;i++)
		// for(int i=j-1;i>=0;i--)
		{
			int j = i+j0;
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
					
					uint16_t rez = _mm_extract_epi16(min,0);
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
			// cout <<  << endl;
		}
		// cout << "_____________" << endl;
		j=j0;
	}
	string structure = "";
	uint16_t energy = table[0][len-1];
	// structure = traceback(table, 0, len-1, sequence);
	// cout << energy << endl;
	// cout << structure << endl;
	// printTableFile(table,sequence, "Simd");

bool match(string &sequence,uint16_t i,uint16_t j)
{
	if ( ((sequence[i] == 'A' and sequence[j] == 'U' ) or (sequence[i] == 'G' and sequence[j] == 'C' )) and j-i>3)
		return 1;

	return 0;
}
// This will traceback through the table defined in nussinov
string traceback(vector< vector<uint16_t> > & table, uint16_t i, uint16_t j, string sequence){

	if(i>j)
	{
		return "";
	}

	if(table[i+1][j-1] + 1 == table[i][j] and match(sequence,i,j))
	{

	return "(" + traceback(table,i+1,j-1,sequence) + ")";
	}
	else if(table[i+1][j] == table[i][j])
	{
		return "." + traceback(table,i+1,j,sequence);
	}
	else if(table[i][j-1] == table[i][j])
	{
		return traceback(table,i,j-1,sequence) + ".";
	}
	else{

		for(uint16_t k = i+1; k<j;k++){

			if(table[i][k] + table[k+1][j] == table[i][j]){
			return traceback(table,i,k,sequence) + traceback(table,k+1,j,sequence);		
			}
		}
	}
	return "";
}





}



int main{
/** Definition of Variables and creation of data **/
	
	int numofIter = 5;
	
	for (int j=0;j<numofIter;j++)
	{
		// auto start = chrono::steady_clock::now();
		for(auto i=0;i<testData.size();i++){

			nussinovSimd(testData[i]);

		}
    }
}
