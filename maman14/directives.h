#ifndef ASSEMBLERPROJECT_DIRECTIVES_H
#define ASSEMBLERPROJECT_DIRECTIVES_H
#include "global.h"
#include "utils.h"
#include "firstPass.h"
Error processDirective(Sentence currSentence, table *symbolTable, machineWord *dataImg, int *dc);
#endif