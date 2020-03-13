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
#include "parallel.hpp"
#include "recursion.hpp"
#include "recursionParallel.hpp"
#include "simd.hpp"

using namespace std;
int main() {
	/** Definition of Variables and creation of data **/
	//srand(time(0));
	int numofIter = 5;
	double avg = 0.0;
	vector<string> testData = [s1,s2];
	cout << "Creating data...";
	createData(testData,1, 10);
	
	cout << "Calculating Nussinov...";
	/** The functions are run **/
	for (int j=0;j<numofIter;j++)
	{
		auto start = chrono::steady_clock::now();
		for(auto i=0;i<testData.size();i++){

			nussinovOpt(testData[i]);

		}
		auto end = chrono::steady_clock::now();

		

		avg += chrono::duration_cast<chrono::seconds>(end-start).count();
	}
	avg = avg / numofIter;

	cout << "Average elapsed time: " << avg << endl;
	avg = 0.0;
	for (int j=0;j<numofIter;j++)
	{
		auto start = chrono::steady_clock::now();
		for(auto i=0;i<testData.size();i++){

			nussinovRecursion(testData[i],0,testData[i].size());

		}
		auto end = chrono::steady_clock::now();

		

		avg += chrono::duration_cast<chrono::seconds>(end-start).count();
	}
	avg = avg / numofIter;

	cout << "Average elapsed time: " << avg << endl;

	avg = 0.0;
	for (int j=0;j<numofIter;j++)
	{
		auto start = chrono::steady_clock::now();
		for(auto i=0;i<testData.size();i++){

			nussinovRecursionParallel(testData[i],0,testData[i].size());

		}
		auto end = chrono::steady_clock::now();

		

		avg += chrono::duration_cast<chrono::seconds>(end-start).count();
	}
	avg = avg / numofIter;

	cout << "Average elapsed time: " << avg << endl;



}
// 	avg = 0;
// 	for (int j=0;j<numofIter;j++)
// 	{
		
// 		auto start = chrono::steady_clock::now();
// 		for(auto i=0;i<testData.size();i++){

// 			nussinovSimd(testData[i]);

// 		}
// 		auto end = chrono::steady_clock::now();

// 		avg += chrono::duration_cast<chrono::seconds>(end-start).count();
// 		cout << endl;
// 	}


// 	avg = avg / numofIter;
// 	cout << "Average elapsed time: " << avg << endl;
// 	return 0;
// }
