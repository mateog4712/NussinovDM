#include "baseline.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <utility>


using namespace std;

bool match(string &sequence,uint16_t i,uint16_t j)
{
	if ( ((sequence[i] == 'A' and sequence[j] == 'U' ) or (sequence[i] == 'G' and sequence[j] == 'C' )) and j-i>3)
		return 1;

	return 0;
}

void printTable(auto T){
	for(uint16_t i=0;i<T.size();i++){
		for(uint16_t j=0;j<T.size();j++){

			cout << T[i][j] << " ";
		}
		cout<<endl;

	}


}

void nussinov(string sequence){

	//take len of the sequence for further use
	uint16_t len = sequence.length();
	cout<< "Length: "<< len << endl;

	//first we initialize the matrics D
	vector< vector<uint16_t> > table(len,vector<uint16_t>(len));
	//printTable(table);

	for (uint16_t i =len-1;i>=0;i--)
	{
		for(uint16_t j=i;j<len;j++)
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
					//cout << m1 << " " << m2 << " " << m3 << " " << m4 << endl;
					table[i][j] = max(m1,max(m2,max(m3,m4)));
			}
		}
		if(i==0)
		{
		break;
		} 
	}

//cout<<endl;
//printTable(table);

string structure = "";
uint16_t energy = table[0][len-1];

structure = traceback(table, 0, len-1, sequence);

cout << sequence << endl;
cout << structure << endl;
cout << energy << endl;

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
}