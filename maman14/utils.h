#ifndef ASSEMBLERPROJECT_UTILS_H
#define ASSEMBLERPROJECT_UTILS_H

#include "global.h"
#include "commands.h"

bool isInteger(char *string);

bool isAlphanumericStr(char *string);

bool isReservedWord(char *name);

bool isLegalLabelName(char *name);

Command checkIfStringIsCommand(char *str);

Directive checkIfStringIsDirective(char *str);

bool isStringRegister(char *str, Register *regOutput);

int *locateAllChrInFile(char *line, int *amountOfChrOutput, char x);

char *removeAllCertainChr(char *line, char certainChr, int lineLen);

int findLastOccurrenceOfChar(char *str, char x);

void printError(Error error, int whichLine);

char *dec2bin(int num, int binSize);

void binTo2Complement(char *output);

void *my_malloc(int size);

void swapArray(char *arr, int size);

void fillNumIntoArrayInBinary(char *arr, int num, int amountOfCharsToFill);

FILE *openFile(char *fileName, char *suffix, char *mode, char *prefix);

table *findLabelInSymbolTable(char *label, table *symbolTable);

char **splitStringByDelimiter(char *str, char *delimiter, int *amountOfSplits);

int FourBinToDecimal(char *fourBinArray);

void my_free();
#endif
