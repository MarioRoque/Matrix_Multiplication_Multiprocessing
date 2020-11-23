#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define OUTOFMEMORY -1
#define WRONGMATRIXSIZES -2

const char* MatrixAPath = "./Inputs/matrixA.txt";
const char* MatrixBPath = "./Inputs/matrixB.txt";
const char* MatrixCPath = "./Outputs/matrixC.txt";

const double epsilon = 0.0000000001; // For error on decimal less than ten spaces.

struct Results {
    int serial[5];
    int paralelo1[5];
    int paralelo2[5];
};


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

void printTable(struct Results results) {
    double serialProm = promedio(results.serial);
    double paralelo1Prom = promedio(results.paralelo1);
    double paralelo2Prom = promedio(results.paralelo2);

    printf("\n-----------------------------------------------------------------------------------\n");
    printf("\t\tResultados\n");
    printf("\n-----------------------------------------------------------------------------------\n");
    printf("%16s%16s%16s%16s\n", "Corrida", "Serial", "Paralelo 1", "Paralelo 2");
    printf("%16s%16d%16d%16d\n", "1", results.serial[0], results.paralelo1[0], results.paralelo2[0]);
    printf("%16s%16d%16d%16d\n", "2", results.serial[1], results.paralelo1[1], results.paralelo2[1]);
    printf("%16s%16d%16d%16d\n", "3", results.serial[2], results.paralelo1[2], results.paralelo2[2]);
    printf("%16s%16d%16d%16d\n", "4", results.serial[3], results.paralelo1[3], results.paralelo2[3]);
    printf("%16s%16d%16d%16d\n", "5", results.serial[4], results.paralelo1[4], results.paralelo2[4]);
    printf("\t%16s \t%.17g \t%.17g \t%.17g\n", "Promedios: ", serialProm, paralelo1Prom, paralelo2Prom);
    printf("\n-----------------------------------------------------------------------------------\n");
    printf("%16s %.17g %.17g %.17g\n", "% vs Serial ", " - ", paralelo1Prom / serialProm, paralelo2Prom / serialProm);
}

///

int getMatrixFileSize(const char* filename) {
    FILE* fp;
    char buff[255];
    int i =0;

    fp = fopen(filename, "r");
    
    while (!feof(fp)) {
        i++;
        fscanf(fp, "%s", buff);
    }
    return i-1;
}


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

void printMatrix(double* Matrix, int X, int Y) {
    int x, y;
    for (x = 0; x < X; x++) {
        for (y = 0; y < Y; y++) {
            printf("[%d , %d] : %.10lf \t", x, y, Matrix[x * Y + y]);
        }
        printf("\n");
    }
}


double obtainMatrixIJValue(double* Matrix, int i, int j, int Y) {
    return Matrix[i * Y + j];
}

int mallocVerification(double* Matrix) { //https://stackoverflow.com/questions/763159/should-i-bother-detecting-oom-out-of-memory-errors-in-my-c-code
    if (Matrix == NULL) {
        return 0;
    }
    return 1;
}

void matrixSerialMultiplication(double* MatrixA, double* MatrixB, double* MatrixC, int AX, int AY, int BX, int BY) {
    double sum = 0;
    int i, j, k;

    for (i = 0; i < AX; i++) {
        for (j = 0; j < BY; j++) {
            sum = 0;
            for (k = 0; k < AY; k++) {
                sum += MatrixA[i * AY + k] * MatrixB[j * BX + k]; //This function works only when transpose flag on B is on
            }
            MatrixC[i * AY + j] = sum;
        }
    }
}

void matrixOMPMultiplication(double* MatrixA, double* MatrixB, double* MatrixC, int AX, int AY, int BX, int BY) {
    double sum = 0;
    int i, j, k;
    //EDIT THIS TO GET A BETTER TIME RESULT AND USE TRANSPOSED MATRIX FOR B
#pragma omp parallel for private(i,j,k)
    for (i = 0; i < AX; i++) {
        for (j = 0; j < BY; j++) {
            sum = 0;
            for (k = 0; k < AY; k++) {
                sum += MatrixA[i * AY + k] * MatrixB[j * BX + k]; //This function works only when transpose flag on B is on , for no transpose swap j and k on MatrixB
            }
            MatrixC[i * AY + j] = sum;
        }
    }
}


int MatrixIsRight(double* MatrixC, double* MatrixCC, int X, int Y, char name[],int iteration) {
    int x; 
    int y;
    double delta;
    for (x = 0; x < X; x++) {
        for (y = 0; y < Y; y++) {
            delta = MatrixCC[x * Y + y] - MatrixC[x * Y + y];
            if (delta > epsilon) {
                //printf("%.17g =? %.17g", MatrixCC[x * Y + y], MatrixC[x * Y + y]);
                printf("\t\tCalculated matrix on: %s is not equal \n", name);
                return 0;
            }
        }
    }
    printf("\tCalculated matrix on: %s is equal on iteration %d \n", name, iteration);

    return 1;
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
        printf("Matrix size is diferent to data contained on %s", MatrixAPath);
        exit(WRONGMATRIXSIZES);
    }

    printf("Write the Matrix B size on the format (X,Y): ");
    scanf("%d %d", &BX, &BY);
    printf("\n");

    MatrixBTotalElements = getMatrixFileSize(MatrixAPath);

  
    if (BX * BY != MatrixBTotalElements) {
        printf("Matrix size is diferent to data contained on %s", MatrixBPath);
        exit(WRONGMATRIXSIZES);
    }


    if (AX == BY) {
        //Creating the Matrix for A and B
        double* MatrixA = (double*)malloc(AX * AY * sizeof(double));
        double* MatrixB = (double*)malloc(BX * BY * sizeof(double));
        double* MatrixC = (double*)malloc(AX * BY * sizeof(double)); //Result size row per column
        double* MatrixCC = (double*)malloc(AX * BY * sizeof(double)); //Result size row per column

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
            //printf("Serial Time: %d \n", end - start);
            results.serial[i] = end - start; // save results
        }
        writeMatrix(MatrixC, AX, BX);

        readMatrix(MatrixCPath, MatrixCC, AX, AY, 0); //##Read Matrix c

        for (i = 0; i < 5; i++) {
            start = clock();
            matrixOMPMultiplication(MatrixA, MatrixB, MatrixC, AX, AY, BX, BY);
            end = clock();
            //Verify the operation
            MatrixIsRight(MatrixC, MatrixCC, AX, BY, "OpenMP", i+1);
            
            //printf("OMP Time: %d \n", end - start);
            results.paralelo1[i] = end - start; // save results
            results.paralelo2[i] = 0;           // TODO: REMOVE THIS
        }
        printf("\n");
        /*Debug matrix printing*/
        
            //Printing the matrix
        
            //printf("\n\tMatrix A [%d][%d]\n", AX,AY);
            //printMatrix(MatrixA, AX, AY);
        
            //printf("\n\tMatrix B[%d][%d]\n", BX, BY);
            //printMatrix(MatrixB, BX, BY);

            //printf("\n\tMatrix C[%d][%d]\n", BX, BY);
            //printMatrix(MatrixC, AX, BX);

            //printMatrix(MatrixC, AX, BY);
            //printf("\n");
            //printMatrix(MatrixCC, AX, BY);
        
        /*End of debug matrix printing*/
        free(MatrixA);
        free(MatrixB);
        free(MatrixC);
        free(MatrixCC);

        //Print a table of results
        printTable(results);
    }
    else {
        printf("\t Matrix sizes are not compatible (Row != Column) \n");
    }
}