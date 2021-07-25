#include <vector>
#include <iostream>
#include "baseline.hpp"
#include <string>
#include "testCases.hpp"
#include <chrono>
#include <unistd.h>
#include "optimized.hpp"
#include "bottomUp.hpp"
#include "removedBranching.hpp"
#include "variation.hpp"
#include "transp2mtr.hpp"
#include <ctime>
#include "recursion.hpp"
#include "simd.hpp"
#include "optPar.hpp"
#include <fstream>
#include "cases.hpp"


using namespace std;





int main() {

	int numofIter = 1;
	double avg = 0.0;
	cout << "Baseline: ";
	vector<string> test = {seq_3200, seq2_3200};
	// return 0;

	for (int j=0;j<numofIter;j++)
	{
		auto start = chrono::steady_clock::now();
		// for(auto i=0;i<testData.size();i++){
			nussinovSimd(seq2_3200);
			// cout << seq_10k.length();
		// }
		auto end = chrono::steady_clock::now();
		avg += chrono::duration_cast<chrono::seconds>(end-start).count();

	}
	// ofstream out;
	// out.open("cases.txt", ofstream::trunc);

	// for(int i=1; i<64; i*=2){
	// 	for(int j=0;j<2;j++){
	// 	string seq = createDataS(i*100);
	// 	out << "seq_" << i*100 << "= " << seq << "\n";
	// 	}
	// } 
	// out.close();
	// avg = avg / numofIter;
	cout << "Average elapsed time: " << avg << endl;
	
}
