#include "utils.h"
/*
 * All the utilities functions that are for project-wide usage.
 */

/* ==== Section 1/5: STRING SIMPLE MANIPULATIONS =====*/
/*
 * Create a new line that doesnt contain a specific char.
 */
char *removeAllCertainChr(char *line, char certainChr, int lineLen) {
    int i = 0;
    int newLineIterator = 0;
    char *newLine = my_malloc(lineLen + 1);
    while (i < lineLen) {
        if (line[i] != certainChr) { /* ignore the specific char*/
            newLine[newLineIterator++] = line[i];
        }
        i++;
    }
    newLine[newLineIterator] = '\0';
    /*free(line);*/
    return newLine;
}

/*
 * Returns the last occurrence of char in string or -1 if char isn't in string.
 */
int findLastOccurrenceOfChar(char *str, char x) {
    int i, lastOccurrence = nullVal;
    for (i = 0; i < strlen(str); i++) {
        if (str[i] == x) lastOccurrence = i;
    }
    return lastOccurrence;
}

/*
 * Returns an int array containing all placements of a certain char in line.
 */
int *locateAllChrInFile(char *line, int *amountOfChrOutput, char x) {
    int i;
    int j = 0;
    int *commasPlacement;
    /* find the amount of instances of the char in the given line*/
    for (i = 0; i < strlen(line); i++) {
        if (x == line[i]) {
            (*amountOfChrOutput)++;
        }
    }
    if (*amountOfChrOutput == 0) return NULL;
    commasPlacement = my_malloc((sizeof(int)) * *amountOfChrOutput);
    for (i = 0; i < strlen(line); i++) {
        if (x == line[i]) {
            commasPlacement[j] = i;
            j++;
        }
    }
    return commasPlacement;
}

/*
 * splits a string by a delimiter. returns array of split string
 */
char **splitStringByDelimiter(char *str, char *delimiter, int *amountOfSplits) {
    char *buffer;
    char **arrayOfStrings;
    char *token;
    *amountOfSplits = 0;
    buffer = my_malloc(strlen(str) + 1);
    strcpy(buffer, str);

    arrayOfStrings = my_malloc((strlen(buffer) + 1) * (sizeof(char *)));
    token = strtok(buffer, delimiter);

    while (token != NULL) {
        arrayOfStrings[(*amountOfSplits)] = my_malloc(strlen(token) + 1);
        strcpy(arrayOfStrings[(*amountOfSplits)], token);
        (*amountOfSplits)++;
        token = strtok(NULL, delimiter);
    }
    return arrayOfStrings;
}
/*
 * frees the given array and create a new swapped one. ex "123456"-> "654321".
 */
void swapArray(char *arr, int size) {
    int i;
    char *temp = malloc(size + 1);
    for (i = 0; i < size; i++) {
        temp[i] = arr[size - i - 1];
    }
    strncpy(arr, temp, size);
    free(temp);
}

/* ==== Section 2/5: IS STRING X? =====*/
/*returns true if all chars in string are alphanumeric, false otherwise*/
bool isInteger(char *string) {
    int i = 0;
    if (string[0] == '-' || string[0] == '+') string++; /* if string starts with +/-, it's OK */
    for (; string[i]; i++) { /* Just make sure that everything is a digit until the end */
        if (!isdigit(string[i])) {
            return false;
        }
    }
    return i > 0; /* if i==0 then it was an empty string! */
}

bool isAlphanumericStr(char *string) {
    int i;
    /*check for every char in string if it is non-alphanumeric char if it is function returns true*/
    for (i = 0; string[i]; i++) {
        if (!isalpha(string[i]) && !isdigit(string[i])) return false;
    }
    return true;
}
/* checks if a string is a reserved word - a command, register or directive*/
bool isReservedWord(char *name) {
    Register x;
    if (isNullCommand(checkIfStringIsCommand(name)) && !isStringRegister(name, &x) &&
        isNullDirective(checkIfStringIsDirective(name)))
        return false;
    return true;
}
/*
 * receive a string representing a label.
 * return true if it's a legal one.
 */
bool isLegalLabelName(char *name) {
    bool isFirstChrALetter, isRestOfStringAlphanumeric, isCorrectSize, isntReservedWord;

    if (strlen(name) == 0) return false;
    /* Check length, first char is alpha and all the others are alphanumeric, and not saved word */
    isFirstChrALetter = isalpha(name[0]);
    isRestOfStringAlphanumeric = isAlphanumericStr(name + 1);
    isCorrectSize = strlen(name) < MAX_LABEL_LENGTH;
    isntReservedWord = !isReservedWord(name);

    return isCorrectSize && isFirstChrALetter && isRestOfStringAlphanumeric && isntReservedWord;

}

/*
 * check if a string represent the name of a command. (ex "cmd" = cmd)
 * If not Cmd return a Command that represent null (nullCommand)
 */
Command checkIfStringIsCommand(char *str) {
    int i;
    for (i = 0; i < amountOfCommands; i++) {
        if (areStringsEqual(str, commands[i]))
            return i;

    }
    return nullCommand;
}

/*
 * check if a string represent the name of a directive. (ex ".data" = dataDirective)
 * If not return a directive that represent null (nullDirective)
 */
Directive checkIfStringIsDirective(char *str) {
    int i;
    for (i = 0; i <= amountOfDirectives; i++) {
        if (areStringsEqual(str, directives[i]))
            return i;

    }
    return nullDirective;
}
/*
 * check if a string represent the name of a register. (ex "r1" = R1)
 * If not return a register that represent null (nullRegister)
 */
bool isStringRegister(char *str, Register *regOutput) {
    int i;
    for (i = 0; i <= amountOfRegisters; i++) {
        if (areStringsEqual(str, regs[i])) {
            *regOutput = i;
            return true;
        }
    }
    *regOutput = nullRegister;
    return false;
}

/* ==== Section 3/5: BINARY STUFF =====*/
/*
 * Receives a number. Transforms it into binary/2 complement and copy it into
 * an array.
 */
void fillNumIntoArrayInBinary(char *arr, int num, int amountOfCharsToFill) {

    char *binNum = dec2bin(abs(num), amountOfCharsToFill);
    if (num < 0) {
        swapArray(binNum, amountOfCharsToFill);
        binTo2Complement(binNum);
        swapArray(binNum, amountOfCharsToFill);
    }
    /*swapArray(binNum, 4);
    */strncpy(arr, binNum, amountOfCharsToFill);
}
/*
 * Find the two complement of given (binary) array, and print it into
 * the given array.
 */
void binTo2Complement(char *output) {
    int SIZE = (int) strlen(output), i, carry = 1;
    char *one = malloc(SIZE + 1);
    char *two = malloc(SIZE + 1);
    for (i = 0; i < SIZE; i++) {
        if (output[i] == '0') {
            one[i] = '1';
        } else if (output[i] == '1') {
            one[i] = '0';
        }
    }
    one[SIZE] = '\0';
    for (i = SIZE - 1; i >= 0; i--) {
        if (one[i] == '1' && carry == 1) {
            two[i] = '0';
        } else if (one[i] == '0' && carry == 1) {
            two[i] = '1';
            carry = 0;
        } else {
            two[i] = one[i];
        }
    }
    two[SIZE] = '\0';
    strncpy(output, two, SIZE);
    free(one);
    free(two);
}

/*
 * transform a decimal number into binary, prints the result into a given
 * 16 BIT ARRAY OUTPUT
 */
char *dec2bin(int num, int binSize) {
    char *temp, *temp2;
    int i=0, c, k;
    temp = malloc(18);
    for (c = 15; c >= 0; c--) {
        k = num >> c;

        if (k & 1)
            temp[i++] = '1';
        else
            temp[i++] = '0';
    }
    temp2 = my_malloc(17);
    for (i = 15; i > 15 - binSize; i--) {
        temp2[15 - i] = temp[i];
    }
    free(temp);
    return temp2;
}
/*
 * Receives 4 bits and outputs its decimal value.
 */
int FourBinToDecimal(char *fourBinArray) {
    char temp[4];
    int output;
    strncpy(temp, fourBinArray, 4);
    output = (int) strtol(temp, NULL, 2);
    return output;
}
/* ==== Section 4/5: MEMORY ALLOCATION =====*/
static int indexAllocedStorage = 0;
static void **pAllocedStorage = NULL;
static int total_mem_alloced = 0;

void *
my_malloc(int size) {
    void **temp;
    void *p = malloc(size);
    if (p == NULL) {
        exit(EXIT_FAILURE);
    }

    total_mem_alloced += size;

    temp = realloc(pAllocedStorage, (indexAllocedStorage + 1) * sizeof(void *));
    if (temp == NULL) {
        exit(EXIT_FAILURE);
    }

    pAllocedStorage = temp;
    pAllocedStorage[indexAllocedStorage++] = p;

    return p;
}

void my_free() {
    size_t i;
    for (i = 0; i < indexAllocedStorage; i++) {
        free(pAllocedStorage[i]);
    }
    indexAllocedStorage= 0;
}


/* ==== Section 5/5: MISC =====*/
void printError(Error error, int whichLine) {
    switch (error) {
        case illegalLabelError:
            printf("Line %i: illegal label\n", whichLine);
            break;
        case unknownSentenceTypeError:
            printf("Line %i: Sentence is not one of the 4 known types\n", whichLine);
            break;
        case commaAfterOperationError:
            printf("Line %i: Comma immediately after operation\n", whichLine);
            break;
        case commaInEndOfLineError:
            printf("Line %i: Line ends with a comma\n", whichLine);
            break;
        case twoConsecutiveCommasError:
            printf("Line %i: Two consecutive commas\n", whichLine);
            break;
        case parametersWithoutSeparatingCommaError:
            printf("Line %i: Two parameters without a comma separating them\n", whichLine);
            break;
        case tooLittleOperatorsError:
            printf("Line %i: Command has too little operators\n", whichLine);
            break;
        case tooManyOperatorsError:
            printf("Line %i: Command can have at most 2 operands\n", whichLine);
            break;
        case firstOperatorIncorrectTypeError:
            printf("Line %i: first operator is of incorrect type\n", whichLine);
            break;
        case secondOperatorIncorrectTypeError:
            printf("Line %i: second operator is of incorrect type\n", whichLine);
            break;
        case incorrectAmountOfOperandsError:
            printf("Line %i: incorrect amount of operands\n", whichLine);
            break;
        case externLabelDuplicateError:
            printf("Line %i: label operand for extern directive has been declared as non external\n", whichLine);
            break;
        case tooManyParametersError:
            printf("Line %i: directive has too many parameters\n", whichLine);
            break;
        case missingClosingQuoteError:
            printf("Line %i: closing quote in string directive is not found\n", whichLine);
            break;
        case missingOpeningQuoteError:
            printf("Line %i: opening quote in string directive is not found\n", whichLine);
            break;
        case parameterIsNotIntegerError:
            printf("Line %i: data directive operator is not an integer\n", whichLine);
            break;
        case unrecognizableAddressingType:
            printf("Line %i: unrecognizable addressing type\n", whichLine);
            break;
        case indexAddressingIllegalRegisterError:
            printf("Line %i: index addressing register is not between r10 to r15\n", whichLine);
            break;
        case immediateOperandNotIntegerError:
            printf("Line %i: immediate addressing operand is non-integer\n", whichLine);
            break;
        case indexIllegalLabelError:
            printf("Line %i: index addressing label is illegal\n", whichLine);
            break;
        case tooLittleParametersError:
            printf("Line %i: too little parameters\n", whichLine);
            break;
        case labelIsNotInSymbolTable:
            printf("Line %i: label operand has not been declared in code\n", whichLine);
            break;
        case fileError:
            printf("Error handling file\n");
            break;
        case labelDuplicateError:
            printf("Line %i: label has been declared twice\n", whichLine);
            break;
        default:
            printf("Line %i: Unknown error debugg\n", whichLine);
            break;
    }
}

table *findLabelInSymbolTable(char *label, table *symbolTable) {
    table *currSymbolEntry = symbolTable;
    while ((*currSymbolEntry)->value != nullVal) {
        if (areStringsEqual((*currSymbolEntry)->key, label)) {
            return currSymbolEntry;
        }
        currSymbolEntry = &(*currSymbolEntry)->next;
    }
    return NULL;
}

FILE *openFile(char *fileName, char *suffix, char *mode, char *prefix) {
    char *fullFileName;
    FILE *currentFile;
    fullFileName = my_malloc(strlen(fileName) + strlen(suffix) + strlen(mode) + 10);
    strcpy(fullFileName, prefix);
    strcat(fullFileName, fileName);
    strcat(fullFileName, suffix);
    /*==*/
    currentFile = fopen(fullFileName, mode);
    if (currentFile == NULL) {
        return NULL;
    }
    return currentFile;
}
