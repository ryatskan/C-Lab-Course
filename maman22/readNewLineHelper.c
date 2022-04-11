
#include "complex.h"
void findRealInputEndAndBeginning(int *endOfInputExcludingSpaces, int *end, int *beginning, char line[maxCharLine]);
char* stringToCommandID(int *outputCommandID, char *line, int endOfCommand);
int findCommandLastDigit(char line[maxCharLine]);
char commandIDs[][maxCommandLen] = {"read_comp","print_comp", "add_comp",
                         "sub_comp", "mult_comp_real", "mult_comp_img",
                         "mult_comp_comp", "abs_comp", "stop"};
char* trimInput(char* input);
char *trimCommand(char* command);
void findRealBeginningOfInput(char line[maxCharLine],int *beginningOfInput, int endOfInput);
#define commandIDLen 9

/* receives a string(line), finds the command, input1/in2/in3*/
char* lineToCommandAndInput(char line[maxCharLine], int *commandID, char input1[maxCharLine], char input2[maxCharLine], char input3[maxCharLine]) {
  int endOfCommand, beginningOfInput1, endOfInput1, endOfInput1ExcludingSpaces, beginningOfInput2, endOfInput2, endOfInput2ExcludingSpaces,
  beginningOfInput3, endOfInput3ExcludingSpaces ,endOfInput3, firstCommaAfterCommand;
  char* error;
  /* ===========READ COMMAND===========*/
  endOfCommand = findCommandLastDigit(line);
  firstCommaAfterCommand = endOfCommand + 1;
  while (!isspace(line[firstCommaAfterCommand]) && line[firstCommaAfterCommand] != 0 && firstCommaAfterCommand < maxCharLine && line[firstCommaAfterCommand] != ',') {
    firstCommaAfterCommand++;
  }
  if (line[firstCommaAfterCommand] == ',') {/*comma after a command (excluding ' ')*/
    return "comma immediately after a command";
  }

  error  = stringToCommandID(commandID, line, endOfCommand); /* endOfCommand not including last number*/

  if (strcmp(error, "") != 0) { /* a stop error or command not found error*/
    return error;
  }
  /*sprintf(commandIDString,"%d", commandID);*/

  /* ===========READ INPUTS===========*/
  beginningOfInput1 = endOfCommand + 1;
  findRealInputEndAndBeginning(&endOfInput1ExcludingSpaces, &endOfInput1 ,&beginningOfInput1, line);

  beginningOfInput2 = endOfInput1 + 1;
  findRealInputEndAndBeginning(&endOfInput2ExcludingSpaces, &endOfInput2, &beginningOfInput2, line);

  beginningOfInput3 = endOfInput2 + 1;
  findRealInputEndAndBeginning(&endOfInput3ExcludingSpaces, &endOfInput3, &beginningOfInput3, line);

  /* ===========CREATE THE 3 INPUT STRINGS FROM BEGIN AND END OF EACH ONE===========*/
  /*input1[endOfInput1 + 1] = 0;*/
  strncpy(input1, line + beginningOfInput1, endOfInput1ExcludingSpaces - beginningOfInput1 + 1);
  /*input2[endOfInput2 + 1] = 0;*/
  strncpy(input2, line + beginningOfInput2, endOfInput2ExcludingSpaces - beginningOfInput2 + 1);
  /*input3[endOfInput3 + 1] = 0;*/
  strncpy(input3, line + beginningOfInput3, endOfInput3ExcludingSpaces - beginningOfInput3 + 1);

/*
  strcpy(input1,trimInput(input1));
  strcpy(input2,trimInput(input2));
  strcpy(input3,trimInput(input3));
*/
  return "";
  }






/* receives the input line, and where to start looking for input*/
/* finds the where the input ends - a comma or '\0'*/
/* Also trims it from beginning to end*/
void findRealInputEndAndBeginning(int *endOfInputExcludingSpaces, int *end, int *beginning, char line[maxCharLine]) {

  int endOfInput = *beginning + 1;
  int lenOfInput = 0;
  while (line[endOfInput] != ',' && line[endOfInput] != '\0')  {
    lenOfInput = lenOfInput +1;
    endOfInput = endOfInput + 1;
  }
  if ((line[endOfInput] == '\0') && *beginning< endOfInput) {
    endOfInput--;
  }
  *end = endOfInput;
  /* IGNORE SPACES IN THE END OF INPUT*/
  *endOfInputExcludingSpaces = endOfInput;
  while ((isspace(line[*endOfInputExcludingSpaces]) || line[*endOfInputExcludingSpaces] == ',') && endOfInput > *beginning) {
    *endOfInputExcludingSpaces = *endOfInputExcludingSpaces - 1;
  }

  while ((line[*beginning] == ',' || isspace(line[*beginning])) && *beginning<*endOfInputExcludingSpaces) {
    *beginning = *beginning + 1;
  }

}

/* finds the end of a lastCommand in a new line, return -1 if not found before a ' ', aka an incorrect lastCommand*/
int findCommandLastDigit(char line[maxCharLine]) {
  int endOfCommand = 0;
  while (isspace(line[endOfCommand]) && line[endOfCommand] != '\0') { /* if start with spaces*/
    endOfCommand++;
  }
  while (!isspace(line[endOfCommand]) && endOfCommand < maxCharLine && line[endOfCommand] != '\0') {
    if (endOfCommand >= maxCommandLen ) {
      return -1;
    }
    endOfCommand++;
    if (endOfCommand != '\0') {
      /* means no input only lastCommand*/
    }
  }
  return endOfCommand - 1 ;
}
/* receives a string(the line). Finds the command in it, matches it with the known commands,
 * return the ID of a lastCommand, or -1 if not a lastCommand*/
char* stringToCommandID(int *outputCommandID, char *line, int endOfCommand) {
  int i;

  char commandOnlyString[maxCommandLen + 1];
  char *commandOnlyStringWithoutSpaces;
  for (i=0;i<=endOfCommand;i++) {
    commandOnlyString[i] = line[i];
  }
  commandOnlyString[endOfCommand + 1] = '\0';
  commandOnlyStringWithoutSpaces = trimCommand(trimCommand(commandOnlyString));
  if (strcmp("stop", commandOnlyStringWithoutSpaces) == 0) {

    while (line[i]!='\0') {
      if (!isspace(line[i])) {
        return "illegal stop, contains extra text";
      }
      i++;
    }
    return "stop";
  }
  for (i = 0; i<commandIDLen;i++) {
    if (strcmp(commandIDs[i],commandOnlyStringWithoutSpaces) == 0) {
      *outputCommandID = i;
      return "";
    }
  }
  return "command not found";

   }


char *trimString(char *str)
{
  char *end;

  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)
    return str;

  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  end[1] = '\0';

  return str;
}
/* removes space and ',' in string
 * return updated string*/
char* trimInput(char input[maxCharLine]) {
  while (*input == ',' || *input == ' ') {
    input = input + 1;
  }
  return trimString(input);
}
/* trim spaces in the beginning of command*/
char *trimCommand(char* command) {
  while (isspace(*command)) {
    command = command + 1;
  }
  return command;
}
