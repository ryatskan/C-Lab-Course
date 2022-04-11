#ifndef ASSEMBLERPROJECT_BEAUTIFYLINES_H
#define ASSEMBLERPROJECT_BEAUTIFYLINES_H

#include "beautifyLines.h"
#include "global.h"
#include "preAssemblerRun.h"
#include "utils.h"

#define isChrSpace(x) (x==32)

char **getMinimizeLines(char **lines);

char *removeClusterOfSpaces(char *line);

void tabToSpaceInLine(char *line);

char *getMinimizedLine(char *line);

char *trimTrailingSpacesFromLine(char *line);

char **deleteMultipleLines(char **lines, int *linesToRemove, int amountOfLinesToRemove);

#endif
