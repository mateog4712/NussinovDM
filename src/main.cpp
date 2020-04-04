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


using namespace std;
int main() {
	//string s12 = "UAAGGCAGUGGUAGCGCUGAAGAAUGUUCGUGCAAUUGUCGUUAUUCAUUAUAAAAGGGCGGGUUUUAAAGGAUAUUUUAAAAUUUAAAACAAGCUUUUAAGAGCAGAUGGCGGAUGCCUUGCCAAAGAGAGGCGAUGAAGGACGUACUAGACUGCGAUAAGCUAUGCGGAGCUGUCAAGGAGCUUUGAUGCGUAGAUGUCCGAAUGGGGCAACCCAACUAAUAGAGAUAUUAGUUACUCUAACAGAGAGCGAACCUAGUGAAGUGAAACAUCUCAGUAACUAGAGGAAAAGAAAUCAACGAGAUUCCCUAAGUAGUGGCGAGCGAACGGGGAAAAGGGCAAACCGAGUGCUUGCAUUCGGGGUUGAGGACUGCAACAUCCAAGAGAACGCUUUAGCAGAGUUACCUGGAAAGGUAAGCCAUAGAAAGUGAUAGCCUUGUAUGCGACAAGGCGUUUUUAGGUAGCAGUAUCCAGAGUAGGCCAGGACACGAGGAAUCCAGGUUGAAGCCGGGGAGACCACUCUCCAACUCUAAAUACUACUCUUUGAGCGAUAGCGAACAAGUACCGUGAGGGAAAGGUGAAAAGAACCGCAGUGAGCGGAGUGAAAUAGAACCUGAAACCAUCUGCUUACAAUCAUUCAGAGCCCUAUGAUUUAUCAGGGUGAUGGACUGCCUUUUGCAUAAUGAUCCUGCGAGUUGUGGUAUCUGGCAAGGUUAAGCGAAUGCGAAGCCGUAGCGAAACGAGUUCUUAAUAGGGCGAACAAGUCAGAUGCUGCAGACCCGAAGCUAAGUGAUCUAUCCAUGGCCAAGUUGAAACGCGUGUAAUAGCGCGUGGAGGACUGAACUCCUACCCAUUGAAACGGGUUGGGAUGAGCUGUGGAUAGGGGUGAAAGGCCAAACAAACUUAGUGAUAGCUGGUUCUCUUCGAAAUAUAUUUAGGUAUAGCCUCAAGUGAUAAUAAAAGGGGGUAGAGCGCUGAUUGGGCUAGGGCUGCUCGCCGCGGUACCAAACCCUAUCAAACUUCGAAUACCUUUUAUCGUAUCUUGGGAGUCAGGCGGUGGGUGAUAAAAUCAAUCGUCAAAAGGGGAACAACCCAGACUACCAAAUAAGGUCCCUAAGUUCUAUUCUGAGUGGAAAAAGAUGUGUGGCUACUAAAACAACCAGGAGGUUGGCUUAGAAGCAGCCAUCCUUUAAAGAAAGCGUAACAGCUCACUGGUCUAGUGGUCAUGCGCUGAAAAUAUAACGGGGCUAAGAUAGACACCGAAUUUGUAGAUUGUGUUAAACACAGUGGUAGAAGAGCGUUCAUACCAGCGUUGAAGGUAUACCGGUAAGGAGUGCUGGAGCGGUAUGAAGUGAGCAUGCAGGAAUGAGUAACGAUAAGAUAUAUGAGAAUUGUAUCCGCCGUAAAUCUAAGGUUUCCUACGCGAUGGUCGUCAUCGUAGGGUUAGUCGGGUCCUAAGCCGAGUCCGAAAGGGGUAGGUGAUGGCAAAUUGGUUAAUAUUCCAAUACCGACUGUGGAGCGUGAUGGGGGGACGCAUAGGGUUAAGCGAGCUAGCUGAUGGAAGCGCUAGUCUAAGGGCGUAGAUUGGAGGGAAGGCAAAUCCACCUCUGUAUUUGAAACCCAAACAGGCUCUUUGAGUCCUUUUAGGACAAAGGGAGAAUCGCUGAUACCGUCGUGCCAAGAAAAGCCUCUAAGCAUAUCCAUAGUCGUCCGUACCGCAAACCGACACAGGUAGAUGAGAUGAGUAUUCUAAGGCGCGUGAAAGAACUCUGGUUAAGGAACUCUGCAAACUAGCACCGUAAGUUCGCGAUAAGGUGUGCCACAGCGAUGUGGUCUCAGCAAAGAGUCCCUCCCGACUGUUUACCAAAAACACAGCACUUUGCCAACUCGUAAGAGGAAGUAUAAGGUGUGACGCCUGCCCGGUGCUCGAAGGUUAAGAGGAUGCGUCAGUCGCAAGAUGAAGCGUUGAAUUGAAGCCCGAGUAAACGGCGGCCGUAACUAUAACGGUCCUAAGGUAGCGAAAUUCCUUGUCGGUUAAAUACCGACCUGCAUGAAUGGCGUAACGAGAUGGGAGCUGUCUCAACCAGAGAUUCAGUGAAAUUGUAGUGGAGGUGAAAAUUCCUCCUACCCGCGGCAAGACGGAAAGACCCCGUGGACCUUUACUACAACUUAGCACUGCUAAUGGGAAUAUCAUGCGCAGGAUAGGUGGGAGGCUUUGAAGUAAGGGCUUUGGCUCUUAUGGAGUCAUCCUUGAGAUACCACCCUUGAUGUUUCUGUUAGCUAACUGGCCUGUGUUAUCCACAGGCAGGACAAUGCUUGGUGGGUAGUUUGACUGGGGCGGUCGCUCCUAAAAAGUAACGGAGGCUUGCAAAGGUUGGCUCAUUGCGGUUGGAAAUCGCAAGUUGAGUGUAAUGGCACAAGCCAGCCUGACUGUAAGACAUACAAGUCAAGCAGAGACGAAAGUCGGUCAUAGUGAUCCGGUGGUUCUGUGUGGAAGGGCCAUCGCUCAAAGGAUAAAAGGUACCCCGGGGAUAACAGGCUGAUCUCCCCCAAGAGCUCACAUCGACGGGGAGGUUUGGCACCUCGAUGUCGGCUCAUCGCAUCCUGGGGCUGGAGCAGGUCCCAAGGGUAUGGCUGUUCGCCAUUUAAAGCGGUACGCGAGCUGGGUUCAGAACGUCGUGAGACAGUUCGGUCCCUAUCUGCCGUGGGCGUAGGAAAGUUGAGGAGAGCUGUCCCUAGUACGAGAGGACCGGGAUGGACGUGUCACUGGUGCACCAGUUGUCUGCCAAGAGCAUCGCUGGGUAGCACACACGGAUGUGAUAACUGCUGAAAGCAUCUAAGCAGGAACCAACUCCAAGAUAAACUUUCCCUGAAGCUCGCACAAAGACUAUGUGCUUGAUAGGGUAGAUGUGUGAGCGCAGUAAUGCGUUUAGCUGACUACUACUAAUAGAGCGUUUGGCUUGUUUUU";
	//cout << s12.length() << endl;
	/** Definition of Variables and creation of data **/
	//srand(time(0));
	//nussinovSimd2(s12);
	int numofIter = 5;
	
	// vector<string> testData;// = {"AAGGGGUU"};
	//cout << "Creating data..." << endl;
	// createData(testData,5, 100x00);
	
	// cout << "Calculating Nussinov..." << endl;
	/** The functions are run **/


	double avg = 0.0;
	cout << "Simd: ";

	for (int j=0;j<numofIter;j++)
	{
		auto start = chrono::steady_clock::now();
		for(auto i=0;i<testData.size();i++){
			
			nussinovSimd(testData[i]);
		}
		auto end = chrono::steady_clock::now();
		avg += chrono::duration_cast<chrono::seconds>(end-start).count();

	}
	avg = avg / numofIter;
	cout << "Average elapsed time (SIMD): " << avg << endl;

	avg = 0.0;
	cout << "Optimized: ";
	for (int j=0;j<numofIter;j++)
	{
		auto start = chrono::steady_clock::now();
		for(auto i=0;i<testData.size();i++){
			cout << "Simd: ";
			nussinovOpt(testData[i]);
		}
		auto end = chrono::steady_clock::now();
		avg += chrono::duration_cast<chrono::seconds>(end-start).count();
	}
	avg = avg / numofIter;
	cout << "Average elapsed time (Optimized single core): " << avg << endl;
	// 	auto end = chrono::steady_clock::now();
	// 	avg += chrono::duration_cast<chrono::seconds>(end-start).count();
	// }
	// avg = avg / numofIter;
	// cout << "Average elapsed time: " << avg << endl;
	// avg =0;
	// cout << "Calculating Nussinov..." << endl;
	// avg = 0.0;
	// for (int j=0;j<numofIter;j++)
	// {
	// 	auto start = chrono::steady_clock::now();
	// 	for(auto i=0;i<testData.size();i++){

	// 		nussinovSimd(testData[i]);

	// 	}
	// 	auto end = chrono::steady_clock::now();
	// 	avg += chrono::duration_cast<chrono::seconds>(end-start).count();
	// }
	// avg = avg / numofIter;
	// cout << "Average elapsed time: " << avg << endl;
	// avg =0;
	// cout << "Calculating Nussinov..." << endl;
	// for (int j=0;j<numofIter;j++)
	// {
	// 	auto start = chrono::steady_clock::now();
	// 	for(auto i=0;i<testData.size();i++){

	// 		nussinovSimd2(testData[i]);

	// 	}
	// 	auto end = chrono::steady_clock::now();
	// 	avg += chrono::duration_cast<chrono::seconds>(end-start).count();
	// }
	// avg = avg / numofIter;
	// cout << "Average elapsed time: " << avg << endl;



	// /** The functions are run **/
	// avg = 0.0;
	// for (int j=0;j<numofIter;j++)
	// {
	// 	auto start = chrono::steady_clock::now();
	// 	for(auto i=0;i<testData.size();i++){

	// 		nussinovSimd(testData[i]);

	// 	}
	// 	auto end = chrono::steady_clock::now();
	// avg += chrono::duration_cast<chrono::seconds>(end-start).count();
	// }
	// avg = avg / numofIter;
	// cout << "Average elapsed time: " << avg << endl;
	
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
