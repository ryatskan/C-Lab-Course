
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef MAMAN22_COMPLEX_H
#define MAMAN22_COMPLEX_H

#define maxCharLine 500
#define maxInputLen 100
#define maxCommandLen 100
#define maxInputLen 100
typedef struct complex {
    double real;
    double img;
}complex;
void read_comp(complex *complexNum, double givenReal, double givenImg);
char *readNewLine(complex variables[6]);
void runCommand(int commandID, complex *complexVar1, complex *complexVar2, double input1, double input2);
char* findErrorPreReading(char givenLine[maxCharLine]);
char* lineToCommandAndInput(char line[maxCharLine], int *commandID, char *input1, char *input2, char *input3);
char* runCommandAndFindErrors(complex *variables,int commandID, char *input1, char *input2, char *input3);
void print_comp(complex  complexNum);
complex add_comp(complex firstComplexNum, complex secondComplexNum);
complex sub_comp(complex firstComplexNum, complex secondComplexNum);
complex mult_comp_real(complex firstComplexNum, double realNum);
complex mult_comp_img(complex firstComplexNum, double imgNum);
complex mult_comp_comp(complex firstComplexNum, complex secondComplexNum);
complex abs_comp(complex complexNum);
#endif
