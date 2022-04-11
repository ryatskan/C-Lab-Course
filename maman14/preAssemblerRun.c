#include "preAssemblerRun.h"


/* NOTE : There is no error finding in the pre-assembler, assumes macros in file are correct
 *
 * Functions in charge of running the pre-assembler part of the program.
 * Main mission: remove all macros from file.
 * Here we first transform the input file into an array of lines using "fileFunctions".
 * And then trim the input using "beautifyFiles" for the first pass
 */

/*
 * main function, receives a file, process it and creates a new file without macros.
 */
char ** preAssemblerRun(FILE *assemblyFile) {
    char** lines;
    Macro* macroList;
    int amountOfMacros = 0;
    /*transform the file into an array of lines (char**)*/
    lines = fileToLines(assemblyFile);
    lines = getMinimizeLines(lines);
    /*find all macro declerations in the file (the array of lines)*/
    macroList = findAllMacrosInFile(lines, &amountOfMacros);
    /*find and replace all macro calls in the array of lines with its code*/

    lines = replaceAllMacroCalls(lines, macroList, amountOfMacros);
    return lines;
}

/*
 * create a macro struct for each macro declaration in file. outputs a list of all
 * macro found in lines.
 */
Macro* findAllMacrosInFile(char **lines, int *amountOfMacros) {
    char *nameOfMacro;
    Macro *macroList,*currentMacro;
    int amountOfLines = linesLen(lines);
    int currentLine = 0, insideMacro = 0, amountOfMacro = 0;

    nameOfMacro = my_malloc(MAX_MACRO_NAME_LENGTH);
    macroList = my_malloc(sizeof(Macro) * amountOfLines);
    /* Iterates over each line in file*/
    while (currentLine < amountOfLines) {
        if (insideMacro == 0) {
            nameOfMacro = isLineStartOfMacro(lines[currentLine]);
            if (!isNull(nameOfMacro)) {
                insideMacro = 1;
                /* add a new macro to the MacroList*/
                amountOfMacro++;
                currentMacro = macroList + amountOfMacro - 1;
                currentMacro-> name = nameOfMacro;
                currentMacro->lineOfBeginning = currentLine;
            }
        } else {
            /* currently inside a macro declaration, looks for its end (endm)*/
            if (isLineEndOfMacro(lines[currentLine])) {
                currentMacro->lineOfEnd = currentLine;
                currentMacro->amountOfLines = currentLine - macroList[amountOfMacro -1].lineOfBeginning + 1;
                currentMacro->amountOfContent = macroList[amountOfMacro -1].amountOfLines - 2;
                addMacroDeclaration(lines,currentMacro);
                insideMacro = 0;
            }
        }
        currentLine++;
    }
    *amountOfMacros = amountOfMacro;
    return macroList;
}
/*
 * Iterate over all the macros, and for each one replace all
 * macro calls with its declaration code.
 */
char ** replaceAllMacroCalls(char **lines, Macro *macroList, int amountOfMacros) {
        int amountOfLines = linesLen(lines);
        int macroIterator = 0, *linesToRemove = my_malloc(sizeof(int) *(amountOfLines + 1)), j, linesToRemoveIterator = 0;
        Macro* currentMacro;
        /* Iterate over all macros*/
        for (macroIterator = 0; macroIterator < amountOfMacros; macroIterator++) {
            /* For each macro, add all lines of macro declaration to the to-be-removed-lines*/
            currentMacro = macroList + macroIterator;
            for (j = currentMacro->lineOfBeginning; j <= currentMacro->lineOfEnd; j++) {

                linesToRemove[linesToRemoveIterator] = j;
                linesToRemoveIterator++;
            }
            /* macro declaration will be removed, so change beginning & ending of declaration to junk val*/
            currentMacro-> lineOfBeginning = nullVal;
            currentMacro-> lineOfEnd = nullVal;
        }
    /*delete all lines of macro declaration*/
        lines =deleteMultipleLines(lines, linesToRemove,linesToRemoveIterator );
    /* for each macro, find all its calls in lines, and replace it with code*/
        for (macroIterator = 0; macroIterator < amountOfMacros; macroIterator++) {
            lines = replaceMacroCallsWithCode(lines, macroList + macroIterator);
        }

    return lines;
    }
/*
 * Checks if str is of forumla "macro X", where X would be the macro name.
 * If so, returns the macro name, else return NULL
 */
char* isLineStartOfMacro(char *line) {
    int size = 0;
    char **splitLine = splitStringByDelimiter(line, " ",&size );
    if (size == 2 && areStringsEqual(splitLine[0], "macro")) {
         return splitLine[1];
    } else return NULL;
}
int isLineEndOfMacro(char *line) {
    return (areStringsEqual(line, "endm"));
}
/*
 * receives a macro. replace all macro calls in the array of lines
 * with its code (macro declaration)
 */
char **replaceMacroCallsWithCode(char **lines, Macro *givenMacro) {
    char **updatedLines;
    int amountOfLines = linesLen(lines);
    int i, macroCalls = 0, j, updatedLinesIterator = 0, newSize;
    /* find how many macro calls there are (for a specific macro)*/
    for (i=0;i<amountOfLines;i++) {
        if (strcmp(givenMacro->name, lines[i]) == 0) {
            macroCalls++;
        }
    }
    /* create a new array of lines, add*/
    newSize = ((amountOfLines + 1) + givenMacro->amountOfLines* macroCalls - 2*macroCalls);
    updatedLines = my_malloc(newSize* sizeof(char*));
    i=0;
    while(i<amountOfLines) {

            if (areStringsEqual(givenMacro->name, lines[i])) { /* if found a macro call*/
                /* where there would be the macro call in Lines,
                 * put the macro code (can be multiple lines) in updatedLines instead.*/
                j=0;
                while (j < givenMacro->amountOfContent) {
                    updatedLines[updatedLinesIterator] = (givenMacro->content)[j];
                    updatedLinesIterator++;
                    j++;
                }
                i++;
            } else { /* notCmd found a macro - just copy the line into updatedLines*/
                updatedLines[updatedLinesIterator] = lines[i];
                i++;
                updatedLinesIterator++;
            }
        }
    /*linesToNewFile(updatedLines);*/
    /* Add ending symbol*/
    updatedLines[updatedLinesIterator] = NULL;
    return updatedLines;
}
void addMacroDeclaration(char **lines, Macro *macro) {
    int j= macro->lineOfBeginning + 1, i =0;
    macro->content = my_malloc(macro->amountOfContent* sizeof(char*));
    while (j<macro->lineOfEnd) {
        macro->content[i] = my_malloc(strlen(lines[j])+ 1);
        strcpy(macro->content[i], lines[j]);
        i++;
        j++;
    }
}
