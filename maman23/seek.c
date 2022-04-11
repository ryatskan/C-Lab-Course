#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
* note: does not assume infinite arguments - assumes maximum of maxAmountOfFiles
 */
#define maxAmountOfFiles 50
char* inputHandler(int argc, char **argv, int *indexOfChar, char  ***filesSource, int *actualAmountOfFiles);
void findCharInMultipleFiles(int charIndex, int charInFiles[maxAmountOfFiles], char **filesSource, int actualAmountOfFiles);
void outputAndErrorHandling(int charInFiles[maxAmountOfFiles], int actualAmountOfFiles);
void findCharInFile(int charIndex, int* charFound, char* fileSource);
char *errorIDToText(int errorID);
int errorTextToId(char* errorText);
void mainProgram(int argc, char **argv);

int main( int argc, char **argv)  {

  setbuf(stdout, 0);
  mainProgram(argc, argv);
  return 0;
}

void mainProgram(int argc, char **argv) {
  char ***filesSourcePointer = malloc(sizeof(char**));
  int charInFiles[maxAmountOfFiles];
  int actualAmountOfFiles;
  int indexOfChar;
  char *error;

  /* INPUT*/
  error = inputHandler(argc, argv, &indexOfChar, filesSourcePointer, &actualAmountOfFiles);

  if (strcmp(error, "") !=0 ) {
    printf("%s",error);
    return;
  }

  /* Find chars in inputs, and update it in charInFiles array*/
  findCharInMultipleFiles(indexOfChar, charInFiles, *filesSourcePointer , actualAmountOfFiles);
  /* Output handling - printing the char found in each file or the error*/
  outputAndErrorHandling(charInFiles, actualAmountOfFiles);
  free(filesSourcePointer);
}


char* inputHandler(int argc, char **argv, int *indexOfChar, char  ***filesSource, int *actualAmountOfFiles) {
  if (argc < 3) { /* argv[0] is "./seek" so doesnt count*/
    return "Input has less than two arguments\n";
  }
  *indexOfChar = atoi(argv[1]);
  if (*indexOfChar <= 0) { /* meaning conversion failed - first word in input line is Not
 * an integer*/
    return "First argument is not a positive integer\n";
  }
  *filesSource = argv + 2;
  *actualAmountOfFiles = argc - 2;
  return "";
}


/* receives multiple sources (filesSource array), find the char at charIndex-th index
 * and put it in corresponding charInFiles array
 * if couldn't find, put error there instead*/
void findCharInMultipleFiles(int charIndex, int charInFiles[maxAmountOfFiles], char **filesSource, int actualAmountOfFiles) {
 int i;
 int charFound;
 for (i=0;i<actualAmountOfFiles;i++) {
   /* < 0 means error - errorID, > 0 means ASCII*/
   findCharInFile(charIndex, &charFound, filesSource[i]);
   charInFiles[i] = charFound;
 }
}

/* searches for the char (of index charIndex) in given file.
 * might not find it (file too short or doesn't exist).
 * in this case places errorID instead of the char in charFound*/
void findCharInFile(int charIndex, int* charFound, char* fileSource) {
  FILE *theFile;
  int AmountOfCharsInFile;
  theFile = fopen(fileSource, "r");
  if (theFile == NULL) {
    *charFound  = errorTextToId("File not found");
    return;
  }
  /* Find file length*/
  fseek(theFile, 0, SEEK_END); /* seek to end of file*/
  AmountOfCharsInFile = ftell(theFile); /* get current file pointer*/
  if (AmountOfCharsInFile <charIndex) {
    *charFound =  errorTextToId("File not long enough");
    return;
  }
  /* If found no problems find and return the char*/
  fseek(theFile, charIndex - 1, SEEK_SET);
  *charFound = fgetc(theFile);
}
/* Prints the chars found in each file in order. if couldn't find the char
 * print the error*/
void outputAndErrorHandling(int charInFiles[maxAmountOfFiles], int actualAmountOfFiles) {
  int i;
  for (i = 0; i < actualAmountOfFiles; i++) {

    if (charInFiles[i] >= 0) { /* found a char - no error*/
     printf("file %d: %i\n", i + 1, charInFiles[i]);
    }
    else { /* error*/
      char* theError = errorIDToText(charInFiles[i]);
      printf("file %d: %s\n", i+1, theError);
    }
  }
}
char *errorIDToText(int errorID) {
  if (errorID == -1) {
    return "File not found";
  }
  if (errorID == -2) {
    return "File not long enough";
  }
  return "unknown error";
}
int errorTextToId(char* errorText) {
  if (strcmp(errorText,"File not found") ==0 ) {
    return -1;
  }
  if (strcmp(errorText, "File not long enough") == 0) {
    return -2;
  }
  return -333;
}
