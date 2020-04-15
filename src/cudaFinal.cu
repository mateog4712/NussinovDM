#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <chrono>

#include <vector>

#include "stojanovski/stojanovski.cuh"


__global__ void add(int *a, int *b, int *c,int size) {
	c[size*blockIdx.x+ threadIdx.x] = a[size*blockIdx.x+ threadIdx.x] + b[size*blockIdx.x+ threadIdx.x];
}

__device__ bool match(char * sequence,int i,int j)
{
	if ( ((sequence[i] == 'A' and sequence[j] == 'U' ) or (sequence[i] == 'G' and sequence[j] == 'C' )) and j-i>3)
		return 1;
	return 0;
}

//or (sequence[i] == 'G' and sequence[j] == 'U' )
void printTable(int * T, char* s, int * len1){
	int len = *len1;
	// cout << "\t";
	// s.resize(T.size(),'$');
	printf("\t");
	printf("\n");
	printf("\t");
	for (int i =0; i<len; i++){
		printf("%c\t", s[i]);
	}
	printf("\n");
	for(int i=0;i<len;i++){
		printf("%c\t", s[i]);
		for(int j=0;j<len;j++){
			printf("%d\t", T[len*i +j]);
		}
		printf("\n");

	}
}

void writeTable(int * T, char* s, int * len1,FILE* fptr){
	int len = *len1;
	// cout << "\t";
	// s.resize(T.size(),'$');
	fprintf(fptr,"\t");
	fprintf(fptr,"\n");
	fprintf(fptr,"\t");
	for (int i =0; i<len; i++){
		fprintf(fptr,"%c\t", s[i]);
	}
	fprintf(fptr,"\n");
	for(int i=0;i<len;i++){
		fprintf(fptr,"%c\t", s[i]);
		for(int j=0;j<len;j++){
			fprintf(fptr,"%d\t", T[len*i +j]);
		}
		fprintf(fptr,"\n");

	}
}


__device__  void printTableGPU(int * T, char* s, int * len1){

	
	int len = *len1;
	// cout << "\t";
	// s.resize(T.size(),'$');
	printf("\t");
	printf("\n");
	printf("\t");
	for (int i =0; i<len; i++){
		printf("%c\t", s[i]);
	}
	printf("\n");
	for(int i=0;i<len;i++){
		printf("%c\t", s[i]);
		for(int j=0;j<len;j++){
			printf("%d %d\t", T[len*i +j],len*i +j);
		}
		printf("\n");

	}
}


__device__  void printArrayGPU(int * T,int * len1){
	
	
	int len = *len1;
	for(int j=0;j<len;j++){
		printf("%d\t", T[j]);
	}
	printf("\n");

}

__device__ void add2(int *c) {
	c[threadIdx.x] = blockIdx.x+ threadIdx.x;
}

// modified function to accept ONLY 4+ threads! (table, sequence,length,j,numOfBlocks,maxThreads,sharedVel);
__global__ void nussinovCuda(int * table, char * sequence,int * len1, int j, int numBlocks, int numThreads, int sharedVel) {
	
	int len = *len1;
	// printf("%d\t%d\t%d\t%d\t\n", numBlocks,numThreads,sharedVel,j);
	int begin = blockIdx.x; // threadIdx.x;
    int end = blockIdx.x;
	int blockLen =  (len- j) / numBlocks;

	//main 4 variables for storing mid results
	extern __shared__ int sh_array [];

    if (blockIdx.x == numBlocks - 1)
    {
        begin = begin * blockLen;
        blockLen += ( len - j ) - blockLen * numBlocks;
        end = begin + blockLen;
    }
    else
    {
        begin = begin * blockLen;
        end = begin + blockLen;
    }
    
	// printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",begin,end, blockLen,j,blockIdx.x,numBlocks,len);

	auto j0 = j;



    if (blockLen!=0)
	for(int i=begin; i<end; i++)
	{
		int j = i+j0;
		if (i<j)
		{
			switch (threadIdx.x)
			{
				case 0:
					sh_array[threadIdx.x] = table[(len*i)+(j-1)];
					break;
				case 1:
					sh_array[threadIdx.x] = table[(len*(i+1))+j];
					break;
				case 2:
					int m = match(sequence,i,j);
					if (m)
						sh_array[threadIdx.x] = table[(len*(i+1))+(j-1)] + match(sequence,i,j);
					else 
						sh_array[threadIdx.x] = 0;
					break;
				default:
					int num = 0;
					int first = threadIdx.x - 4;
					int NumFreeThreads =numThreads-3;
					// we are going to use threads in here
					for (int k = i+1 + first;k<j;k+= NumFreeThreads)
					{
						sh_array[threadIdx.x + num] = table[len * i + k ] + table[ len * j + (k+1)];
						num +=  NumFreeThreads;
					}

			}
			__syncthreads();
			if (threadIdx.x == 0)
			{
				int max =0;
				for (int i =0; i < sharedVel;i++){
					if (sh_array[i] > max)
						max = sh_array[i];
				}
				// transposed case
				table[len*j+i] = max;
				// normal case
				table[len*i+j] = max;
			}
		}
	}
	// __syncthreads();
	// printTableGPU(table, sequence, len1);
}


int main(void) {

	// reading from file
	double allSeconds =0;
	char myword[71 * 31719]; 
	
	FILE *stream;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
 
	stream = fopen("test.txt", "r");//sequence.fasta
	if (stream == NULL)
		exit(EXIT_FAILURE);
	int k = 0;
	int mover = 0;
	while ((read = getline(&line, &len, stream)) != -1) {
		
		if(line[0] != '>')
		{
			for (int i =0;i<read;i++){
				myword[mover+i] = line[i];
			}	
			if (myword[mover+read-1] == '\n')
				myword[mover+read-1] = '\0';
			mover+= read-1;
			if (k++ == 30) break;
		}
	}

	free(line);
	fclose(stream);

    char * allSequences[]= { myword,
	// "GGAAACACCU",
	// "GGAUACGGCCAUACUGCGCAGAAAGCAC",
	//  "GGAUACGGCCAUACUGCGCAGAAAGCACCGCUUCCCAUCCGAACAGCG",
	// "GGAUACGGCCAUACUGCGCAGAAAGCACCGCUUCCCAUCCGAACAGCGAAGUUAAGCUGCGCCAGGCGGUGUUAGUACUGGGGUGGGCGACCACCCGGGAAUCCACCGUGCCGUAUCCU",
	// "GGACCUGGUGGCUAUGGCGGGAGAGAUCCACCCGAUCCCAUCCCGAACUCGGCCGUGAAAACCCCCAGCGCCUAUGAUACUGCGGCUUAAGCCGUGGGAAAGUCGGUCGCCGCCAGGUCC",
	// "GCCAACGUCCAUACCACGUUGAAAGCACCGGUUCUCGUCCGAUCACCGAAGUUAAGCAGCGUCGGGCGCGGUUAGUACUUGGAUGGGUGACCGCCUGGGAACCCCGCGUGACGUUGGCAU",
	//  "UAGGUUUGGUCCUAGCCUUUCUAUUAACUCUUAGUAGGAUUACACAUGCAAGCAUCCCCGCCCCAGUGAGUCACCCUCUAAAUCACCACGAUCAAAAGGAACAAGCAUCAAGUACGCAGAAAUGCAGCUCAAAACGCUUAGCCUAGCCACACCCCCACGGGAGACAGCAGUGAUAAACCUUUAGCAAUAAACGAAAGUUUAACUAAGCCAUACUAACCCCAGGGUUGGUCAAUUUCGUGCCAGCCACCGCGGUCACACGAUUAACCCAAGCCAAUAGAAAUCGGCGUAAAGAGUGUUUUAGAUCAAUCCCCCAAUAAAGCUAAAAUUCACCUGAGUUGUAAAAAACUCCAGCUGAUAUAAAAUAAACUACGAAAGUGGCUUUAAUAUAUCUGAACACACAAUAGCUAGGACCCAAACUGGGAUUAGAUACCCCACUAUGCCUAGCCCUAAACUUCAACAGUUAAAUUAACAAGACUGCUCGCCAGAACACUACGAGCCACAGCUUAAAACUCAAAGGACCUGGCGGUGCUUCACAUCCUUCUAGAGGAGCCUGUUCUGUAAUCGAUAAACCCCGAUCAACCUCACCACCUCUUGCUCAGCCUAUAUACCGCCAUCUUCAGCAAACCCUGACGAAGGCCACAAAGUAAGCACAAGUACCCACGUAAAGACGUUAGGUCAAGGUGUAGCCCAUGAGGUGGCAAGAAAUGGGCUACAUUUUCUACUUCAGAAAACUACGAUAACCCUUAUGAAACCUAAGGGUAGAAGGUGGAUUUAGCAGUAAACUAAGAGUAGAGUGCUUAGUUGAACAGGGCCCUGAAGCGCGUACACACCGCCCGUCACCCUCCUCAAGUAUACUUCAAAGGACAUUUAACUAAAACCCCUACGCAUCUAUAUAGAGGAGAUAAGUCGUAACAUGGUAAGUGUACUGGAAAGUGCACUUGGACGAAC",
	// "UUUUCUGAGAAUUUGAUCUUGGUUCAGAUUGAACGCUGGCGGCGUGGAUGAGGCAUGCAAGUCGAACGGAAUAAUGACUUCGGUUGUUAUUUAGUGGCGGAAGGGUUAGUAAUACAUAGAUAAUCUGUCCUCAACUUGGGAAUAACGGUUGGAAACGACCGCUAAUACCGAAUGUGGUAUGUUUAGGCAUCUAAAACAUAUUAAAGAAGGGGAUCUUCGGACCUUUCGGUUGAGGGAGAGUCUAUGGGAUAUCAGCUUGUUGGUGGGGUAAUGGCCUACCAAGGCUUUGACGUCUAGGCGGAUUGAGAGAUUGACCGCCAACACUGGGACUGAGACACUGCCCAGACUUCUACGGAAGGCUGCAGUCGAGAAUCUUUCGCAAUGGACGAAAGUCUGACGAAGCGACGCCGCGUGUGUGAUGAAGGCUCUAGGGUUGUAAAGCACUUUCGCUUGGGAAUAAGAGAGAUUGGCUAAUAUCCAAUCGAUUUGAGCGUACCAGGUAAAGAAGCACCGGCUAACUCCGUGCCAGCAGCUGCGGUAAUACGGAGGGUGCUAGCGUUAAUCGGAUUUAUUGGGCGUAAAGGGCGUGUAGGCGGAAAGGAAAGUUAGAUGUUAAAUCUUGGGGCUCAACCCCAAGCCAGCAUCUAAUACUAUCUUUCUAGAGGGUAGAUGGAGAAAAGGGAAUUCCACGUGUAGCGGUGAAAUGCGUAGAUAUGUGGAAGAACACCAGUGGCGAAGGCGCUUUUCUAAUUUACACCUGACGCUAAGGCGCGAAAGCAAGGGGAGCAAACAGGAUUAGAUACCCUGGUAGUCCUUGCCGUAAACGAUGCAUACUUGAUGUGGAUAGUCUCAACCCUAUCCGUGUCGUAGCUAACGCGUUAAGUAUGCCGCCUGAGGAGUACACUCGCAAGGGUGAAACUCAAAAGAAUUGACGGGGGCCCGCACAAGCAGUGGAGCAUGUGGUUUAAUUCGAUGCAACGCGAAGAACCUUACCUGGGCUUGACAUGUAUUUGACCGCGGCAGAAAUGUCGUUUUCCGCAAGGACAGAUACACAGGUGCUGCAUGGCUGUCGUCAGCUCGUGCCGUGAGGUGUUGGGUUAAGUCCCGCAACGAGCGCAACCCUUAUCGUUAGUUGCCAACACUUAGGGUGGGAACUCUAACGAGACUGCCUGGGUUAACCAGGAGGAAGGCGAGGAUGACGUCAAGUCAGCAUGGCCCUUAUGCCCAGGGCUACACACGUGCUACAAUGGCCAGUACAGAAGGUAGCAAUAUCGUGAGAUGGAGCAAAUCCUCAAAGCUGGCCCCAGUUCGGAUUGUAGUCUGCAACUCGACUACAUGAAGUCGGAAUUGCUAGUAAUGGCGUGUCAGCUAUAACGCCGUGAAUACGUUCCCGGGCCUUGUACACACCGCCCGUCACAUCAUGGGAGUUGGUUUUGCCUUAAGUCGUUGACUCAACCUGCAAAGGAGAGAGGCGCCCAAGGUGAGGCUGAUGACUGGGAUGAAGUCGUAACAAGGUAGCCCUACCGGAAGGUGGGGCUGGAUCACCUCCUUU",
	//  "AUCCAUGGAGAGUUUGAUCCUGGCUCAGGACGAACGCUGGCGGCAUGCUUAACACAUGCAAGUCGAACGAGCAAAGCAAUUUGUGUAGUGGCGAACGGGUGCGUAACGCGUAAGAACCUACCUAUCGGAGGGGGAUAACAUUGGGAAACUGUUGCUAAUACCCCAUACAGCUGAGGAGUGAAAGGUGAAAAACCGCCGAUAGAGGGGCUUGCGUCUGAUUAGCUAGUUGGUGGGGGUAACGGCCUCCCAAGGCCACGAGCAGUAGCUGGUCUGAGAGGAUGAUCAGCCACACUGGGACUGAGACACGGCCCAGACUCCUACGGGAGGCAGCAGUGAGGAAUUUUUCGCAAUGGGCGCAAGCGACGGAGCAAUGCCGCGUGCAGGAAGAAGGCCUGUGGGUCGUAAACUGCUUUUCUCAGAGAAGAAGUUCUGACGGUAUCUGAGGAAUAAGCACCGGCUAACUCUGUGCCAGCAGCCGCGGUAAUACAGAGGGUGCAAGCGUUGUCCGGAAUGAUUGGGCGUAAAGCGUCUGUAGGUGGCUCGUAAAGUCUAAUGUCAAAUACCAGGGCUCAACCUUGGACCGGCAUUGGAGUACUCACGAGCUUGAGUACGGUAGGGGCAGAGGGAAUUCCAUGUGGAGCGGUGAAAUGCGUAGAGAUAUGGAGGAACACCAGUGGCGAAGGCGCUCUGCUGGGCCGAAACUGACACUGAGAGACGAAAGCUGGGGGAGCGAAUAGGAUUAGAUACCCUAGUAGUCCCAGCCGUAAACUAUGGAGACUAAGUGCUGCCGCAAGCAGUGCUGUAGCUAACGCGUUAAGUCUCCCGCCUGGGGAGUAUGCUCGCAAGAGUGAAACUCAAAGGAAUUGACGGGACCGCACAAGCGGUGGAUUAUGUGGAUUAAUUCGAUACAACGCGAAGAACCUUACCAGGGUUUGACAUGUCAAGAACCUCUCAGAAAUGGGAGGGUGCCCUAACGGACUUGAACACAGGUGGUGCAUGGCUGUCGUCAGCUCGUGCUGUGAAGUGUAUAGUUAAGUCUCAUAACGAGCGCAACCCUCGUCUUUAGUUGCCAUUUGGUUCUCUAAAGAGACUGCCAGUGUAAGCUGGAGGAAGGUGAGGAUGACGUCAAGUCAGCAUGCCCCUUACAUCCUGGGCUUCACACGUAAUACAAUGGUUGGGACAAUCAGAAGCGACUCGUGAGAGCUAGCGGCUCUGUUAAACCCAACCUCAGUUCGGAUUGUAGGCUGCAACUCGCCUACAUGAAGCCGGAAUCGCUAGUAAUCGCCAGUCAGCUAUAUGGCGGUGAAUACGUUCCCGGGUCUUGUACACACCGCCCGUCACACCAUGGAAGCUGGUUCUGCUCCAAGUCGUUACCCUAACCUUCGGGAGGGGGGCGCCUAAAGCAGGGCUAGUGACUAGGGUGAAGUCGUAACAAGGUAGGGCUACUGGAAGGUGGCCCUGGCUCACCUCCUUC",
	//  "AAAUUGAAGAGUUUGAUCAUGGCUCAGAUUGAACGCUGGCGGCAGGCCUAACACAUGCAAGUCGAACGGUAACAGGAAGAAGCUUGCUUCUUUGCUGACGAGUGGCGGACGGGUGAGUAAUGUCUGGGAAACUGCCUGAUGGAGGGGGAUAACUACUGGAAACGGUAGCUAAUACCGCAUAACGUCGCAAGACCAAAGAGGGGGACCUUCGGGCCUCUUGCCAUCGGAUGUGCCCAGAUGGGAUUAGCUAGUAGGUGGGGUAACGGCUCACCUAGGCGACGAUCCCUAGCUGGUCUGAGAGGAUGACCAGCCACACUGGAACUGAGACACGGUCCAGACUCCUACGGGAGGCAGCAGUGGGGAAUAUUGCACAAUGGGCGCAAGCCUGAUGCAGCCAUGCCGCGUGUAUGAAGAAGGCCUUCGGGUUGUAAAGUACUUUCAGCGGGGAGGAAGGGAGUAAAGUUAAUACCUUUGCUCAUUGACGUUACCCGCAGAAGAAGCACCGGCUAACUCCGUGCCAGCAGCCGCGGUAAUACGGAGGGUGCAAGCGUUAAUCGGAAUUACUGGGCGUAAAGCGCACGCAGGCGGUUUGUUAAGUCAGAUGUGAAAUCCCCGGGCUCAACCUGGGAACUGCAUCUGAUACUGGCAAGCUUGAGUCUCGUAGAGGGGGGUAGAAUUCCAGGUGUAGCGGUGAAAUGCGUAGAGAUCUGGAGGAAUACCGGUGGCGAAGGCGGCCCCCUGGACGAAGACUGACGCUCAGGUGCGAAAGCGUGGGGAGCAAACAGGAUUAGAUACCCUGGUAGUCCACGCCGUAAACGAUGUCGACUUGGAGGUUGUGCCCUUGAGGCGUGGCUUCCGGAGCUAACGCGUUAAGUCGACCGCCUGGGGAGUACGGCCGCAAGGUUAAAACUCAAAUGAAUUGACGGGGGCCCGCACAAGCGGUGGAGCAUGUGGUUUAAUUCGAUGCAACGCGAAGAACCUUACCUGGUCUUGACAUCCACGGAAGUUUUCAGAGAUGAGAAUGUGCCUUCGGGAACCGUGAGACAGGUGCUGCAUGGCUGUCGUCAGCUCGUGUUGUGAAAUGUUGGGUUAAGUCCCGCAACGAGCGCAACCCUUAUCCUUUGUUGCCAGCGGUCCGGCCGGGAACUCAAAGGAGACUGCCAGUGAUAAACUGGAGGAAGGUGGGGAUGACGUCAAGUCAUCAUGGCCCUUACGACCAGGGCUACACACGUGCUACAAUGGCGCAUACAAAGAGAAGCGACCUCGCGAGAGCAAGCGGACCUCAUAAAGUGCGUCGUAGUCCGGAUUGGAGUCUGCAACUCGACUCCAUGAAGUCGGAAUCGCUAGUAAUCGUGGAUCAGAAUGCCACGGUGAAUACGUUCCCGGGCCUUGUACACACCGCCCGUCACACCAUGGGAGUGGGUUGCAAAAGAAGUAGGUAGCUUAACCUUCGGGAGGGCGCUUACCACUUUGUGAUUCAUGACUGGGGUGAAGUCGUAACAAGGUAACCGUAGGGGAACCUGCGGUUGGAUCACCUCCUUA",
	// "CAUCCGGUCGAUCCUGCCGGAGCGCGACGCUCUCCCCAAGGACGAAGCCAUGCAUGCCCGCUCACCCGGGACGCGGCGGACGGCUCAGGACAACGGUUGCACCCCCCGCGGCGGUCCCUGCUAGCCGGACACCGCUGGCAACCCGGCGCCAAGACGUGCGCGCAAGGGCGGGCGCCCGCGGGCGAGCAGCGUGACGCAGCGACGGCCCGCCCGGGCUUCCGGGGCAUCACCCGGUCGGCGCGGUCGCGGCGCGCCGAGGGCCCGACGCCUGGCGGAGAAUCAGGGUUCGACUCCGGAGAGCGGGCCUGCGAGACGGCCCGCACAUCCAAGGACGGCAGCAGGCGCGGAACUUGCCCAAUGCGCGGCGCGCGAGGCAGCGACGGGGAGCGCGCGAGCGAGGCGGGCCCACAGCCCCCGCCGCGGAGCCGAGGGCAAGGUCUGGUGCCAGCAGCCGCGGUAAUUCCAGCUCGGCGAGCGUCGCGCGGCGCUGCUGCAGUUGAAACGCCCGUAGUUGGCCCCCCGCCGCCACGAGGAAACGGGAGCGCUCCAGGCAGGCCCGUUGGACCCGCCGCGUGGGACCGCGCAGCGGGCCGGCGCGCCGCGGCAGCCCCGAGGAGAGCGGGCGGGGGCACCGGUACCGGCCGGGGACGGGUGAAACAGGAUGAUCCCGCCGAGACCGCCGGCCGCGCAGGCGCCUGCCAAGACCGCCUCUGUCAAUCAAGGGCGAAGGCCGGGGGCUAGAAGGCGAUCAGACACCACCGUAUUCCCGGCCGUAAACGGUGCCGCCCCGCGGCCGGCGCGCGCGUCCCGCCGGCCGCCCAGGGAAACCGGGAGGCUCCGGGCUCUGGGGGGAGUAUGGCCGCAAGGCUGAAACUUGAAGGCAUUGACGGAGGGGUACCACCAGACGUGGAGUCUGCGGCUCAAUCUGACUCAACGCGCGCACCUCACCAGGCCCGGACGCGCGGAGGACCGACAGCCGGGCGCGCUUUCGCGAUCGCGCGGGCGGUGGUGCAUGGCCGCUCCCAGCCCGUGGCGCGAGCCGUCUGCUCCAUUGCGACAACGAGCGAGACCCCGGCCGCGGGCGCCGCGGGACGGCCCGCGCGAGCGGGAGGACGGCGGGGCGAUAGCAGGUCUGUGAUGCCCUCAGACGCCCUGGGCCGCACGCGCGCUACACUGGCGGGGCCAGCCGGCGCGCGCGAGGACGCGCGGAGCCCCCGCCGUGGCCGGGACCGCGGGCUGGAACGCCCCCGCGCACCAGGAAUGUCUUGUAGGCGCCCGCCCCCACCGCGCGCCGGACGCGUCCCUGCCCCUUGUACACACCGCCCGUCGCUCCUACCGACUGGGCGCGGCGGCGAGCGCCCCGGACGCGCGAAGGGCCGCGAGCCCCCGCGCCUGGAGGAAGGAGAAGUCGUAACAAGGUAUUCCGUAGGUGAACCUGCGGAUGGAUCCCUC",
	// "GGUUAAGCGACUAAGCGUACACGGUGGAUGCCCUGGCAGUCAGAGGCGAUGAAGGACGUGCUAAUCUGCGAUAAGCGUCGGUAAGGUGAUAUGAACCGUUAUAACCGGCGAUUUCCGAAUGGGGAAACCCAGUGUGUUUCGACACACUAUCAUUAACUGAAUCCAUAGGUUAAUGAGGCGAACCGGGGGAACUGAAACAUCUAAGUACCCCGAGGAAAAGAAAUCAACCGAGAUUCCCCCAGUAGCGGCGAGCGAACGGGGAGCAGCCCAGAGCCUGAAUCAGUGUGUGUGUUAGUGGAAGCGUCUGGAAAGGCGCGCGAUACAGGGUGACAGCCCCGUACACAAAAAUGCACAUGCUGUGAGCUCGAUGAGUAGGGCGGGACACGUGGUAUCCUGUCUGAAUAUGGGGGGACCAUCCUCCAAGGCUAAAUACUCCUGACUGACCGAUAGUGAACCAGUACCGUGAGGGAAAGGCGAAAAGAACCCCGGCGAGGGGAGUGAAAAAGAACCUGAAACCGUGUACGUACAAGCAGUGGGAGCACGCUUAGGCGUGUGACUGCGUACCUUUUGUAUAAUGGGUCAGCGACUUAUAUUCUGUAGCAAGGUUAACCGAAUAGGGGAGCCGAAGGGAAACCGAGUCUUAACUGGGCGUUAAGUUGCAGGGUAUAGACCCGAAACCCGGUGAUCUAGCCAUGGGCAGGUUGAAGGUUGGGUAACACUAACUGGAGGACCGAACCGACUAAUGUUGAAAAAUUAGCGGAUGACUUGUGGCUGGGGGUGAAAGGCCAAUCAAACCGGGAGAUAGCUGGUUCUCCCCGAAAGCUAUUUAGGUAGCGCCUCGUGAAUUCAUCUCCGGGGGUAGAGCACUGUUUCGGCAAGGGGGUCAUCCCGACUUACCAACCCGAUGCAAACUGCGAAUACCGGAGAAUGUUAUCACGGGAGACACACGGCGGGUGCUAACGUCCGUCGUGAAGAGGGAAACAACCCAGACCGCCAGCUAAGGUCCCAAAGUCAUGGUUAAGUGGGAAACGAUGUGGGAAGGCCCAGACAGCCAGGAUGUUGGCUUAGAAGCAGCCAUCAUUUAAAGAAAGCGUAAUAGCUCACUGGUCGAGUCGGCCUGCGCGGAAGAUGUAACGGGGCUAAACCAUGCACCGAAGCUGCGGCAGCGACGCUUAUGCGUUGUUGGGUAGGGGAGCGUUCUGUAAGCCUGCGAAGGUGUGCUGUGAGGCAUGCUGGAGGUAUCAGAAGUGCGAAUGCUGACAUAAGUAACGAUAAAGCGGGUGAAAAGCCCGCUCGCCGGAAGACCAAGGGUUCCUGUCCAACGUUAAUCGGGGCAGGGUGAGUCGACCCCUAAGGCGAGGCCGAAAGGCGUAGUCGAUGGGAAACAGGUUAAUAUUCCUGUACUUGGUGUUACUGCGAAGGGGGGACGGAGAAGGCUAUGUUGGCCGGGCGACGGUUGUCCCGGUUUAAGCGUGUAGGCUGGUUUUCCAGGCAAAUCCGGAAAAUCAAGGCUGAGGCGUGAUGACGAGGCACUACGGUGCUGAAGCAACAAAUGCCCUGCUUCCAGGAAAAGCCUCUAAGCAUCAGGUAACAUCAAAUCGUACCCCAAACCGACACAGGUGGUCAGGUAGAGAAUACCAAGGCGCUUGAGAGAACUCGGGUGAAGGAACUAGGCAAAAUGGUGCCGUAACUUCGGGAGAAGGCACGCUGAUAUGUAGGUGAGGUCCCUCGCGGAUGGAGCUGAAAUCAGUCGAAGAUACCAGCUGGCUGCAACUGUUUAUUAAAAACACAGCACUGUGCAAACACGAAAGUGGACGUAUACGGUGUGACGCCUGCCCGGUGCCGGAAGGUUAAUUGAUGGGGUUAGCGCAAGCGAAGCUCUUGAUCGAAGCCCCGGUAAACGGCGGCCGUAACUAUAACGGUCCUAAGGUAGCGAAAUUCCUUGUCGGGUAAGUUCCGACCUGCACGAAUGGCGUAAUGAUGGCCAGGCUGUCUCCACCCGAGACUCAGUGAAAUUGAACUCGCUGUGAAGAUGCAGUGUACCCGCGGCAAGACGGAAAGACCCCGUGAACCUUUACUAUAGCUUGACACUGAACAUUGAGCCUUGAUGUGUAGGAUAGGUGGGAGGCUUUGAAGUGUGGACGCCAGUCUGCAUGGAGCCGACCUUGAAAUACCACCCUUUAAUGUUUGAUGUUCUAACGUUGACCCGUAAUCCGGGUUGCGGACAGUGUCUGGUGGGUAGUUUGACUGGGGCGGUCUCCUCCUAAAGAGUAACGGAGGAGCACGAAGGUUGGCUAAUCCUGGUCGGACAUCAGGAGGUUAGUGCAAUGGCAUAAGCCAGCUUGACUGCGAGCGUGACGGCGCGAGCAGGUGCGAAAGCAGGUCAUAGUGAUCCGGUGGUUCUGAAUGGAAGGGCCAUCGCUCAACGGAUAAAAGGUACUCCGGGGAUAACAGGCUGAUACCGCCCAAGAGUUCAUAUCGACGGCGGUGUUUGGCACCUCGAUGUCGGCUCAUCACAUCCUGGGGCUGAAGUAGGUCCCAAGGGUAUGGCUGUUCGCCAUUUAAAGUGGUACGCGAGCUGGGUUUAGAACGUCGUGAGACAGUUCGGUCCCUAUCUGCCGUGGGCGCUGGAGAACUGAGGGGGGCUGCUCCUAGUACGAGAGGACCGGAGUGGACGCAUCACUGGUGUUCGGGUUGUCAUGCCAAUGGCACUGCCCGGUAGCUAAAUGCGGAAGAGAUAAGUGCUGAAAGCAUCUAAGCACGAAACUUGCCCCGAGAUGAGUUCUCCCUGACCCUUUAAGGGUCCUGAAGGAACGUUGAAGACGACGACGUUGAUAGGCCGGGUGUGUAAGCGCAGCGAUGCGUUGAGCUAACCGGUACUAAUGAACCGUGAGGCUUAACCUU",
	// "UAAGGCAGUGGUAGCGCUGAAGAAUGUUCGUGCAAUUGUCGUUAUUCAUUAUAAAAGGGCGGGUUUUAAAGGAUAUUUUAAAAUUUAAAACAAGCUUUUAAGAGCAGAUGGCGGAUGCCUUGCCAAAGAGAGGCGAUGAAGGACGUACUAGACUGCGAUAAGCUAUGCGGAGCUGUCAAGGAGCUUUGAUGCGUAGAUGUCCGAAUGGGGCAACCCAACUAAUAGAGAUAUUAGUUACUCUAACAGAGAGCGAACCUAGUGAAGUGAAACAUCUCAGUAACUAGAGGAAAAGAAAUCAACGAGAUUCCCUAAGUAGUGGCGAGCGAACGGGGAAAAGGGCAAACCGAGUGCUUGCAUUCGGGGUUGAGGACUGCAACAUCCAAGAGAACGCUUUAGCAGAGUUACCUGGAAAGGUAAGCCAUAGAAAGUGAUAGCCUUGUAUGCGACAAGGCGUUUUUAGGUAGCAGUAUCCAGAGUAGGCCAGGACACGAGGAAUCCAGGUUGAAGCCGGGGAGACCACUCUCCAACUCUAAAUACUACUCUUUGAGCGAUAGCGAACAAGUACCGUGAGGGAAAGGUGAAAAGAACCGCAGUGAGCGGAGUGAAAUAGAACCUGAAACCAUCUGCUUACAAUCAUUCAGAGCCCUAUGAUUUAUCAGGGUGAUGGACUGCCUUUUGCAUAAUGAUCCUGCGAGUUGUGGUAUCUGGCAAGGUUAAGCGAAUGCGAAGCCGUAGCGAAACGAGUUCUUAAUAGGGCGAACAAGUCAGAUGCUGCAGACCCGAAGCUAAGUGAUCUAUCCAUGGCCAAGUUGAAACGCGUGUAAUAGCGCGUGGAGGACUGAACUCCUACCCAUUGAAACGGGUUGGGAUGAGCUGUGGAUAGGGGUGAAAGGCCAAACAAACUUAGUGAUAGCUGGUUCUCUUCGAAAUAUAUUUAGGUAUAGCCUCAAGUGAUAAUAAAAGGGGGUAGAGCGCUGAUUGGGCUAGGGCUGCUCGCCGCGGUACCAAACCCUAUCAAACUUCGAAUACCUUUUAUCGUAUCUUGGGAGUCAGGCGGUGGGUGAUAAAAUCAAUCGUCAAAAGGGGAACAACCCAGACUACCAAAUAAGGUCCCUAAGUUCUAUUCUGAGUGGAAAAAGAUGUGUGGCUACUAAAACAACCAGGAGGUUGGCUUAGAAGCAGCCAUCCUUUAAAGAAAGCGUAACAGCUCACUGGUCUAGUGGUCAUGCGCUGAAAAUAUAACGGGGCUAAGAUAGACACCGAAUUUGUAGAUUGUGUUAAACACAGUGGUAGAAGAGCGUUCAUACCAGCGUUGAAGGUAUACCGGUAAGGAGUGCUGGAGCGGUAUGAAGUGAGCAUGCAGGAAUGAGUAACGAUAAGAUAUAUGAGAAUUGUAUCCGCCGUAAAUCUAAGGUUUCCUACGCGAUGGUCGUCAUCGUAGGGUUAGUCGGGUCCUAAGCCGAGUCCGAAAGGGGUAGGUGAUGGCAAAUUGGUUAAUAUUCCAAUACCGACUGUGGAGCGUGAUGGGGGGACGCAUAGGGUUAAGCGAGCUAGCUGAUGGAAGCGCUAGUCUAAGGGCGUAGAUUGGAGGGAAGGCAAAUCCACCUCUGUAUUUGAAACCCAAACAGGCUCUUUGAGUCCUUUUAGGACAAAGGGAGAAUCGCUGAUACCGUCGUGCCAAGAAAAGCCUCUAAGCAUAUCCAUAGUCGUCCGUACCGCAAACCGACACAGGUAGAUGAGAUGAGUAUUCUAAGGCGCGUGAAAGAACUCUGGUUAAGGAACUCUGCAAACUAGCACCGUAAGUUCGCGAUAAGGUGUGCCACAGCGAUGUGGUCUCAGCAAAGAGUCCCUCCCGACUGUUUACCAAAAACACAGCACUUUGCCAACUCGUAAGAGGAAGUAUAAGGUGUGACGCCUGCCCGGUGCUCGAAGGUUAAGAGGAUGCGUCAGUCGCAAGAUGAAGCGUUGAAUUGAAGCCCGAGUAAACGGCGGCCGUAACUAUAACGGUCCUAAGGUAGCGAAAUUCCUUGUCGGUUAAAUACCGACCUGCAUGAAUGGCGUAACGAGAUGGGAGCUGUCUCAACCAGAGAUUCAGUGAAAUUGUAGUGGAGGUGAAAAUUCCUCCUACCCGCGGCAAGACGGAAAGACCCCGUGGACCUUUACUACAACUUAGCACUGCUAAUGGGAAUAUCAUGCGCAGGAUAGGUGGGAGGCUUUGAAGUAAGGGCUUUGGCUCUUAUGGAGUCAUCCUUGAGAUACCACCCUUGAUGUUUCUGUUAGCUAACUGGCCUGUGUUAUCCACAGGCAGGACAAUGCUUGGUGGGUAGUUUGACUGGGGCGGUCGCUCCUAAAAAGUAACGGAGGCUUGCAAAGGUUGGCUCAUUGCGGUUGGAAAUCGCAAGUUGAGUGUAAUGGCACAAGCCAGCCUGACUGUAAGACAUACAAGUCAAGCAGAGACGAAAGUCGGUCAUAGUGAUCCGGUGGUUCUGUGUGGAAGGGCCAUCGCUCAAAGGAUAAAAGGUACCCCGGGGAUAACAGGCUGAUCUCCCCCAAGAGCUCACAUCGACGGGGAGGUUUGGCACCUCGAUGUCGGCUCAUCGCAUCCUGGGGCUGGAGCAGGUCCCAAGGGUAUGGCUGUUCGCCAUUUAAAGCGGUACGCGAGCUGGGUUCAGAACGUCGUGAGACAGUUCGGUCCCUAUCUGCCGUGGGCGUAGGAAAGUUGAGGAGAGCUGUCCCUAGUACGAGAGGACCGGGAUGGACGUGUCACUGGUGCACCAGUUGUCUGCCAAGAGCAUCGCUGGGUAGCACACACGGAUGUGAUAACUGCUGAAAGCAUCUAAGCAGGAACCAACUCCAAGAUAAACUUUCCCUGAAGCUCGCACAAAGACUAUGUGCUUGAUAGGGUAGAUGUGUGAGCGCAGUAAUGCGUUUAGCUGACUACUACUAAUAGAGCGUUUGGCUUGUUUUU",
	// "GAUUAAGUUAUUAAGGGCGCACGGUGGAUGCCUUGGCACUAGAAGCCGAUGAAGGACGUUACUAACGACGAUAUGCUUUGGGGAGCUGUAAGUAAGCUUUGAUCCAGAGAUUUCCGAAUGGGGAAACCCAGCAUGAGUUAUGUCAUGUUAUCGAUAUGUGAAUACAUAGCAUAUCAGAAGGCACACCCGGAGAACUGAAACAUCUUAGUACCCGGAGGAAGAGAAAGAAAAUUCGAUUCCCUUAGUAGCGGCGAGCGAAACGGGAAGAGCCCAAACCAACAAGCUUGCUUGUUGGGGUUGUAGGACACUCUAUACGGAGUUACAAAGGACGACAUUAGACGAAUCAUCUGGAAAGAUGAAUCAAAGAAGGUAAUAAUCCUGUAGUCGAAAAUGUUGUCUCUCUUGAGUGGAUCCUGAGUACGACGGAGCACGUGAAAUUCCGUCGGAAUCUGGGAGGACCAUCUCCUAAGGCUAAAUACUCUCUAGUGACCGAUAGUGAACCAGUACCGUGAGGGAAAGGUGAAAAGCACCCCGGAAGGGGAGUGAAAUAGAACCUGAAACCGUGUGCUUACAAGUAGUCAGAGCCCGUUAAUGGGUGAUGGCGUGCCUUUUGUAGAAUGAACCGGCGAGUUACGAUUUGAUGCAAGGUUAAGCAGUAAAUGUGGAGCCGUAGCGAAAGCGAGUCUGAAUAGGGCGUUUAGUAUUUGGUCGUAGACCCGAAACCAGGUGAUCUACCCUUGGUCAGGUUGAAGUUCAGGUAACACUGAAUGGAGGACCGAACCGACUUACGUUGAAAAGUGAGCGGAUGAACUGAGGGUAGCGGAGAAAUUCCAAUCGAACCUGGAGAUAGCUGGUUCUCUCCGAAAUAGCUUUAGGGCUAGCCUCAAGUGAUGAUUAUUGGAGGUAGAGCACUGUUUGGACGAGGGGCCCCUCUCGGGUUACCGAAUUCAGACAAACUCCGAAUGCCAAUUAAUUUAACUUGGGAGUCAGAACAUGGGUGAUAAGGUCCGUGUUCGAAAGGGAAACAGCCCAGACCACCAGCUAAGGUCCCAAAAUAUAUGUUAAGUGGAAAAGGAUGUGGCGUUGCCCAGACAACUAGGAUGUUGGCUUAGAAGCAGCCAUCAUUUAAAGAGUGCGUAAUAGCUCACUAGUCGAGUGACACUGCGCCGAAAAUGUACCGGGGCUAAACAUAUUACCGAAGCUGUGGAUUGUCCUUUGGACAAUGGUAGGAGAGCGUUCUAAGGGCGUUGAAGCAUGAUCGUAAGGACAUGUGGAGCGCUUAGAAGUGAGAAUGCCGGUGUGAGUAGCGAAAGACGGGUGAGAAUCCCGUCCACCGAUUGACUAAGGUUUCCAGAGGAAGGCUCGUCCGCUCUGGGUUAGUCGGGUCCUAAGCUGAGGCCGACAGGCGUAGGCGAUGGAUAACAGGUUGAUAUUCCUGUACCACCUAUAAUCGUUUUAAUCGAUGGGGGGACGCAGUAGGAUAGGCGAAGCGUGCGAUUGGAUUGCACGUCUAAGCAGUAAGGCUGAGUAUUAGGCAAAUCCGGUACUCGUUAAGGCUGAGCUGUGAUGGGGAGAAGACAUUGUGUCUUCGAGUCGUUGAUUUCACACUGCCGAGAAAAGCCUCUAGAUAGAAAAUAGGUGCCCGUACCGCAAACCGACACAGGUAGUCAAGAUGAGAAUUCUAAGGUGAGCGAGCGAACUCUCGUUAAGGAACUCGGCAAAAUGACCCCGUAACUUCGGGAGAAGGGGUGCUCUUUAGGGUUAACGCCCAGAAGAGCCGCAGUGAAUAGGCCCAAGCGACUGUUUAUCAAAAACACAGGUCUCUGCUAAACCGUAAGGUGAUGUAUAGGGGCUGACGCCUGCCCGGUGCUGGAAGGUUAAGAGGAGUGGUUAGCUUCUGCGAAGCUACGAAUCGAAGCCCCAGUAAACGGCGGCCGUAACUAUAACGGUCCUAAGGUAGCGAAAUUCCUUGUCGGGUAAGUUCCGACCCGCACGAAAGGCGUAACGAUUUGGGCACUGUCUCAACGAGAGACUCGGUGAAAUCAUAGUACCUGUGAAGAUGCAGGUUACCCGCGACAGGACGGAAAGACCCCGUGGAGCUUUACUGUAGCCUGAUAUUGAAAUUCGGCACAGCUUGUACAGGAUAGGUAGGAGCCUUUGAAACGUGAGCGCUAGCUUACGUGGAGGCGCUGGUGGGAUACUACCCUAGCUGUGUUGGCUUUCUAACCCGCACCACUUAUCGUGGUGGGAGACAGUGUCAGGCGGGCAGUUUGACUGGGGCGGUCGCCUCCUAAAAGGUAACGGAGGCGCUCAAAGGUUCCCUCAGAAUGGUUGGAAAUCAUUCAUAGAGUGUAAAGGCAUAAGGGAGCUUGACUGCGAGACCUACAAGUCGAGCAGGGUCGAAAGACGGACUUAGUGAUCCGGUGGUUCCGCAUGGAAGGGCCAUCGCUCAACGGAUAAAAGCUACCCCGGGGAUAACAGGCUUAUCUCCCCCAAGAGUUCACAUCGACGGGGAGGUUUGGCACCUCGAUGUCGGCUCAUCGCAUCCUGGGGCUGUAGUCGGUCCCAAGGGUUGGGCUGUUCGCCCAUUAAAGCGGUACGCGAGCUGGGUUCAGAACGUCGUGAGACAGUUCGGUCCCUAUCCGUCGUGGGCGUAGGAAAUUUGAGAGGAGCUGUCCUUAGUACGAGAGGACCGGGAUGGACAUACCUCUGGUGUACCAGUUGUCGUGCCAACGGCAUAGCUGGGUAGCUAUGUGUGGACGGGAUAAGUGCUGAAAGCAUCUAAGCAUGAAGCCCCCCUCAAGAUGAGAUUUCCCAACUUCGGUUAUAAGAUCCCUCAAAGAUGAUGAGGUUAAUAGGUUCGAGGUGGAAGCAUGGUGACAUGUGGAGCUGACGAAUACUAAUCGAUCGAAGACUUAAUCAA",
	// "UCAUAACGUUUGGGUAAUCGCUGCAACGCCAACGUUGUAGAGGAAAGUCCAUGCUCGCACGGCCUGAGAUGGCUGUAGUGUUCGUGCCUAGCCAAUUCAUAAGCUAGGGUAUUCUGGCUGUAAGGCUGGUUUAACGGCAGGGAAAAAACCUAAGUCCUUUCGGAUAUGGUUUGACUACCUUUAAAGUGCCACAGUGACGAAGUCCUUGAAGAAAUGAUAGGAGUGGAACGAGGUAAACCCCACGAGCGAGAAACCCAAAUAAUGGUAGGGGAAUCUUUUCCAAGGAAAUGAACGAUGGGAAAGGACAGGUUGUAUAACCUGUAGAUAGAUGAUUGCCACCGGAGUACGAGGCGUGGGCCGUUUGCAGUACAAAGGAACAGAACAUGGCUUACAGAACGUUAUGAACCA",
	// "GGUUAAGUUAGAAAGGGCGCACGGUGGAUGCCUUGGCACUAGGAGCCGAUGAAGGACGGGACGAACACCGAUAUGCUUCGGGGAGCUGUAAGCAAGCUUUGAUCCGGAGAUUUCCGAAUGGGGAAACCCACCACUCGUAAUGGAGUGGUAUCCAUAUCUGAAUUCAUAGGAUAUGAGAAGGCAGACCCGGGGAACUGAAACAUCUAAGUACCCGGAGAAGAGAAAGCAAAUGCGAUUCCCUGAGUAGCGGCGACGAACACGGGAUCAGCCCAAACCAAGAGGCUUGCCUCUGUGGUUGUAGGACACUCUGUACGGAGUUACAAAAGAACGAGGUAGAUGAAGAGGUCUGGAAAGGGCCCGCCAUAGGAGGUAACAGCCCUGUAGUCAAAACUUCGUUCUCUCCUGAGUGGAUCCUGAGUACGGCGGAACACGUGAAAUUCCGUCGGAAUCCGGGAGGACCAUCUCCCAAGGCUAAAUACUCCCUAGUGACCGAUAGUGAACCAGUACCGUGAGGGAAAGGUGAAAAGCACCCCGGAAGGGGAGUGAAAGAGAUCCUGAAACCGUGUGCCUACAAGUAGUCAGAGCCCGUUAACGGUGAUGGCGUGCCUUUUGUAGAAUGAACCGGCGAGUUACGAUCCCGUGCAAGGUUAAGCAGAAGAUGCGGAGCCGCAGCGAAAGCGAGUCUGAAUAGGGCGCAUGAGUACGUGGUCGUAGACCCGAAACCAGGUGAUCUACCCAUGUCCAGGGUGAAGUUCAGGUAACACUGAAUGGAGGCCCGAACCCACGCACGUUGAAAAGUGCGGGGAUGAGGUGUGGGUAGGGGUGAAAUGCCAAUCGAACCUGGAGAUAGCUGGUUCUCUCCGAAAUAGCUUUAGGGCUAGCCUCAAGGUAAGAGUCUUGGAGGUAGAGCACUGAUUGGACUAGGGGCCCUCACCGGGUUACCGAAUUCAGUCAAACUCCGAAUGCCAAUGACUUAUCCUUGGGAGUCAGACUGCGAGUGAUAAGAUCCGUAGUCGAAAGGGAAACAGCCCAGACCGCCAGCUAAGGUCCCAAAGUAUACGUUAAGUGGAAAAGGAUGUGGAGUUGCUUAGACAACCAGGAUGUUGGCUUAGAAGCAGCCACCAUUUAAAGAGUGCGUAAUAGCUCACUGGUCGAGUGACUCUGCGCCGAAAAUGUACCGGGGCUAAACGUAUCACCGAAGCUGCGGACUGUUCUUCGAACAGUGGUAGGAGAGCGUUCUAAGGGCUGUGAAGCCAGACCGGAAGGACUGGUGGACGGCUUAGAAGUGAGAAUGCCGGUAUGAGUAGCGAAAAGAGGGGUGAGAAUCCCUCCACCGAAUGCCUAAGGGUUCCUGAGGAAGGCUCGUCCGCUCAGGGUUAGUCGGGACCUAAGCCGAGGCCGAAAGGCGUAGGCGAUGGACAACAGGUUGAUAUUCCUGUACCACCUCCUCACCAUUUGAGCAAUGGGGGGUCGCAGGAGGAUAGGGUAAGCGCGGUAUUGGAUAUCCGCGUCCAAGCAGUUAGGCUGGGAAAUAGGCAAAUCCGUUUCCCAUAAGGCUGAGCUGUGAUGGCGAGCGAAAUAUAGUAGCGAAGUUCCUGAUUCCACACUGCCAAGAAAAGCCUCUAGCGAGGUGAGAGGUGCCCGUACCGCAAACCGUCACAGGUAGGCGAGGAGAGAAUCCUAAGGUGAUCGAGAGAACUCUCGUUAAGGAACUCGGCAAAAUGACCCCGUAACUUCGGGAGAAGGGGUGCUCUGUUAGGGUGCAAGCCCGAGAGAGCCGCAGUGAAUAGGCCCAGGCGACUGUUUAGCAAAAACACAGGUCUCUGCGAAGCCGUAAGGCGAAGUAUAGGGGCUGACGCCUGCCCGGUGCUGGAAGGUUAAGAGGAGCGCUUAGCGUAAGCGAAGGUGCGAAUUGAAGCCCCAGUAAACGGCGGCCGUAACUAUAACGGUCCUAAGGUAGCGAAAUUCCUUGUCGGGUAAGUUCCGACCCGCACGAAAGGCGCAACGAUCUGGGCGCUGUCUCAACGAGAGACUCGGUGAAAUUAUAGUACCUGUGAAGAUGCAGGUUACCCGCGACAGGACGGAAAGACCCCGUGGAGCUUUACUGCAGCCUGAUAUUGAAUGUUGGUACAGCUUGUACAGGAUAGGUAGGAGCCUUGGAAACCGGAGCGCCAGCUUCGGUGGAGGCAUCGGUGGGAUACUACCCUGGCUGUAUUGACCUUCUAACCCCCCGCCCUUAUCGGGCGGGGAGACAGUGUCAGGUGGGCAGUUUGACUGGGGCGGUCGCCUCCUAAAAGGUAACGGAGGCGCCCAAAGGUUCCCUCAGAAUGGUUGGAAAUCAUUCGCAGAGUGUAAAGGCACAAGGGAGCUUGACUGCGAGACCUACAAGUCGAGCAGGGACGAAAGUCGGGCUUAGUGAUCCGGUGGUUCCGCAUGGAAGGGCCAUCGCUCAACGGAUAAAAGCUACCCCGGGGAUAACAGGCUUAUCUCCCCCAAGAGCUCCACAUCGACGGGGAGGUUUGGCACCUCGAUGUCGGCUCAUCGCAUCCUGGGGCUGUAGUCGGUCCCAAGGGUUGGGCUGUUCGCCCAUUAAAGCGGUACGCGAGCUGGGUUCAGAACGUCGUGAGACAGUUCGGUCCCUAUCCGUCGCGGGCGCUGGAAAUUUGAGAGGAGCUGUCCUUAGUACGAGAGGACCGGGAUGGACGCACCGCUGGUGUACCAGUUGUUCUGCCAAGGGCAUCGCUGGGUAGCUAUGUGCGGACGGGAUAAGUGCUGAAAGCAUCUAAGCAUGAAGCCCCCCUCAAGAUGAGAUUUCCCAUUCCGCAAGGAAGUAAGAUCCCUGAAAGAUGAUCAGGUUGAUAGGUCUGAGGUGGAAGUGUGGCAACACAUGGAGCUGACAGAUACUAAUCGAUCGAGGACUUAACCAU"
};
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	int numOfIterations = 1;
	int numOfSequences = sizeof(allSequences) /(sizeof(allSequences[0]));
	printf("Number of sequences: %d,\n",numOfSequences);
    
    for(int iterations =0;iterations < numOfIterations;iterations++)
    for (int ss = 0; ss < numOfSequences; ss++) 
	{
        

	
    // allocate memory for the sequence
	// printf(allSequences[ss]);
    char * seq2= allSequences[ss]; //"UUUUCUGAGAAUUUGAUCUUGGUUCAGAUUGAACGCUGGCGGCGUGGAUGAGGCAUGCAAGUCGAACGGAAUAAUGACUUCGGUUGUUAUUUAGUGGCGGAAGGGUUAGUAAUACAUAGAUAAUCUGUCCUCAACUUGGGAAUAACGGUUGGAAACGACCGCUAAUACCGAAUGUGGUAUGUUUAGGCAUCUAAAACAUAUUAAAGAAGGGGAUCUUCGGACCUUUCGGUUGAGGGAGAGUCUAUGGGAUAUCAGCUUGUUGGUGGGGUAAUGGCCUACCAAGGCUUUGACGUCUAGGCGGAUUGAGAGAUUGACCGCCAACACUGGGACUGAGACACUGCCCAGACUUCUACGGAAGGCUGCAGUCGAGAAUCUUUCGCAAUGGACGAAAGUCUGACGAAGCGACGCCGCGUGUGUGAUGAAGGCUCUAGGGUUGUAAAGCACUUUCGCUUGGGAAUAAGAGAGAUUGGCUAAUAUCCAAUCGAUUUGAGCGUACCAGGUAAAGAAGCACCGGCUAACUCCGUGCCAGCAGCUGCGGUAAUACGGAGGGUGCUAGCGUUAAUCGGAUUUAUUGGGCGUAAAGGGCGUGUAGGCGGAAAGGAAAGUUAGAUGUUAAAUCUUGGGGCUCAACCCCAAGCCAGCAUCUAAUACUAUCUUUCUAGAGGGUAGAUGGAGAAAAGGGAAUUCCACGUGUAGCGGUGAAAUGCGUAGAUAUGUGGAAGAACACCAGUGGCGAAGGCGCUUUUCUAAUUUACACCUGACGCUAAGGCGCGAAAGCAAGGGGAGCAAACAGGAUUAGAUACCCUGGUAGUCCUUGCCGUAAACGAUGCAUACUUGAUGUGGAUAGUCUCAACCCUAUCCGUGUCGUAGCUAACGCGUUAAGUAUGCCGCCUGAGGAGUACACUCGCAAGGGUGAAACUCAAAAGAAUUGACGGGGGCCCGCACAAGCAGUGGAGCAUGUGGUUUAAUUCGAUGCAACGCGAAGAACCUUACCUGGGCUUGACAUGUAUUUGACCGCGGCAGAAAUGUCGUUUUCCGCAAGGACAGAUACACAGGUGCUGCAUGGCUGUCGUCAGCUCGUGCCGUGAGGUGUUGGGUUAAGUCCCGCAACGAGCGCAACCCUUAUCGUUAGUUGCCAACACUUAGGGUGGGAACUCUAACGAGACUGCCUGGGUUAACCAGGAGGAAGGCGAGGAUGACGUCAAGUCAGCAUGGCCCUUAUGCCCAGGGCUACACACGUGCUACAAUGGCCAGUACAGAAGGUAGCAAUAUCGUGAGAUGGAGCAAAUCCUCAAAGCUGGCCCCAGUUCGGAUUGUAGUCUGCAACUCGACUACAUGAAGUCGGAAUUGCUAGUAAUGGCGUGUCAGCUAUAACGCCGUGAAUACGUUCCCGGGCCUUGUACACACCGCCCGUCACAUCAUGGGAGUUGGUUUUGCCUUAAGUCGUUGACUCAACCUGCAAAGGAGAGAGGCGCCCAAGGUGAGGCUGAUGACUGGGAUGAAGUCGUAACAAGGUAGCCCUACCGGAAGGUGGGGCUGGAUCACCUCCUUU";//ACCGCUUCCCAUCCGAACAGCG";
    
    int a = strlen(seq2);
    // printf("%d\n",a);
    printf("Length of sequence: %d,\n",a);
	char * seq = (char *)malloc( (a) * sizeof(char));
    char * hold_seq = seq;
    for (int i = 0; i< a;i++){
        * seq++  = * seq2++;
    }
    seq = hold_seq;
	// holds pointer location for the sequence
	// char ** helpSeq = (char **)malloc( (15) * sizeof(char*));
	 
    

    
	
	// int len_help = strlen(seq2);
    // char * helpSeq2 = (char *)malloc( (len_help) * sizeof(char));
    // helpSeq2 = helpSeq[];
    // helpSeq[i] = helpSeq2;

	


	// int a = (int)malloc(sizeof(int));
	

	// allocate memory for len
	int * len = (int *)malloc(sizeof(int));
    // holds pointer location of len
    int * hold_len = len;
    *len = a;
    // Setup the sizes for memory allocation
    int size = sizeof(int);
    int size2 = (*len) * sizeof(char);
    int size3 = (*len) * (*len) * sizeof(int);
    printf("Allocated memory on yhe host\n");

    // creates vector x for holding the "table"
    int * x;
    // allocate memory for x
    x = (int*)malloc( (*len) * (*len) * sizeof(int) );
    

    // holds pointer location for x
    int* hold_x = x;
    // Defaults all values in x to 0
    for(int j = 0;j<(*len)*(*len);j++){
        x[j] =0;
    }
    // setup pointers for device
    int *length;
    char *sequence;
    int *table;
    //Allocate memory on device
    cudaMalloc((void **)&length, size);
    cudaMalloc((void **)&sequence, size2);
    cudaMalloc((void **)&table, size3);
    printf("Allocated memory on the device\n");
    // Copy inputs to device
	cudaMemcpy(length, &a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(sequence, hold_seq, size2, cudaMemcpyHostToDevice);
	cudaMemcpy(table, x, size3, cudaMemcpyHostToDevice);

		// // here we put main code of algorithm:
    int numOfBlocks;
	int numOfThreads;
	int sharedVel = 0;
	int maxThreads=1024;
	numOfThreads= maxThreads;
    int maxT = 20024;
	cudaEventRecord(start);

	
    for (int j =1;j<(*len);j++)
    {
        numOfBlocks = min( (*len)-j, maxT);
		sharedVel = j+3;
		numOfThreads = min(sharedVel,maxThreads);

    	// printf("%d\t%d\t%d\t%d\t\n", numOfBlocks,numOfThreads,sharedVel,j);

        nussinovCuda<<<numOfBlocks,numOfThreads,sharedVel*sizeof(int)>>>(table, sequence,length,j,numOfBlocks,numOfThreads,sharedVel);
		// cudaDeviceSynchronize();
		// cudaStreamSynchronize(0);
    }

	cudaEventRecord(stop);
	cudaEventSynchronize(stop);
	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);

	// printf("Milliseconds: %f\n",milliseconds);
	allSeconds += milliseconds;

	//end of main algorithmic code

	// Bring memory from devide back into the host
	cudaMemcpy(x, table, size3 , cudaMemcpyDeviceToHost);

    // printTable(x,seq,len);

	// writing in file for testing purposes
    FILE * fptr = fopen("/home/hiseric1/gpu/NussinovDM/check2.txt","w");
    writeTable(x,seq,len,fptr);
    fclose(fptr);
    printf("Energy = ");
    printf("%d\n",x[a-1]);
    // for(int k=0;k<=a;k++){
    //     printf("%d\t", x[k]);
    // }
		// // add<<<5,1>>>(table,table+5,table+10);
		// // cudaMemcpy(x, table, size3, cudaMemcpyDeviceToHost);
	// printTable(x,seq,len);
		


	// for error checking

	cudaError_t err = cudaGetLastError();  // add
	if (err != cudaSuccess) printf("CUDA error: %s\n", cudaGetErrorString(err) );
	// cudaProfilerStop();
	// Free up memory on the device
    cudaFree(sequence); cudaFree(length);  cudaFree(x);
    free(hold_len); free(hold_x);

	
	free(hold_seq);
	}
	printf("All seconds: %d", allSeconds);
	return 0;
}