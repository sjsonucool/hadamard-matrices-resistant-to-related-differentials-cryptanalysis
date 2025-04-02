#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

#include "boxes-ref.dat"

//arranging elemwnts to form a 4X4 Hadamard Matrix
unsigned char ArrangeHadamard(int n, unsigned char h[n], unsigned char H[n][n])
{
	int i, j;

	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			H[i][j] = h[i^j];
		}
	}
}

unsigned char ArrangeCircular(int n, unsigned char h[n], unsigned char H[n][n]){
	
	int i, j;
	
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			H[i][j] = h[((i*(n-1))+j)%n];
		}
	}
}

//addition in Galois field
unsigned char gadd(unsigned char a, unsigned char b) {
	return a ^ b;
}

unsigned char gmul(unsigned char a, unsigned char b) {
   /* multiply two elements of GF(2^m)
    * needed for MixColumn and InvMixColumn
    */
	if (a && b) return Alogtable[(Logtable[a] + Logtable[b])%255];
	else return 0;
}

//finding modular inverse of an element
unsigned char modInverse(unsigned char a){

	int i,j;
	unsigned char res = 1;
	for(i=0;i<254;i++){ /*Multiplying a by itself 254 times since, a^254 = a^-1*/
		res = gmul(res,a);
	}
	return res;
}

// Optimized exponentiation for GF(256) elements
unsigned char gpow(unsigned char a, int pow) {
	if (pow == 0) {
		return (a == 0) ? 0 : 1; // Handle 0⁰
	}
	if (a == 0) {
		return 0; // 0^pow = 0 for pow > 0
	}
	// Compute a^pow using log/antilog tables
	int exponent = (Logtable[a] * pow) % 255;
	if (exponent < 0) exponent += 255; // Ensure non-negative
	return Alogtable[exponent];
}


//division in Galois field (a*b^-1 = a/b)
unsigned char gdiv(unsigned char a, unsigned char b) {

	return gmul(a,modInverse(b));
}


void multiplyMatrices(int r1, int c1, int c2, unsigned char first[r1][c1], unsigned char second[c1][c2], unsigned char result[r1][c2]) {

	for(int i = 0; i < r1; i++) {
		for(int j = 0; j < c2; j++) {
		    unsigned char sum = 0;
		    for(int k = 0; k < c1; k++) {
		    	sum = gadd(sum, gmul(first[i][k], second[k][j]));
		    }
		    result[i][j] = sum;
		}
	}
}

// Matrix exponentiation by squaring (optimized)
void expMatrices(int pow, int r, int c, unsigned char mat[r][c], unsigned char result[r][c]) {
	unsigned char Mid[8][8] = {
		{1,0,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0}, {0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0}, {0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,0,1}
	};
	unsigned char temp[r][c]; // Current base matrix (mat^k)
	unsigned char res[r][c];  // Accumulated result
	unsigned char tmp[r][c];  // Temporary storage for multiplication

	// Initialize res = identity matrix
	memcpy(res, Mid, sizeof(Mid[0][0])*r*c);
	// Initialize temp = input matrix
	memcpy(temp, mat, sizeof(mat[0][0])*r*c);

	int current_pow = pow;
	while (current_pow > 0) {
		if (current_pow % 2 == 1) {
			// res = res * temp
			multiplyMatrices(r, c, c, res, temp, tmp);
			memcpy(res, tmp, sizeof(tmp[0][0])*r*c);
		}
		// temp = temp * temp
		multiplyMatrices(r, c, c, temp, temp, tmp);
		memcpy(temp, tmp, sizeof(tmp[0][0])*r*c);
		current_pow /= 2;
	}
	// Copy final result to output
	memcpy(result, res, sizeof(res[0][0])*r*c);
}

void addMatrices(int r, int c, unsigned char first[r][c], unsigned char second[r][c], unsigned char result[r][c]){
	for(int i = 0; i < r; i++)
		for(int j = 0; j < c; j++)
			result[i][j] = gadd(first[i][j],second[i][j]);
}

int main(int argc, char *argv[]) {
	int elem, i, j, m, n;
	int powers[255], powerPerm[255];
	char filename1[100] = {"noRelfile"};
	char filename2[100] = {".txt"};
	strcat(filename1,argv[1]);
	strcat(filename1,filename2);
	FILE *fp1;
	fp1 = fopen(filename1, "r");
	
	char py_filename[100];
	strcpy(py_filename, filename1);
	strcat(py_filename, "_binmatrices.py");
	FILE *py_file = fopen(py_filename, "w");
	fprintf(py_file, "# Generated binary matrices\nmatrices = [\n");
	int first_matrix = 1;  // Track first matrix for commas
	
	// identity matrix
	unsigned char Mid[8][8] = {
		{1,0,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0}, {0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,0}, {0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,0,1}
	};

	// 8x8 binary multiplication matrix of x ~ 2 
	unsigned char Mult2[8][8] = {
		{0,1,0,0,0,0,0,0},{0,0,1,0,0,0,0,0},
		{0,0,0,1,0,0,0,0},{1,0,0,0,1,0,0,0},
		{1,0,0,0,0,1,0,0},{0,0,0,0,0,0,1,0},
		{1,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0}
	};

	unsigned char h[4], M[4][4], Mult3[8][8], Mbin[8][8], Mat[32][32];

	// create multiplication matrix of x+1 ~ 3
	// since 3 is the generator in GF(256), Mult3 will be exponentated to produce other binary matrices
	addMatrices(8,8,Mult2,Mid,Mult3);
	
	// create a list 'powers' containing values of 3^j from j = 1 to 255 
	for(j = 1; j < 256; j++){
		powers[j-1] = gpow(3,j);
	}
	
	// create a lookup table powerPerm containing list of exponents,
	// such that 3^powerPerm[i-1] for i= 1 to 255 generates elements in order 1,2,...,255
	for(i = 1; i < 256; i++){
		for(j = 0; j < 255; j++){
			if(powers[j] == i)
				powerPerm[i-1] = j+1;
		}
	}
	
	// read 4-element tuples from file 
	while(fscanf(fp1, "%d", &elem) != EOF){
		h[0] = elem;
		fscanf(fp1, "%d", &elem);
		h[1] = elem;
		fscanf(fp1, "%d", &elem);
		h[2] = elem;
		fscanf(fp1, "%d", &elem);
		h[3] = elem;
		
		// create Hadamard matrix M
		ArrangeHadamard(4,h,M);
		
		// initialize corresponding 32x32 binary matrix Mat of M with 0
		for(i = 0; i < 32; i++){
			for(j = 0; j < 32; j++){
				Mat[i][j] = 0;
			}
		}
		
		// convert each element M[i][j] into 8x8 matrix Mbin by performing Mult3^{powerPerm[M[i][j]-1}
		// and copying Mbin to the corresponding block of Mat
		for(i = 0; i < 4; i++){
			for(j = 0; j < 4; j++){
				if (M[i][j] == 0) {
					// Fill Mbin with zeros
					memset(Mbin, 0, sizeof(Mbin));
				} 
				else {
					// Compute exponent only for non-zero elements
					expMatrices(powerPerm[M[i][j]-1],8,8,Mult3,Mbin);
				}
				//printf("\nM[i][j]=%d 3^%d = %d\n",M[i][j],powerPerm[M[i][j]-1],gpow(3,powerPerm[M[i][j]-1]));
				for(m = 0; m < 8; m++){
					for(n = 0; n < 8; n++){
						Mat[m+8*i][n+8*j] = Mbin[m][n];
					}
				}
			}
		}
		
		// Write matrix to Python file
		if (!first_matrix) {
			fprintf(py_file, ",\n");
		} 
		else {
			first_matrix = 0;
		}

		fprintf(py_file, "    [\n");
		for(int i = 0; i < 32; i++) {
			fprintf(py_file, "        [");
			for(int j = 0; j < 32; j++) {
				fprintf(py_file, "%d", Mat[i][j]);
				if(j < 31) fprintf(py_file, ", ");
			}
			fprintf(py_file, "]");
			if(i < 31) fprintf(py_file, ",\n");
			else fprintf(py_file, "\n");
		}
		fprintf(py_file, "    ]");
	}
	fclose(fp1);
	fprintf(py_file, "\n]\n");
	fclose(py_file);
	return 0;
}
				
