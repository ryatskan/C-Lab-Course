#ifndef ASSEMBLERPROJECT_LINETOSENTENCE_H
#define ASSEMBLERPROJECT_LINETOSENTENCE_H

#include "global.h"
#include "utils.h"

Error extractInputParameters(char *parameters, Sentence *sentenceOutput);

Error lineToSentence(char *line, Sentence *sentenceOutput);

void addToDataImg(machineWord *dataImg, int data, int *dc);

int isNoteOrEmptySentence(Sentence *sentenceOutput, char *line);

bool RemoveEndingColon(char *str);

void findAllCommas(char *line, int *amountOfLinesOutput);

char *trimSpacesSurroundingCommas(char *line);

Error checkLineForLabel(char ***splitsPointer, Sentence *sentenceOutput, char **linePointer);

Error checkLineForOperator(char **splits, Sentence *sentenceOutput, char **linePointer);
#endif
