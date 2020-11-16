#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* TODO ( Completed [0/8] ):  
    -Need to manage the case when im trying to read more than it currently exist on Matrix.txt.
    -Need to perform serial mutliplication.
    -Need to perform parallel multiplication 1 five times.
    -Need to perform parallel multiplication 2 five times.
    -Need to generate a write to txt function to save matrix C.
    -Need to add time functions to calculate execution time.
    -Need to generate a function compare matrixC.txt with Matrix results on serial processing.
    -Need to add function descriptive comments.
*/

//#define MatrixAPath "./Inputs/matrixA.txt"  //Apparently it doesnt like macros for constants
//#define MatrixBPath "./Inputs/matrixB.txt"

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


int main()
{
    int AX, AY, BX, BY; //Variables for size.(AX,AY), * (BX,BY)

    printf("Write the Matrix A size on the format (X,Y): ");
    scanf_s("%d %d", &AX, &AY);  //Why does it stop me from using scanf?

    printf("Write the Matrix B size on the format (X,Y): ");
    scanf_s("%d %d", &BX, &BY);

    if (AX == BY) {
        printf("Can calculate\n");
        //Creating the Matrix for A and B
        double* MatrixA = (double*)malloc(AX * AY * sizeof(double));
        double* MatrixB = (double*)malloc(BX * BY * sizeof(double));

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

