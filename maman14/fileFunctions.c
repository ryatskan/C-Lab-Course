#include "fileFunctions.h"
/*
 * Receive a file. parse it into a string and return the string.
 */
char *fileToString(FILE *assemblyFile) {
    char *buffer; int length;
    fseek (assemblyFile, 0, SEEK_END);
    length = ftell (assemblyFile);
    fseek (assemblyFile, 0, SEEK_SET);
    buffer = malloc(length + 1) ;
    if (buffer)
    {
        fread (buffer, 1, length, assemblyFile);
    }
    buffer[length] = '\0';
    return buffer;
}
/*
* Receive a file. transforms it into an array of lines (char **)
 */
char **fileToLines(FILE *assemblyFile) {
    int amountOfLines= 0;
    char *fileString = fileToString(assemblyFile);

    char **lines = splitStringByDelimiter(fileString, "\n",&amountOfLines );
    lines[amountOfLines] = NULL;
    fclose(assemblyFile);
    free(fileString);
    return lines;
}
void linesToFile(char **lines, char *fileName, char *suffix) {
    int i;
    FILE *fileDesc;
    /*concatenate filename & extension, and open the file for writing:*/
    fileDesc = openFile(fileName, suffix, "w", "files/");
    for (i=0;i< linesLen(lines);i++) {
        fprintf(fileDesc, "%s\n", lines[i]);
    }
    fclose(fileDesc);

}

