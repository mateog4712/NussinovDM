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

void nussinovSimd(string sequence){

	//take len of the sequence for further use
	uint16_t len = sequence.length();
	//first we initialize the matrics D
	uint16_t var = -1;
	vector< vector<uint16_t> > table(len+16,vector<uint16_t>(len+16,var));
	
	
	
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
				
				if(mB!=0)
					m3 = table[i+1][j-1] - mB;
				
				uint16_t m4 = 0;
				for (uint16_t k = i+1;k<j+7;k+=8)
				{
					if(k+8> len){
						uint16_t n = len-8;
					__m128i const row1 = _mm_load_si128( (__m128i*) &table[i][n] );
					__m128i const row2 = _mm_load_si128( (__m128i*) &table[j][n] );
					__m128i result_values = _mm_add_epi16(row1,row2);
					__m128i max = _mm_minpos_epu16 (result_values);
					//vector<int> results = {_mm_extract_epi16(result_values,0),_mm_extract_epi16(result_values,1),_mm_extract_epi16(result_values,2),_mm_extract_epi16(result_values,3),_mm_extract_epi16(result_values,4),_mm_extract_epi16(result_values,5),_mm_extract_epi16(result_values,6),_mm_extract_epi16(result_values,7)};
					//for(uint16_t m=0;m<8;m++){ 
						//union { __m128i result_values; int16_t i16[8]; };
						
							//uint16_t value = results[m];
						
							if ( _mm_extract_epi16(max,0) > m4){
								m4 = _mm_extract_epi16(max,0);
							} 
					//}	
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
	cout << var-energy << endl;
	cout << structure << endl;
	//printTableI(table,sequence);
}

void printTableI(vector< vector<int> > T, string s){
	cout << "  ";
	for (auto i =0; i<s.size(); i++){
		cout << s[i] << " " ;
	}
	cout << endl;
	for(int i=0;i<T.size();i++){
		cout << s[i] << " ";
		for(int j=0;j<T.size();j++){

			cout << T[i][j] << " ";
		}
		cout<<endl;

	}


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