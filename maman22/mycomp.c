#include "complex.h"

void inputHandler();
void initVariables(complex complexNumbers[6]);

int main() {
  setbuf(stdout, 0); /*for debugging*/
  printf("==~~ Welcome to the Complex Numbers Environment ~~== \n");
  inputHandler();
  return 0;
}
/* init the complex variables and update the global var*/
void initVariables(complex complexNumbers[6]) {
  complex A,B,C,D,E,F;

  read_comp(&A,0,0);
  read_comp(&B,0,0);
  read_comp(&C,0,0);
  read_comp(&D,0,0);
  read_comp(&E,0,0);
  read_comp(&F,0,0);
  complexNumbers[0] = A;
  complexNumbers[1] = B;
  complexNumbers[2] = C;
  complexNumbers[3] = D;
  complexNumbers[4] = E;
  complexNumbers[5] = F;

}
void inputHandler() {
  char *error = "";

  complex complexNumbers[6];

  initVariables(complexNumbers);

  while (strcmp(error, "Program has stopped") != 0 && strcmp(error, "reached EOF without stop, Program terminates") != 0) {
    printf("\nEnter a command!\n");
    error = (readNewLine(complexNumbers));
    if (strcmp(error,"") != 0) {/* if found error/stop command */
      printf("%s\n", error);
    }
  }
}
/* reads new line from stdin, and run the command/return error*/
char *readNewLine(complex *variables) {
  char input1[maxInputLen] = "",input2[maxInputLen] = "",input3[maxInputLen] = "";
  char line[maxCharLine] = "";
  char *error;
  int commandID;
  if (fgets(line, maxCharLine, stdin) == NULL) { /* reached EOF*/
    return "reached EOF without stop, Program terminates"; /* means an error handling EOF case*/
  }
  if (line[(strlen(line) - 1)] =='\n') {/* remove \n if there is*/
    line[(strlen(line) -1)] = '\0';
  }

  printf("%s\n", line);
/* 3 times checking errors- before, during and after reading and splitting the string into command/input1/in2/in3*/
  error = findErrorPreReading(line);
  if (strcmp(error,"") != 0) { /* if found error*/
    return error;
  }

  error = lineToCommandAndInput(line, &commandID, input1, input2, input3); /* takes a line, and transform it into a command and input
 * (and find errors along the way)*/
  if (strcmp(error,"") != 0) {
    if (strcmp(error,"stop") == 0) {
      return "Program has stopped";
    }
    return error;
  }
  error = runCommandAndFindErrors(variables, commandID, input1, input2, input3); /* checks if inputs are fitting to command, and run the command*/
  if (strcmp(error,"") != 0) {
    printf("%s\n",error);
    return "";
  }
  return "";
}

/* Run command using matching commandID and inputs*/
void runCommand(int commandID, complex *complexVar1, complex *complexVar2, double input1, double input2) {
  complex result;
  switch(commandID) {
    case 0:
      read_comp(complexVar1,input1,input2);
      break;
    case 1:
      print_comp(*complexVar1);
      break;
    case 2:
      result = add_comp(*complexVar1, *complexVar2);
      print_comp(result);
      break;
    case 3:
      result = sub_comp(*complexVar1, *complexVar2);
      print_comp(result);
      break;
    case 4:
      result = mult_comp_real(*complexVar1, input1);
      print_comp(result);
      break;
    case 5:
      result = mult_comp_img(*complexVar1, input1);
      print_comp(result);
      break;
    case 6:
      result = mult_comp_comp(*complexVar1, *complexVar2);
      print_comp(result);
      break;
    case 7:
      result = abs_comp(*complexVar1);
      print_comp(result);
      break;
  }
}

