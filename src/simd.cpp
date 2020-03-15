#include "simd.hpp"
#include "baseline.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>
//#include "nmmintrin.h" // for SSE4.2
#include <immintrin.h> // for AVX
#include <type_traits>


//os.environ['CFLAGS'] = '-mavx';

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
	
	
	
	for (uint16_t i =len-1;i>=0;i--)
	{
		for(uint16_t j=i;j<len;j++)
		{
			if (i<j)
			{
				uint16_t m1 = table[i][j-1];
				uint16_t m2 = table[i+1][j];

				auto mB = match(sequence,i,j);
				uint16_t m3 = var;
				
				if(mB!=0)
					m3 = table[i+1][j-1] + mB;
				
				uint16_t m4 = var;

				for (uint16_t k = i+1;k<j+step-1;k+=step)
				{
					uint16_t n = k;
					if(k+step> len){
						n = len-step;
					}
					__m128i const row1 = _mm_loadu_si128( (__m128i*) &table[i][n] );
					__m128i const row2 = _mm_loadu_si128( (__m128i*) &table[j][n + 1] );
					__m128i result_values = _mm_add_epi16(row1,row2);
					vector<uint16_t> temp = {-1,-1,-1,-1,-1,-1,-1,-1};
					// printVector(temp);
					__m128i tempr = _mm_loadu_si128( (__m128i*) &temp[0] );
					// vector<uint16_t> results = {_mm_extract_epi16(result_values,0),_mm_extract_epi16(result_values,1),_mm_extract_epi16(result_values,2),_mm_extract_epi16(result_values,3),_mm_extract_epi16(result_values,4),_mm_extract_epi16(result_values,5),_mm_extract_epi16(result_values,6),_mm_extract_epi16(result_values,7)};
					// cout << "results";
					// cout << endl;
					// printVector(results);

					// results = {_mm_extract_epi16(tempr,0),_mm_extract_epi16(tempr,1),_mm_extract_epi16(tempr,2),_mm_extract_epi16(tempr,3),_mm_extract_epi16(tempr,4),_mm_extract_epi16(tempr,5),_mm_extract_epi16(tempr,6),_mm_extract_epi16(tempr,7)};
					// cout << "tempr";
					// cout << endl;
					// printVector(results);

					__m128i xor_result_values = _mm_xor_si128(result_values, tempr);



					// results = {_mm_extract_epi16(xor_result_values,0),_mm_extract_epi16(xor_result_values,1),_mm_extract_epi16(xor_result_values,2),_mm_extract_epi16(xor_result_values,3),_mm_extract_epi16(xor_result_values,4),_mm_extract_epi16(xor_result_values,5),_mm_extract_epi16(xor_result_values,6),_mm_extract_epi16(xor_result_values,7)};
					// cout << "one";
					// cout << endl;
					// printVector(results);
					
					__m128i min = _mm_minpos_epu16 (xor_result_values);
					
					xor_result_values = _mm_xor_si128(min, tempr);
					// results = {_mm_extract_epi16(xor_result_values,0),_mm_extract_epi16(xor_result_values,1),_mm_extract_epi16(xor_result_values,2),_mm_extract_epi16(xor_result_values,3),_mm_extract_epi16(xor_result_values,4),_mm_extract_epi16(xor_result_values,5),_mm_extract_epi16(xor_result_values,6),_mm_extract_epi16(xor_result_values,7)};
					// cout << "two";
					// cout << endl;
					// printVector(results);

					//vector<int> results = {_mm_extract_epi16(result_values,0),_mm_extract_epi16(result_values,1),_mm_extract_epi16(result_values,2),_mm_extract_epi16(result_values,3),_mm_extract_epi16(result_values,4),_mm_extract_epi16(result_values,5),_mm_extract_epi16(result_values,6),_mm_extract_epi16(result_values,7)};
					//for(uint16_t m=0;m<8;m++){ 
						//union { __m128i result_values; int16_t i16[8]; };
						
							//uint16_t value = results[m];
							uint16_t rez = _mm_extract_epi16(xor_result_values,0);
							if ( rez > m4){
								m4 = rez;
							}
					//}	
					// break;
								
				}
				// break;
				
				// uint16_t ins = min(m1,min(m2,min(m3,m4)));
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
	cout << energy << endl;
	cout << structure << endl;
	printTable(table,sequence);
}


//int val0 = _mm_extract_epi16(result_values,0);
						//int val1 = _mm_extract_epi16(result_values,1);
						//int val2 = _mm_extract_epi16(result_values,2);
						//int val3 = _mm_extract_epi16(result_values,3);
						//int val4 = _mm_extract_epi16(result_values,4);
						//int val5 = _mm_extract_epi16(result_values,5);
						//int val6 = _mm_extract_epi16(result_values,6);
						//int val7 = _mm_extract_epi16(result_values,7);
						
						//m4 = max (m4,max(val0,max(val1,max(val2,max(val3,max(val4,max(val5,max(val6,val7))))))));
string tracebackS(vector< vector<uint16_t> > & table, int i, int j, string sequence){

	if(i>j)
	{
		return "";
	}

	if(table[i+1][j-1] - 1 == table[i][j] and match(sequence,i,j))
	{

	return "(" + tracebackS(table,i+1,j-1,sequence) + ")";
	}
	else if(table[i+1][j] == table[i][j])
	{
		return "." + tracebackS(table,i+1,j,sequence);
	}
	else if(table[i][j-1] == table[i][j])
	{
		return tracebackS(table,i,j-1,sequence) + ".";
	}
	else{

		for(uint16_t k = i+1; k<j;k++){

			if(table[i][k] + table[k+1][j] == table[i][j]){
			return tracebackS(table,i,k,sequence) + tracebackS(table,k+1,j,sequence);		
			}
		}
	}
	return "";
}