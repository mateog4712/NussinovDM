#include <vector>
#include <iostream>
 #include "baseline.hpp"
#include <string>
#include "testCases.cpp"
#include "testCases.hpp"
#include <chrono>
#include <unistd.h>
#include "optimized.hpp"
#include "bottomUp.hpp"
#include "removedBranching.hpp"
#include "variation.hpp"
#include "transp2mtr.hpp"
#include <ctime>

using namespace std;
int main() {
	int main() {
	srand(time(0));
	int numofIter = 5;
	double avg = 0.0;
	vector<string> testData;
	createData(testData,3, 600);
	for (int j=0;j<2;j++)
	{
		
		auto start = chrono::steady_clock::now();
		for(auto i=0;i<testData.size();i++){

			
			nussinovOpt(testData[i]);

		}
		auto end = chrono::steady_clock::now();

		

		avg += chrono::duration_cast<chrono::seconds>(end-start).count();
		cout << endl;
	}
	avg = avg / numofIter;
	cout << "Average elapsed time: " << avg << endl;

	
	return 0;
}
