#include "complex.h"
char* inputToObjectAndFindErrors(complex *variables, char* input, complex **complexInputArray, double *doubleInputArray, int *complexInputIterator, int *doubleInputIterator, int *inputType);
char* areInputTypesCorrect(int commandID, int receivedtypeOfInput1, int receivedtypeOfInput2, int receivedtypeOfInput3);
char* isCorrectAmountInputs(int commandID, int inputs);



char* runCommandAndFindErrors(complex *variables,int commandID, char *input1, char *input2, char *input3) {
  int input1Type, input2Type,input3Type;
  int complexInputIterator = 0, doubleInputIterator = 0, howManyInputs;
  char *error;
  complex *complexInputArray[3];
  double doubleInputArray[3];

  /*=============== input -> double/complex/return error ===============*/
  error = inputToObjectAndFindErrors(variables, input1, complexInputArray,
                                     doubleInputArray, &complexInputIterator, &doubleInputIterator, &input1Type);
  if (strcmp(error,"") != 0) {
    return error;
  }
  error = inputToObjectAndFindErrors(variables, input2, complexInputArray,
                                     doubleInputArray, &complexInputIterator, &doubleInputIterator, &input2Type);
  if (strcmp(error,"") != 0) {
    return error;
  }
  error = inputToObjectAndFindErrors(variables, input3, complexInputArray,
                                     doubleInputArray, &complexInputIterator, &doubleInputIterator, &input3Type);
  if (strcmp(error,"") != 0) {
    return error;
  }
  /*=============== commandID + inputs(double/complex) -> run command/error ===============*/

   /*error = findErrorInCommand(input1, doubleInput1, doubleInput2, doubleInput3, complexInput1, complexInput2, complexInput3);
   */
   howManyInputs = complexInputIterator + doubleInputIterator;
   error = isCorrectAmountInputs(commandID, howManyInputs);
   if (strcmp(error,"") != 0) {
     return error;
   }
  if (strcmp(error,"") != 0) {
    return error;
  }
   error = areInputTypesCorrect(commandID, input1Type, input2Type, input3Type);
  if (strcmp(error,"") != 0) {
    return error;
  }


   runCommand(commandID, complexInputArray[0], complexInputArray[1], doubleInputArray[0], doubleInputArray[1]);
   return "";
}



/* receives input as string. determines if it is complex(add to complexInput) or double(add to doubleInputArray) or else (return error)
 * "" as return means no error*/
char* inputToObjectAndFindErrors(complex *variables, char* input, complex **complexInputArray, double *doubleInputArray, int *complexInputIterator, int *doubleInputIterator, int *inputType) {

  double num;
  int i;
  char *variableNames[6] = {"A", "B", "C", "D", "E", "F"};
  char *conversionExtras;
  if (strlen(input) == 0) {

    return "";
  }
  for (i=1;i<strlen(input) - 1;i++) {
    if (isspace(input[i])) {
      return "missing a comma";
    }
  }

  num = strtod (input, &conversionExtras);/*str->double*/
  if (strlen(conversionExtras) == 0) { /* means it converted the input successfully*/
    *(doubleInputArray + *doubleInputIterator) = num;
    *doubleInputIterator = *doubleInputIterator + 1;
    *inputType = 2;
    return "";
  }

  for (i = 0; i < 6; i++) {
    if (strcmp(variableNames[i], input) == 0) {/* if input=A,B,C.. aka complex*/
      *(complexInputArray + *complexInputIterator) = &variables[i];
      *complexInputIterator = *complexInputIterator + 1;
      *inputType = 1;
      return "";
    }
  }
    /* not a double or a complex -> meaning there's an error*/
    return "invalid input(s), not a number or legal variable";


}





char* isCorrectAmountInputs(int commandID, int inputs) {
  int desiredInputs = 0;
  switch(commandID) {
    case 1:
      desiredInputs = 1;
      break;
    case 0:
      desiredInputs = 3;
      break;
    case 2:
      desiredInputs = 2;
      break;
    case 3:
      desiredInputs = 2;
      break;
    case 4:
      desiredInputs = 2;
      break;
    case 5:
      desiredInputs = 2;
      break;
    case 6:
      desiredInputs = 2;
      break;
    case 7:
      desiredInputs = 1;
      break;
    case 8:
      desiredInputs = 1;
  }
  if (desiredInputs == inputs) return "";
  if (desiredInputs > inputs) return "Too little inputs for the command";
  return "Too many inputs for the command"; /* inputs > desiredInputs*/

}
char* areInputTypesCorrect(int commandID, int receivedtypeOfInput1, int receivedtypeOfInput2, int receivedtypeOfInput3) {
  int expectedTypeOfInput1 = 0; /* 0 for no input, 1 for complex, 2 for num*/
  int expectedTypeOfInput2 = 0;
  int expectedTypeOfInput3 = 0;
  switch (commandID) {
    case 0:
      expectedTypeOfInput1 = 1;
      expectedTypeOfInput2 = 2;
      expectedTypeOfInput3 = 2;

    case 1:
      expectedTypeOfInput1 = 1;
      break;
    case 2:
      expectedTypeOfInput1 = 1;
      expectedTypeOfInput2 = 1;
      break;
    case 3:
      expectedTypeOfInput1 = 1;
      expectedTypeOfInput2 = 1;
      break;
    case 4:
      expectedTypeOfInput1 = 1;
      expectedTypeOfInput2 = 2;
      break;
    case 5:
      expectedTypeOfInput1 = 1;
      expectedTypeOfInput2 = 2;
      break;
    case 6:
      expectedTypeOfInput1 = 1;
      expectedTypeOfInput2 = 1;
      break;
    case 7:
      expectedTypeOfInput1 = 1;
      break;
    case 8:
      expectedTypeOfInput1 = 1;
  }
  if (expectedTypeOfInput1 != 0) {
    if (expectedTypeOfInput1 > receivedtypeOfInput1) { /** input1 expected num but received a complex*/
      return "Expected a num, but received a complex variable";
    } else {
      if (expectedTypeOfInput1 < receivedtypeOfInput1) {
        return "Expected a complex variable, but received a num";
      }
    }
  }
  if (expectedTypeOfInput2 != 0) {
    if (expectedTypeOfInput2 > receivedtypeOfInput2) { /** input1 expected num but received a complex*/
      return "Expected a num, but received a complex variable";
    } else {
      if (expectedTypeOfInput2 < receivedtypeOfInput2) {
        return "Expected a complex variable, but received a num";
      }
    }
  }
  if (expectedTypeOfInput3 != 0) {
    if (expectedTypeOfInput3 > receivedtypeOfInput3) { /** input1 expected num but received a complex*/
      return "Expected a num, but received a complex variable";
    } else {
      if (expectedTypeOfInput3 < receivedtypeOfInput3) {
        return "Expected a complex variable, but received a num";
      }
    }
  }
  return ""; /* if didnt found a mismatching types*/
}

