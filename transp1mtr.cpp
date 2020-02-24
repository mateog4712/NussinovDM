#include "optimized.hpp"
#include "baseline.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>


using namespace std;

bool match5(string &sequence,uint16_t i,uint16_t j)
{
	if ( ((sequence[i] == 'A' and sequence[j] == 'U' ) or (sequence[i] == 'G' and sequence[j] == 'C' )) and j-i>3)
		return 1;

	return 0;
}



void nussinov5(string sequence){

	//take len of the sequence for further use
	uint16_t len = sequence.length();
	

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


				auto mB = match5(sequence,i,j);
				uint16_t m3 = 0;
				
				if(mB)
					m3 = table[i+1][j-1] + mB;
				
				uint16_t m4 = 0;
				for (uint16_t k = i+1;k<j;k++)
				{
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


	//aprintTable(table);
	//cout<<endl;
	//printTable(tableT);

	string structure = "";
	uint16_t energy = table[0][len-1];

	// structure = tracebackOpt(table, 0, len-1, sequence);

	// cout << sequence << endl;
	// cout << structure << endl;
	cout << energy << endl;

}



// This will traceback through the table defined in nussinov
string traceback5(vector< vector<uint16_t> > & table, uint16_t i, uint16_t j, string sequence){

    if(i>j)
    {
        return "";
    }

    if(table[i+1][j-1] + 1 == table[i][j] and match4(sequence,i,j))
    {

    return "(" + traceback5(table,i+1,j-1,sequence) + ")";
    }
    else if(table[i+1][j] == table[i][j])
    {
        return "." + traceback5(table,i+1,j,sequence);
    }
    else if(table[i][j-1] == table[i][j])
    {
        return traceback5(table,i,j-1,sequence) + ".";
    }
    else{

        for(uint16_t k = i+1; k<j;k++){

            if(table[i][k] + table[k+1][j] == table[i][j]){
            return traceback5(table,i,k,sequence) + traceback5(table,k+1,j,sequence);		
            }
        }
    }
}