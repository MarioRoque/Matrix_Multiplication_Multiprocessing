#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define OUTOFMEMORY -1

const char* MatrixAPath = "./Inputs/matrixA.txt";
const char* MatrixBPath = "./Inputs/matrixB.txt";

void readMatrix(const char* filename, double* Matrix, size_t X, size_t Y, int transposed) {
    
    FILE* fp;
    char buff[255];
    fp = fopen(filename, "r");  //Openning the file to get MatrixA

    if (transposed) {
        //Filling the matrix with the values
        for (int y = 0; y < Y; y++) {
            for (int x = 0; x < X; x++) {
                fscanf(fp, "%s", buff);
                Matrix[x*Y+y] = strtod(buff, NULL);
            }
        }
    }
    else {
        //Filling the matrix with the values
        for (int x = 0; x < X; x++) {
            for (int y = 0; y < Y; y++) {
                fscanf(fp, "%s", buff);
                Matrix[x*Y+y] = strtod(buff, NULL);
            }
        }
    }
}

void printMatrix(double* Matrix, size_t X, size_t Y) {

    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {          
            printf("[%d , %d] : %.17g \t", x, y, Matrix[x*Y+y]);
        }
        printf("\n");
    }
}


double obtainMatrixIJValue(double* Matrix,int i, int j, size_t Y){
    return Matrix[i*Y+j];
}

int mallocVerification(double* Matrix) { //https://stackoverflow.com/questions/763159/should-i-bother-detecting-oom-out-of-memory-errors-in-my-c-code
    if (Matrix == NULL) {
        return 0;
    }
    return 1;
}

void matrixSerialMultiplication(double* MatrixA, double* MatrixB, double* MatrixC, size_t AX, size_t AY, size_t BX, size_t BY){
    double sum = 0;
    int i, j, k;
    //EDIT THIS TO GET A BETTER TIME RESULT AND USE TRANSPOSED MATRIX FOR B
   	
       for (i = 0; i < AX; i++){
		for (j = 0; j < BY; j++){
			sum = 0;
			for (k = 0; k < AY; k++) {
				sum += MatrixA[i * AY + k] * MatrixB[k * BX + j];   
			} 
            MatrixC[i * AY + j] = sum;  
		}
	}
}


int main()
{
    size_t AX, AY, BX, BY; //Variables for size.(AX,AY), * (BX,BY)
   
    printf("Write the Matrix A size on the format (X,Y): ");
    scanf("%d %d", &AX, &AY); 

    printf("Write the Matrix B size on the format (X,Y): ");
    scanf("%d %d", &BX, &BY);

    if (AX == BY) {
        //Creating the Matrix for A and B
        double* MatrixA = (double*)malloc(AX * AY * sizeof(double));
        double* MatrixB = (double*)malloc(BX * BY * sizeof(double));
        double* MatrixC = (double*)malloc(AX * BY * sizeof(double)); //Result size row per column

        //Allocation verification to avoid running out of memory.
        if (!mallocVerification(MatrixA)){
            fprintf(stderr, "FIRST OUT OF MEMORY OCURRED WHILE ALOCATING MATRIX: A... \nExiting with code %d", OUTOFMEMORY);
            exit(OUTOFMEMORY);
        }
        if (!mallocVerification(MatrixB)) {
            fprintf(stderr, "FIRST OUT OF MEMORY OCURRED WHILE ALOCATING MATRIX: B... \nExiting with code %d", OUTOFMEMORY);
            exit(OUTOFMEMORY);
        }
        if (!mallocVerification(MatrixC)) {
            fprintf(stderr, "FIRST OUT OF MEMORY OCURRED WHILE ALOCATING MATRIX: C... \nExiting with code %d", OUTOFMEMORY);
            exit(OUTOFMEMORY);
        }

        //Reading the A and B matrix
        readMatrix(MatrixAPath, MatrixA, AX, AY, 0); //##Read Matrix A
        readMatrix(MatrixAPath, MatrixB, BX, BY, 0); //##Read Matrix B

        //Serial multiplication
        matrixSerialMultiplication(MatrixA, MatrixB, MatrixC, AX, AY, BX, BY);

        /*Debug matrix printing*/

        //Printing the matrix
        printf("\n\tMatrix A [%d][%d]\n", AX,AY);
        printMatrix(MatrixA, AX, AY);
        printf("\n\tMatrix B[%d][%d]\n", BX, BY);
        printMatrix(MatrixB, BX, BY);
        
        
        printf("\n\tMatrix C[%d][%d]\n", BX, BY);
        printMatrix(MatrixC, AX, BX);
        /*End of debug matrix printing*/
    }
    else {
        printf("\t Matrix sizes are not compatible (Row != Column) \n");
    }
}