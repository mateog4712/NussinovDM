#include <vector>
#include <string>
#include "testCases.hpp"
using namespace std;






/** This creates the data that is run by our functions. The minimum size for the data is 30	**/
void createData(vector<string> &testData, int NOS, int maxSize){


	const vector<char> bases = {'A','C','G','T'}; 


	for(int i = 0; i<NOS;++i){

	string sequence = "";
	int size = rand()%(maxSize-30) + 30;

		for(int j = 0; j<size;j++){

		int random = rand()%4;
		char base = bases[random];
		sequence = sequence + base;

		}
		//cout<< sequence << '\n'<<endl;

	testData.push_back(sequence);
	}
}

