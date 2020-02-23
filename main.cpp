#include <vector>
#include <iostream>
//#include "baseline.hpp"
#include <string>
#include "testCases.cpp"
#include <chrono>
#include <unistd.h>
#include "optimized.hpp"

using namespace std;
int main() {
	string seq = "AGGACC";
	//nussinov(seq);

	auto start = chrono::steady_clock::now();
	for(auto i=0;i<testData.size();i++){

		nussinov(testData[i]);
	}
	auto end = chrono::steady_clock::now();

	cout << "elapsed time:" << chrono::duration_cast<chrono::seconds>(end-start).count() << endl;
	

	
	return 0;
}