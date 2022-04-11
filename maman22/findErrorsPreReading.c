
#include "complex.h"
char* isLineEmpty();
char* errorsRegardingCommas();
char* errorsRegardingSpacing();
char *line;
void findActualLenOfLine(int *realEndOfString);
int actualLengthOfLine;
char* findErrorPreReading(char givenLine[maxCharLine]) {
  char *error;
  line = givenLine;
  findActualLenOfLine(&actualLengthOfLine);
  error = isLineEmpty();
  if (strcmp(error,"") != 0) {
    return error;
  }
  error = errorsRegardingSpacing();
  if (strcmp(error,"") != 0) {
    return error;
  }
  error = errorsRegardingCommas();
  return error;
}
char* isLineEmpty() {
  if (line[0] == 0) {
    return "Line is Empty";
  }
  return "";
}
char* errorsRegardingCommas() {
  char lastCharExcludingSpacing =' ';
  int i;
  if (line[actualLengthOfLine - 1] == ',') {
    return "line ends with a comma";
  }

  for (i=2;i<maxCharLine;i++) {
    if (line[i] == '\0') {
      actualLengthOfLine = i;
    }
  }
  for (i=2; i<actualLengthOfLine; i++) {
    if (lastCharExcludingSpacing == ',' && line[i] == ',') {
      return "Multiple consecutive commas";

    }
    if (!isspace(line[i])) {
      lastCharExcludingSpacing = line[i];
    }
    if (line[i] == ',' && isspace(lastCharExcludingSpacing)) {
      return "Begins with a comma";
    }

  }
  return "";
}
char* errorsRegardingSpacing() {
  return "";
}
void findActualLenOfLine(int *realEndOfString) { /* finds the actual end of string, excluding \0 and ' ' in the end*/
  int strLenOfLine = (int)strlen(line);
  int i;
  if (strLenOfLine == 0) {
    *realEndOfString = 0;
  }
  for (i=strLenOfLine - 1;i>=0;i++) {
    if (!isspace(line[i])) {
      *realEndOfString = i;
      break;
    }
  }
  *realEndOfString = strLenOfLine;
}