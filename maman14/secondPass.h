#ifndef ASSEMBLERPROJECT_SECONDPASS_H
#define ASSEMBLERPROJECT_SECONDPASS_H


#include "global.h"
#include "firstPass.h"
/* Special base that is defined in the maman, strictly used for printing machine word*/
typedef struct {
    int A;
    int B;
    int C;
    int D;
    int E;
} specialBase;
void initExternTable(dataStructures *a, iteratorCapsule *b);

void secondPass(dataStructures *a, iteratorCapsule *b, char **lines, Sentence *sentenceList, char *fileName);

Error processEntryDirective(Sentence currSentence, table *symbolTable);

Error fillCodeImg(Sentence currSentence, table *symbolTable, machineWordCapsule *wordCapsule);

void writeOutputFiles(char **lines, dataStructures *a, iteratorCapsule *b, char *fileName);

void writeObjectFile(machineWordCapsule *codeImg, machineWord *dataImg, int wordCapsuleCount, int dc, int ic,
                     char *fileName);

void writeEntryFile(table *tabPointer, char *fileName);


void updateExternalTable(externalTableEntry *exTable, machineWordCapsule wordCapsule, int *exTableIterator);

void writeExternalFile(externalTableEntry *exTable, int tableSize, char *fileName);

specialBase machineWordToSpecialBase(machineWord x);

machineWordCapsule *sentenceToAssociatedMachineWord(Sentence x, machineWordCapsule *codeImg, int ic);

void printMachineWord(machineWord word, FILE *stream);

bool areThereEntrySymbols(table *tabPointer);

#endif
