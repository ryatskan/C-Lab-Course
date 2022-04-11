#ifndef ASSEMBLERPROJECT_FIRSTPASS_H
#define ASSEMBLERPROJECT_FIRSTPASS_H
#include "global.h"
#include "beautifyLines.h"
#include "preAssemblerRun.h"
#include "lineToSentence.h"
#include "directives.h"
#include "commands.h"


Error addSentenceToSymbolTable(Sentence currSentence, table *symbolTablePointer, int value);


Error firstPass(char **lines, dataStructures *a, iteratorCapsule *b,
                Sentence *sentencesOutput);
void addEntryToSymbolTable(table *symbolTable, table newSymbolEntry);
struct table_entry createSymbolEntry(char *key, int value, attribute attribute1, attribute attribute2);
#endif