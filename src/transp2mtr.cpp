#include "transp2mtr.hpp"
#include "baseline.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>


using namespace std;




void nussinovDouble(string sequence){

	//take len of the sequence for further use
	uint16_t len = sequence.length();
	

	//first we initialize the matrics D
	vector< vector<uint16_t> > table(len,vector<uint16_t>(len));
	vector< vector<uint16_t> > tableT(len,vector<uint16_t>(len));
	
	
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
				for (uint16_t k = i+1;k<j;k++)
				{
					if (table[i][k] + tableT[j][k+1] > m4)
						m4 = table[i][k] + tableT[j][k+1];
				}
				
				uint16_t ins = max(m1,max(m2,max(m3,m4)));
				table[i][j] = ins;
				tableT[j][i] = ins;

			}
		}
		if(i==0)
		{
		break;
		} 
	}


	//aprintTable(table);
	//cout<<endl;
	//printTable(tableT);

	string structure = "";
	uint16_t energy = table[0][len-1];

	structure = traceback(table, 0, len-1, sequence);

	// cout << sequence << endl;
	// cout << structure << endl;
	//cout << energy << endl;

}