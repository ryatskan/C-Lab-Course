#ifndef ASSEMBLERPROJECT_PREASSEMBLERRUN_H
#define ASSEMBLERPROJECT_PREASSEMBLERRUN_H

#include "fileFunctions.h"
#include "beautifyLines.h"
#include "global.h"

/*
 * A struct that represent each macro DECLARATION in code
 */
typedef struct {
    char *name;
    char **content;
    int lineOfBeginning;
    int lineOfEnd;
    int amountOfLines;
    int amountOfContent;
} Macro;

char **preAssemblerRun(FILE *assemblyFile);

char *isLineStartOfMacro(char *line);

int isLineEndOfMacro(char *line);

Macro *findAllMacrosInFile(char **lines, int *amountOfMacros);

char **replaceMacroCallsWithCode(char **lines, Macro *givenMacro);

char **replaceAllMacroCalls(char **lines, Macro *macroList, int amountOfMacros);

void addMacroDeclaration(char **lines, Macro *macro);

int linesLen(char **lines);

#endif
