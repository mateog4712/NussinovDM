#include <vector>
#include <iostream>
 #include "baseline.hpp"
#include <string>
#include "testCases.cpp"
#include <chrono>
#include <unistd.h>
#include "optimized.hpp"
#include "bottomUp.hpp"
#include "removedBranching.hpp"
#include "variation.hpp"
#include "transp2mtr.hpp"

using namespace std;
int main() {
	int numofIter = 5;
	double avg = 0.0;
	for (int j=0;j<5;j++)
	{

		auto start = chrono::steady_clock::now();
		for(auto i=0;i<testData.size();i++){

			// nussinov(testData[i]);
			cout << "---- bottoms" << endl;
			nussinovBottom(testData[i]);
			cout << "---- branch" << endl;
			nussinovBranch(testData[i]);
			cout << "---- opt" << endl;
			nussinovOpt(testData[i]);

		}
		auto end = chrono::steady_clock::now();

		

		avg += chrono::duration_cast<chrono::seconds>(end-start).count();
	}
	avg = avg / numofIter;
	cout << "Average elapsed time: " << avg << endl;

	// start = chrono::steady_clock::now();
	// for(auto i=0;i<testData.size();i++){

	// 	// nussinov(testData[i]);
	// 	nussinovOpt2(testData[i]);
		
	// }
	// end = chrono::steady_clock::now();

	// cout << "elapsed time:" << chrono::duration_cast<chrono::seconds>(end-start).count() << endl;

	
	return 0;
}
