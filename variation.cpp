#include "variation.hpp"
#include "baseline.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>


using namespace std;


void nussinovVar(string sequence){

	//take len of the sequence for further use
	uint16_t len = sequence.length();
	// cout<< "Length: "<< len << endl;

	//first we initialize the matrics D
	vector< vector<uint16_t> > table(len,vector<uint16_t>(len));
	// vector< vector<uint16_t> > tableT(len,vector<uint16_t>(len));
	//printTable(table);
	
	for (uint16_t i =len-1;i>=0;i--)
	{
		for(uint16_t j=i;j<len;j++)
		{
			if (i<j)
			{

				auto mB = match(sequence,i,j);
				uint16_t m3 = 0;
				
				// m3 = table[i+1][j-1] * mB + mB*mB;

				// table[i][j] = max(table[i][j],m3);
				// tableT[j][i] = max(tableT[j][i],m3);
				if(mB)
					m3 = table[i+1][j-1] + mB;
				
				uint16_t m4 = 0;
				for (uint16_t k = i;k<j;k++)
				{
					if (table[i][k] + table[j][k+1] > m4)
						m4 = table[i][k] + table[j][k+1];
					// m4 =(table[i][k] + tableT[j][k+1] > m4) * (table[i][k] + tableT[j][k+1]) ;
				}
					//cout << m1 << " " << m2 << " " << m3 << " " << m4 << endl;

				// table[i][j] = max(table[i][j],m4);
				// tableT[j][i] = max(tableT[j][i],m4);
				// uint16_t ins = max(m3,m4);
				table[i][j] = max(m3,m4);
				table[j][i] = max(m3,m4);

			}
		}
		if(i==0)
		{
		break;
		} 
	}


//printTable(table);
//cout<<endl;
//printTable(tableT);

 string structure = "";
 uint16_t energy = table[0][len-1];

 structure = traceback(table, 0, len-1, sequence);

// cout << sequence << endl;
// cout << structure << endl;
//cout << energy << endl;

}