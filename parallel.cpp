#include "parallel.hpp"
#include "baseline.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>
#include <omp.h>


using namespace std;

void nussinovPar(string sequence){

	//take len of the sequence for further use
	uint16_t len = sequence.length();
	// cout<< "Length: "<< len << endl;

	//first we initialize the matrics D
	vector< vector<uint16_t> > table(len,vector<uint16_t>(len));
	//printTable(table);

	for (uint16_t j =1;j<len;j++)
	{
		auto j0 =j;
		for(uint16_t i=0;i<len - j0;i++)
		{
			if (i<j)
			{
				uint16_t m1 = table[i][j-1];
				uint16_t m2 = table[i+1][j];
				uint16_t m3;
				if(match(sequence,i,j) )
					m3 = table[i+1][j-1] + match(sequence,i,j);
				else 
					m3 = 0;
				uint16_t m4 = 0;
				for (uint16_t k = i+1;k<j;k++)
				{
					if (table[i][k] + table[k+1][j] > m4)
						m4 = table[i][k] + table[k+1][j];
				}
				table[i][j] = max(m1,max(m2,max(m3,m4)));
			}
			j++;
		}
		j = j0;
	}

//cout<<endl;
// printTable(table);

 string structure = "";
 uint16_t energy = table[0][len-1];

structure = traceback(table, 0, len-1, sequence);

// cout << sequence << endl;
// cout << structure << endl;
 cout << energy << endl;

}