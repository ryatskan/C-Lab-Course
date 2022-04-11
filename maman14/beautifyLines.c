#include "beautifyLines.h"
/*
 * Minimize - transform all tabs to spaces, remove redundant spaces and remove trailing spaces
 * from beginning and end of line.
 * Usage - pre-assembler pass
 */

/* Main function: receive an array of lines. minimize each one.*/
char **getMinimizeLines(char **lines) {
    int i, j=0, amountOfLines = linesLen(lines);
    char *trimmedLine;
    for (i=0;i<amountOfLines;i++) {
        trimmedLine = getMinimizedLine(lines[i]);
        if (strlen(trimmedLine) > 0) {
            lines[j] = trimmedLine;
            j++;
        }
        /*free(lines[i]);*/
    }
    lines[j] = NULL;
    return lines;
}
/* return a minimized line of given line*/
char *getMinimizedLine(char *line) {
    if (strlen(line) > 0) {
        tabToSpaceInLine(line);
        line = trimTrailingSpacesFromLine(line);
        line = removeClusterOfSpaces(line);
    }
    return line;
}
/*
 * trims the beginning and ending of the given line from tabs and spaces
*/
 char *trimTrailingSpacesFromLine(char* line) {
    int iterator;
    char *trimmedLine;
    /*== Trims beginning*/
    while (isChrSpace(*line))
        ++line;
    /*== Trims ending*/

    iterator = strlen(line) - 1;
    while (isChrSpace(line[iterator]) && iterator >= 0)
        iterator--;
    line[++iterator] = '\0';
    /*==*/

    trimmedLine = my_malloc(strlen(line) + 1);
    strncpy(trimmedLine, line, strlen(line) + 1);
    /*free(originalLine);*/
    return trimmedLine;
}


/* transforms all tabs into spaces in given string (line)*/
void tabToSpaceInLine(char *line) {
     int i;
     for (i=0;i<strlen(line);i++) {
         if (line[i] == 9) /* if char is tab*/
             line[i] = ' ';
     }
 }
/* replace every group of two or more (cluster) of spaces
 * with exactly one space.
 */
char *removeClusterOfSpaces(char *line) {
    int i, lenOfLine = strlen(line);
    char* newLine = malloc(lenOfLine + 1);
    char *originalNewLine = newLine;
     strcpy(newLine, line);

    for (i=0;i<lenOfLine;i++) {
        /* iterate over all chars in line.
         * if line is ' ' and so is successor, mark it with '\0'*/
        if (line[i] == line[i+1] && isChrSpace(line[i])) {
            newLine[i] = '\0';
        }
    }
    newLine = removeAllCertainChr(newLine, '\0', lenOfLine);
    free(originalNewLine);
    return newLine;
}
char **deleteMultipleLines(char **lines, int *linesToRemove, int amountOfLinesToRemove) {
     int amountOfLines = linesLen(lines);
     char* currentLine;
     char **newLines = my_malloc(sizeof (char*) *  (amountOfLines + 1));
    int i, linesIterator, newLinesIterator = 0;
    /* Replace all lines to be removed with NULL, and free memory of line*/
    for (i=0;i<amountOfLinesToRemove;i++) {
        currentLine = lines[linesToRemove[i]];
        lines[linesToRemove[i]] = NULL;
        /*free(currentLine);*/
    }
    /* Add all lines that are notCmd NULL (marked notCmd to be removed) to newLines array*/
    for (linesIterator = 0; linesIterator < amountOfLines;linesIterator++) {
        if (lines[linesIterator] != NULL) {
            newLines[newLinesIterator] = lines[linesIterator];
            newLinesIterator++;
        }
    }

    /* Add ending symbol to lines*/
    newLines[newLinesIterator] = NULL;
    /*free(lines);*/
    return newLines;
 }
/* similar to how strlen works on string.
 * NULL is the last line (analogous to "\0"), determine how many lines before it.
 */
 int linesLen(char **lines) {
    int i=0;
    while (!isNull(lines[i])) {
        i++;
    }
    return i;
}