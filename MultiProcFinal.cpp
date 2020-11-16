#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* TODO ( Completed [1/9] ):  
                            This list is not prioritized but it would be nice to follow the order.
    [1]-Need to manage the case when im trying to read more than it currently exist on Matrix.txt.
    [2]-Need to perform serial mutliplication.
    [3]-Need to perform parallel multiplication 1 five times.
    [4]-Need to perform parallel multiplication 2 five times.
    [5]-Need to generate a write to txt function to save matrix C.
    [6]-Need to add time functions to calculate execution time.
    [7]-Need to generate a function compare matrixC.txt with Matrix results on serial processing.
    [8]-Need to add a function that prints a table with the execution time for each run and processing type.
    [9]-Need to add function descriptive comments.
    [10]-Need to add a verification to manage when not possible to grant malloc space. [DONE]
*/

//#define MatrixAPath "./Inputs/matrixA.txt"  //Apparently it doesnt like macros for constants
//#define MatrixBPath "./Inputs/matrixB.txt"
#define OUTOFMEMORY -1

const char* MatrixAPath = "./Inputs/matrixA.txt";
const char* MatrixBPath = "./Inputs/matrixB.txt";

void readMatrix(const char* filename, double* Matrix, size_t X, size_t Y) {
    

    FILE* fp;
    char buff[255];
    fp = fopen(filename, "r");  //Openning the file to get MatrixA

    //Filling the matrix with the values
    for (int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {
            fscanf(fp, "%s", buff);
            *((Matrix + y * Y) + x) = strtod(buff, NULL);
            //printf("[%d , %d] : %.17g \t",x ,y, *((Matrix + y * Y) + x)); //Debug  https://stackoverflow.com/questions/16839658/printf-width-specifier-to-maintain-precision-of-floating-point-value  
        }
    }
}

void printMatrix(double* Matrix, size_t X, size_t Y) {

    for (int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {          
            printf("[%d , %d] : %.17g \t", x, y, *((Matrix + y * Y) + x));
        }
        printf("\n");
    }

}

int mallocVerification(double* Matrix) { //https://stackoverflow.com/questions/763159/should-i-bother-detecting-oom-out-of-memory-errors-in-my-c-code
    if (Matrix == NULL) {
        return 0;
    }
    return 1;
}


int main()
{
    size_t AX, AY, BX, BY; //Variables for size.(AX,AY), * (BX,BY)
   
    printf("Write the Matrix A size on the format (X,Y): ");
    scanf_s("%d %d", &AX, &AY);  //Why does it stop me from using scanf?

    printf("Write the Matrix B size on the format (X,Y): ");
    scanf_s("%d %d", &BX, &BY);

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
        readMatrix(MatrixAPath, MatrixA, AX, AY); //##Read Matrix A
        readMatrix(MatrixBPath, MatrixB, BX, BY); //##Read Matrix A

        /*Debug matrix printing*/

        //Printing the matrix
        printf("\n\tMatrix A [%d][%d]\n", AX,AY);
        printMatrix(MatrixA, AX, AY);
        printf("\n\tMatrix B[%d][%d]\n", BX, BY);
        printMatrix(MatrixB, BX, BY);
        /*End of debug matrix printing*/
    }
    else {
        printf("\t Matrix sizes are not compatible (Row != Column) \n");
    }

}

