#include <stdio.h>
#include <algorithm>

__device__ void add(int *a, int *b, int *c) {
	c[blockIdx.x] = a[blockIdx.x] + b[blockIdx.x];
}

__device__ bool match(char * sequence,int i,int j)
{
	if ( ((sequence[i] == 'A' and sequence[j] == 'U' ) or (sequence[i] == 'G' and sequence[j] == 'C' )) and j-i>3)
		return 1;
	return 0;
}

__device__  void printTableGPU(int * T, char* s, int * len1){
	int len = *len1;
	// cout << "\t";
	// s.resize(T.size(),'$');
	printf("\t");
	printf("\n");
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



__global__ void mykernel(int * table, char * sequence,int * len1) {
	int len = *len1;

	
	for (int j =1;j<len;j++)
	{
		auto j0 =j;
		for(int i=0;i<len - j0;i++)
		{
			if (i<j)
			{
				int m1 = table[(len*i)+(j-1)];
				int m2 = table[(len*(i+1))+j];
				int m3;
				if(match(sequence,i,j) )
					m3 = table[(len*(i+1))+(j-1)] + match(sequence,i,j);
				else
					m3 = 0;
				int m4 = 0;
				for (int k = i+1;k<j;k++)
				{
					if (table[len*i+k] + table[len*(k+1)+j] > m4)
						m4 = table[len*i+k] + table[len*(k+1)+j];
				}
				//cout << m1 << " " << m2 << " " << m3 << " " << m4 << endl;
				// __vmaxu4 ( unsigned int  a, unsigned int  b )
				

				table[len*i+j] = __vmaxu4(m1,__vmaxu4(m2,__vmaxu4(m3,m4)));
				// if (table[len*i+j] !=0)
				// {
				// 	printf ("%d\t%d\t%d\t%d\t%d\n", m1,m2,m3, m4, table[len*i+j]);
				// }
					
			}
			j++;
		}
		j = j0;
	}
	// printTableGPU(table,sequence,len1);
}

void printTable(int * T, char* s, int * len1){
	int len = *len1;
	// cout << "\t";
	// s.resize(T.size(),'$');
	printf("\t");
	printf("\n");
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


int main(void) {
	int size = sizeof(int);
	
	char* seq;
	int a = 15;
	int * len = (int *)malloc(sizeof(int*));
	int * hold_len = len;
	
	len = &a;
	int size2 = (*len) * sizeof(char);

	seq = (char *)malloc( (*len) * sizeof(char)); 
	char * hold_seq = seq;

	for(int i =0;i<(*len);i++){
		if (i<(*len)/2){
			seq[i] = 'G';
		}
		else 
		seq[i] = 'C';
	}
	// seq = "GGAAACACCU";
	// printf("%s", seq);

	
	
	int * x;
	x = (int*)malloc( (*len) * (*len) * sizeof(int) );
	int* hold_x = x;

	int size3 = (*len)*(*len);//(sizeof(x))*sizeof(int);



	for(int i = 0;i<(*len)*(*len);i++){
		x[i] =0;
	}


	int *length = nullptr;
	char *sequence;
	int *table;



	cudaMalloc((void **)&length, size);
	cudaMalloc((void **)&sequence, size2);
	cudaMalloc((void **)&table, size3);

	// Copy inputs to device
	cudaMemcpy(length, &a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(sequence, seq, size2, cudaMemcpyHostToDevice);
	cudaMemcpy(table, x, size3, cudaMemcpyHostToDevice);

	// printTable(x,seq,len);

	//printf("%s", seq);

	mykernel<<<1,1>>>(table, sequence,length);


	cudaMemcpy(x, table, size3, cudaMemcpyDeviceToHost);

	printTable(x,seq,len);

	



	cudaFree(length); cudaFree(x); cudaFree(sequence);
	free(hold_x);  free(hold_seq); free(hold_len);
	return 0;
}

