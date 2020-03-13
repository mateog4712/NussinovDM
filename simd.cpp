#include "simd.hpp"
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
	int len = sequence.length();
	//first we initialize the matrics D
	vector< vector<int> > table(len+16,vector<int>(len+16));
	
	
	
	for (int i =len-1;i>=0;i--)
	{
		for(int j=i;j<len;j++)
		{
			if (i<j)
			{
				int m1 = table[i][j-1];
				int m2 = table[i+1][j];


				auto mB = match(sequence,i,j);
				int m3 = 0;
				
				if(mB!=0)
					m3 = table[i+1][j-1] + mB;
				
				int m4 = 0;
				for (int k = i+1;k<j;k+=8)
				{
					__m128i const row1 = _mm_loadu_si128( (__m128i*) &table[i][k] );
					__m128i const row2 = _mm_loadu_si128( (__m128i*) &table[j][k] );
					__m128i result_values = _mm_add_epi16(row1,row2);
					
					vector<uint16_t> results = {_mm_extract_epi16(result_values,0),_mm_extract_epi16(result_values,1),_mm_extract_epi16(result_values,2),_mm_extract_epi16(result_values,3),_mm_extract_epi16(result_values,4),_mm_extract_epi16(result_values,5),_mm_extract_epi16(result_values,6),_mm_extract_epi16(result_values,7)};
					for(int m=0;m<8;m++){ 
						//union { __m128i result_values; int16_t i16[8]; };
						int value = results[m];
						//int value = _mm_extract_epi16(result_values,7);
						//cout<< value<<endl;
						if ( value > m4)
							m4 = value; 
					}				
				}

				cout << m1 << " " << m2 << " " << m3 << " " << m4 << " " << endl;
				int ins = max(m1,max(m2,max(m3,m4)));
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
	int energy = table[0][len-1];
	//structure = traceback(table, 0, len-1, sequence);
	cout << energy << endl;
	//printTableI(table,sequence);
}

void printTableI(vector< vector<int> > T, string s){
	cout << "  ";
	for (auto i =0; i<s.size(); i++){
		cout << s[i] << " " ;
	}
	cout << endl;
	for(uint16_t i=0;i<T.size();i++){
		cout << s[i] << " ";
		for(uint16_t j=0;j<T.size();j++){

			cout << T[i][j] << " ";
		}
		cout<<endl;

	}


}