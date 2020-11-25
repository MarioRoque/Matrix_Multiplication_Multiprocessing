#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <intrin.h>

#define OUTOFMEMORY -1
#define WRONGMATRIXSIZES -2

//Path declarations
const char* MatrixAPath = "./Inputs/matrixA.txt";
const char* MatrixBPath = "./Inputs/matrixB.txt";
const char* MatrixCPath = "./Outputs/matrixC.txt";

const double epsilon = 0.0000000001; // For error on decimal less than ten spaces.

//Structure that aids with storing the results of each execution clock time on each  algorithm to avoid code repetition and improve readability
struct Results {
    int serial[5];
    int paralelo1[5];
    int paralelo2[5];
};

/*
Function: to write matrixC in an external file
Arguments: The Matriz and its size values
Outuput: External file with the MatrixC values that is stored on ./Outputs)
*/
void writeMatrix(double* Matrix, int X, int Y) {
    FILE* f = fopen(MatrixCPath, "wb");
    char buffer[255] = { 0 };

    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            // The precision to be handled in the output is 10 decimal places
            sprintf(buffer, "%.10lf\n", Matrix[x * Y + y]);
            fputs(buffer, f);
        }
    }

    fclose(f);
}


/*
Function: to calculate the average time of the five iterations
Arguments: an array of data (time clocks)
Outuput: the average result
*/

double promedio(int arr[5]) {
    int i;
    double sum = 0;
    double avg = 0.0;
    for (i = 0; i < 5; i++) {
        sum = sum + (double)arr[i];
    }
    avg = sum / 5.f;

    return avg;
}

/*
Function: to write the result table with the time clocks of each method (Seríal, OPENMP and intrinsics), the average time and the % vs serial
Arguments: An structure with the clock time results
Outuput: The table printed in console
*/

void printTable(struct Results results) {
    double serialProm = promedio(results.serial);
    double paralelo1Prom = promedio(results.paralelo1);
    double paralelo2Prom = promedio(results.paralelo2);
    double paralelo1vsSerial = (paralelo1Prom / serialProm) * 100;
    double paralelo2vsSerial = (paralelo2Prom / serialProm) * 100;
    double bestOne;

    printf("\n-----------------------------------------------------------------------------------\n");
    printf("\t\tResultados\n");
    printf("\n-----------------------------------------------------------------------------------\n");
    printf("%16s%16s%16s%16s\n", "Corrida", "Serial", "Paralelo 1 (OpenMP)", "Paralelo 2 (SSE Intrinsics)");
    printf("%16s%16d%16d%16d\n", "1", results.serial[0], results.paralelo1[0], results.paralelo2[0]);
    printf("%16s%16d%16d%16d\n", "2", results.serial[1], results.paralelo1[1], results.paralelo2[1]);
    printf("%16s%16d%16d%16d\n", "3", results.serial[2], results.paralelo1[2], results.paralelo2[2]);
    printf("%16s%16d%16d%16d\n", "4", results.serial[3], results.paralelo1[3], results.paralelo2[3]);
    printf("%16s%16d%16d%16d\n", "5", results.serial[4], results.paralelo1[4], results.paralelo2[4]);
    printf("%16s%16.10g%16.10g%16.10g\n", "Promedios: ", serialProm, paralelo1Prom, paralelo2Prom);
    printf("\n-----------------------------------------------------------------------------------\n");
    printf("%16s%16s%16.3g%16.3g\n", "% vs Serial ", " - ", paralelo1vsSerial, paralelo2vsSerial);
    
    if (paralelo2vsSerial == 100 && paralelo1vsSerial == 100) {
        printf("\n There is no improvement with any implementation vs serial.");
    }

    if (paralelo2vsSerial == paralelo1vsSerial) {
        printf("\nThe best multiprocesing is performed by: both OpenMP and Intrinsics");
    }
    if (paralelo2vsSerial < paralelo1vsSerial) {
        printf("\nThe best multiprocesing is performed by: Intrinsics");
    }
    else {
        printf("\nThe best multiprocesing is performed by: OpenMP");

    }


}

/*
Function: to get the size of the matrix
Arguments: A pointer with the filename
Outuput: The numer of lines in the file minus 1
*/


int getMatrixFileSize(const char* filename) {
    FILE* fp;
    char buff[255];
    int i = 0;

    fp = fopen(filename, "r");

    while (!feof(fp)) {
        i++;
        fscanf(fp, "%s", buff);
    }
    return i - 1;
}

/*
Function: to read the data in an external file and stored in a matrix
Arguments: A pointer with the filename, a pointer to the MAtrix, the sizes of the Matriz and a flag for transposing
Outuput: A Matrix stored with the data of the file
*/

void readMatrix(const char* filename, double* Matrix, int X, int Y, int transposed) {

    FILE* fp;
    char buff[255];
    fp = fopen(filename, "r");  //Openning the file to get MatrixA
    int x, y;

    if (transposed) {
        //Filling the matrix with the values
        for (y = 0; y < Y; y++) {
            for (x = 0; x < X; x++) {
                fscanf(fp, "%s", buff);
                Matrix[x * Y + y] = strtod(buff, NULL);
            }
        }
    }
    else {
        //Filling the matrix with the values
        for (x = 0; x < X; x++) {
            for (y = 0; y < Y; y++) {
                fscanf(fp, "%s", buff);
                Matrix[x * Y + y] = strtod(buff, NULL);
            }
        }
    }
}

/*
Function: to print the Data in the Matrix
Arguments: A pointer to the Matrix and the size of the Matrix
Outuput: Printing the data in the console
*/

void printMatrix(double* Matrix, int X, int Y) {
    int x, y;
    for (x = 0; x < X; x++) {
        for (y = 0; y < Y; y++) {
            printf("[%d , %d] : %.10lf \t", x, y, Matrix[x * Y + y]);
        }
        printf("\n");
    }
}

/*
Function: to verify is the Matrix is null
Arguments: A pointer to the Matriz
Outuput: 0 is the Matrux is NULL, 1 if it is not.
*/


int mallocVerification(double* Matrix) {
    if (Matrix == NULL) {
        return 0;
    }
    return 1;
}

/*
Function: to mutiply the matrix using serial coding
Arguments: A pointer to Matrix A, B and C and the size in X and Y of Matrix A and B
Outuput: The MatrixC with the result of the Multiplication between MatrixA and MatrixB
*/

void matrixSerialMultiplication(double* MatrixA, double* MatrixB, double* MatrixC, int AX, int AY, int BX, int BY) {
    double sum = 0;
    int i, j, k;

    for (i = 0; i < AX; i++) {
        for (j = 0; j < BY; j++) {
            sum = 0;
            for (k = 0; k < BX; k++) {
                //printf("A[%d][%d] * B[%d][%d]\n",i, k, j, k);
                sum += MatrixA[i * AY + k] * MatrixB[j * BX + k]; //This function works only when transpose flag on B is on
            }
            MatrixC[i * BY + j] = sum;
        }
    }
}

/*
Function: to mutiply the matrix using OMP
Arguments: A pointer to Matrix A, B and C and the size in X and Y of Matrix A and B
Outuput: The MatrixC with the result of the Multiplication between MatrixA and MatrixB
*/

void matrixOMPMultiplication(double* MatrixA, double* MatrixB, double* MatrixC, int AX, int AY, int BX, int BY) {
    double sum = 0;
    int i, j, k;
#pragma omp parallel for private(i,j,k)
    for (i = 0; i < AX; i++) {
        for (j = 0; j < BY; j++) {
            sum = 0;
            for (k = 0; k < BX; k++) {
                sum += MatrixA[i * AY + k] * MatrixB[j * BX + k]; //This function works only when transpose flag on B is on , for no transpose swap j and k on MatrixB
            }
            MatrixC[i * BY + j] = sum;
        }
    }
}

/*
Function: to compare the values of two given  Matrices
Arguments: A pointer to MatrixC and to MatrixCC, the size of the Matrices, an array with the name of the Matrix and the numbe rof iteration
Outuput: Return 0 is the matrices are not equal, returns 1 if the matrices are equal on iteration.
*/

int MatrixIsRight(double* MatrixC, double* MatrixCC, int X, int Y, char name[], int iteration) {
    int x;
    int y;
    double delta;
    for (x = 0; x < X; x++) {
        for (y = 0; y < Y; y++) {
            delta = MatrixCC[x * Y + y] - MatrixC[x * Y + y];
            if (delta > epsilon) {
                printf("\t\tCalculated matrix on: %s is not equal \n", name);
                return 0;
            }
        }
    }
    printf("\tCalculated matrix on: %s is equal on iteration %d \n", name, iteration);

    return 1;
}

/*
Function: to mutiply the matrix using Intrinsics
Arguments: A pointer to Matrix A, B and C and the size in X and Y of Matrix A and B
Outuput: The MatrixC with the result of the Multiplication between MatrixA and MatrixB
*/

void matrixIntrinsicsMultiplication(double* MatrixA, double* MatrixB, double* MatrixC, int AX, int AY, int BX, int BY) {
    double sum = 0;
    int i, j, k;
    __m128d intrinsicsC, intrinsicsA, intrinsicsB, temporal;
    for (i = 0; i < AX; i++) {
        for (j = 0; j < BY; j += 4) {
            intrinsicsC = _mm_setzero_pd();
            for (k = 0; k < BX; k++) {
                intrinsicsA = _mm_set1_pd(MatrixA[i * AY + k]);
                intrinsicsB = _mm_load_pd((__m128d*) & MatrixB[j * BX + k]);
                temporal = _mm_mul_pd(intrinsicsA, intrinsicsB);
                intrinsicsC = _mm_add_pd(intrinsicsC, temporal);
            }
            _mm_store_sd(&MatrixC[i * BY + j], intrinsicsC);
        }
    }
}


int main()
{
    int AX, AY, BX, BY; //Variables for size.(AX,AY), * (BX,BY)
    int i;
    clock_t start, end;
    struct Results results;
    int MatrixATotalElements, MatrixBTotalElements;

    printf("Write the Matrix A size on the format (X,Y): ");
    scanf("%d %d", &AX, &AY);

    MatrixATotalElements = getMatrixFileSize(MatrixAPath);

    if (AX * AY != MatrixATotalElements) {
        printf("Matrix size is diferent to data contained on %s, total data should be: %d", MatrixAPath, MatrixATotalElements);
        exit(WRONGMATRIXSIZES);
    }

    printf("Write the Matrix B size on the format (X,Y): ");
    scanf("%d %d", &BX, &BY);
    printf("\n");

    MatrixBTotalElements = getMatrixFileSize(MatrixBPath);


    if (BX * BY != MatrixBTotalElements) {
        printf("Matrix size is diferent to data contained on %s, total data should be: %d", MatrixBPath, MatrixBTotalElements);
        
        exit(WRONGMATRIXSIZES);
    }


    if (AX == BY) {

        //Creating the Matrix for A and B
        double* MatrixA = (double*)_mm_malloc(AX * AY * sizeof(double), 64);
        double* MatrixB = (double*)_mm_malloc(BX * BY * sizeof(double), 64);
        double*  MatrixC = (double*)_mm_malloc(AX * BY * sizeof(double), 64); //Result size row per column
        double* MatrixCC = (double*)_mm_malloc(AX * BY * sizeof(double), 64); //Result size row per column
 
      
        //Allocation verification to avoid running out of memory.
        if (!mallocVerification(MatrixA)) {
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
        readMatrix(MatrixBPath, MatrixB, BX, BY, 1); //##Read Matrix B


        //Serial multiplication
        for (i = 0; i < 5; i++) {
            start = clock();
            matrixSerialMultiplication(MatrixA, MatrixB, MatrixC, AX, AY, BX, BY);
            end = clock();
            results.serial[i] = end - start; // save results
        }
        writeMatrix(MatrixC, AX, BY);

        readMatrix(MatrixCPath, MatrixCC, AX, BY, 0); //##Read Matrix c

        //OMP multiplication

        for (i = 0; i < 5; i++) {
            start = clock();
            matrixOMPMultiplication(MatrixA, MatrixB, MatrixC, AX, AY, BX, BY);
            end = clock();
            //Verify the operation
            MatrixIsRight(MatrixC, MatrixCC, AX, BY, "OpenMP", i + 1);
            results.paralelo1[i] = end - start; // save results
        }
        printf("\n");

        //Intrinsincs multiplication

        for (i = 0; i < 5; i++) {
            start = clock();
            matrixIntrinsicsMultiplication(MatrixA, MatrixB, MatrixC, AX, AY, BX, BY);
            end = clock();
            //Verify the operation
            MatrixIsRight(MatrixC, MatrixCC, AX, BY, "Intrinsics ", i + 1);
            results.paralelo2[i] = end - start;
        }

        printf("\n");


        //Free up  memory
        _mm_free(MatrixA);
        _mm_free(MatrixB);
        _mm_free(MatrixC);
        _mm_free(MatrixCC);

        //Print a table of results
         printTable(results);
    }
    else {
        printf("\t Matrix sizes are not compatible (ARows != BColumns) \n");
    }
}