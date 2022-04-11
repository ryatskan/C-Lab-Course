#ifndef ASSEMBLERPROJECT_FILEFUNCTIONS_H
#define ASSEMBLERPROJECT_FILEFUNCTIONS_H

#include "global.h"
#include "utils.h"
#include "preAssemblerRun.h"

char *fileToString(FILE *assemblyFile);

char **fileToLines(FILE *assemblyFile);


void linesToFile(char **lines, char *fileName, char *suffix);

#endif
