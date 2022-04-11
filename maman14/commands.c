#include "commands.h"


/**
 * A lookup table for opcode & funct by command name
 */
static  commandTableElement commandTable[] = {
        {mov,    movOp,  nullFunct, {1,  2,  3,  -1}, {0,  1,  2,  3},  2},
        {cmp,    cmpOp,  nullFunct, {0,  1,  2,  3},  {0,  1,  2,  3},  2},
        {add,    addOp,  addFunct,  {1,  2,  3,  -1}, {0,  1,  2,  3},  2},
        {sub,    subOp,  subFunct,  {1,  2,  3,  -1}, {0,  1,  2,  3},  2},
        {lea,    leaOp,  nullFunct, {1,  2,  3,  -1}, {1,  2,  -1, -1}, 2},
        {clr,    clrOp,  clrFunct,  {1,  2,  3,  -1}, {-1, -1, -1, -1}, 1},
        {notCmd, notOp,  notFunct,  {1,  2,  3,  -1}, {-1, -1, -1, -1}, 1},
        {inc,    incOp,  incFunct,  {1,  2,  3,  -1}, {-1, -1, -1, -1}, 1},
        {dec,    decOp,  decFunct,  {1,  2,  3,  -1}, {-1, -1, -1, -1}, 1},
        {jmp,    jmpOp,  jmpFunct,  {1,  2,  -1, -1}, {-1, -1, -1, -1}, 1},
        {bne,    bneOp,  bneFunct,  {1,  2,  -1, -1}, {-1, -1, -1, -1}, 1},
        {jsr,    jsrOp,  jsrFunct,  {1,  2,  -1, -1}, {-1, -1, -1, -1}, 1},
        {red,    redOp,  nullFunct, {1,  2,  3,  -1}, {-1, -1, -1, -1}, 1},
        {prn,    prnOp,  nullFunct, {0,  1,  2,  3},  {-1, -1, -1, -1}, 1},
        {rts,    rtsOp,  nullFunct, {-1, -1, -1, -1}, {-1, -1, -1, -1}, 0},
        {stop,   stopOp, nullFunct, {-1, -1, -1, -1}, {-1, -1, -1, -1}, 0},
};

/* ===== Section 1/3: Main Section =====*/
/*
 * MAIN PROGRAM: First analyze a given sentence operators,
 * Then update the symbolTable & codeImg.
 */
Error processCommand(Sentence inputSentence, dataStructures *a, iteratorCapsule *b) {
    opcode thisOp;
    funct thisFunct;
    Error hasError;
    operand *operandList;
    machineWordCapsule newCodeWord;
    int i;
    /* Process Operator and Operands*/
    commandToOptAndFunc(inputSentence.commandName, &thisOp, &thisFunct);

    hasError = processOperands(inputSentence, &operandList);
    if (hasError != noError) {
        return hasError;
    }
    /* Add to SymbolTable*/
    if (inputSentence.label != NULL) {
        hasError = addSentenceToSymbolTable(inputSentence, a->symbolTable, *b->ic + 100);
        if (hasError != noError) {
            return hasError;
        }
    }
    /* no error encountered, so add to codeImg */
    newCodeWord = *getWordCapsule(thisOp, thisFunct, operandList, inputSentence.amountOfParameters);
    for (i = 0; i < 6; i++) newCodeWord.x[i].address = *b->ic + i + 100;
    newCodeWord.associatedSentence = inputSentence;
    addToCodeImg(a->codeImg, newCodeWord, b->amountOfCodeCapsules);
    (*b->ic) = (*b->ic) + newCodeWord.machineWordCount;
    (*b->amountOfCodeCapsules)++;
    return noError;
}
/*
 * Receives a sentence. Process its operands (given as strings) into operand type.
 * Then check if the operands fit the operator. (depending on mamans' rules)
*/
Error processOperands(Sentence currSentence, operand **operandListOutput) {
    Error hasError;
    operand *operandList;
    int i, amountOfParameters = currSentence.amountOfParameters;
    /* Init a list of operands*/
    operandList = my_malloc(sizeof(operand)*2);
    operandList[0].addressingType = nullAddress;
    operandList[1].addressingType = nullAddress;
    if (amountOfParameters > 2) {
        return tooManyOperatorsError;
    }
    /* Transform each input parameter into an operand*/
    for (i = 0; i < amountOfParameters; i++) {
        hasError = stringToOperand(currSentence.inputParameters[i], &operandList[i]);
        if (hasError != noError) return hasError;
    }
    /* Validate that the given operands are what the specific Command allows*/
    hasError = checkIfAddressingTypeFitCommand(currSentence.commandName, operandList[0].addressingType,operandList[1].addressingType);
    *operandListOutput = operandList;
    return hasError;
}

/*
 * receives a String. process it into an operand.
 */
Error stringToOperand(char *opString, operand *operand) {
    Error hasError= noError;
    Register reg;
    char *label;
    /* Addressing Type 3*/
    if (isStringRegister(opString, &reg)) {
        operand->addressingType = registerAddress;
        operand->regDirOp.reg = reg;
    }
        /* Addressing Type 0*/
    else if (opString[0] == '#' && strlen(opString) > 1) {
        if (isInteger(opString + 1)) {
            operand->addressingType = immediateAddress;
            operand->imOp.num = atoi(opString + 1);
        }
        else { /* ILLEGAL Addressing Type 0*/
            return immediateOperandNotIntegerError;
        }
    }
        /* Addressing Type 1*/
    else if (isLegalLabelName(opString)) {
        operand->addressingType = directAddress;
        label = my_malloc(strlen(opString) + 1);
        strcpy(label, opString);
        operand->dirOp.label = label;

        /* Addressing Type 2*/
    } else if (isIndexOperand(opString, operand, &hasError)) {
        return hasError;
    } else {
        return unrecognizableAddressingType;
    }
    return 0;
}

/* checks if a string is an index outputOperand (type 2)
 * should be of type - (Legal Label) & [ & legalRegister & ] (ex: HEY[r1])
 * should be of type - (Legal Label) & [ & legalRegister & ] (ex: HEY[r1])*/

bool isIndexOperand(char *opString, operand *outputOperand, Error *hasError) {
    char *possibleLabel;
    char *possibleRegister;
    int openParanethesesLocation = findLastOccurrenceOfChar(opString, '[');
    int closingParanthesesLocation = findLastOccurrenceOfChar(opString, ']');
    Register reg;

    if ((abs(abs(openParanethesesLocation) - abs(closingParanthesesLocation)) != 3 && abs(abs(openParanethesesLocation) - abs(closingParanthesesLocation)) != 4) ||
        openParanethesesLocation  < 0 || closingParanthesesLocation < 0) {
        return false;
    }

    /* Checks if the label is legal*/
    possibleLabel = my_malloc(openParanethesesLocation + 1);
    possibleLabel[openParanethesesLocation] = '\0';
    strncpy(possibleLabel, opString, openParanethesesLocation);
    if (!isLegalLabelName(possibleLabel)) {
        *hasError = indexIllegalLabelError;
        return false;
    }

    /* Check if the string between parantheses is a Register*/

     possibleRegister = my_malloc(10);
    strncpy(possibleRegister, opString + openParanethesesLocation + 1,
            closingParanthesesLocation- openParanethesesLocation - 1);
    possibleRegister[closingParanthesesLocation - openParanethesesLocation - 1] = '\0';
    if (isStringRegister(possibleRegister, &reg)) {
        if (10 <= reg && reg <= 15) { /* only registers between 10-15 are allowed here*/
            outputOperand->indexOp.reg = reg;
            outputOperand->indexOp.label = possibleLabel;
            outputOperand->addressingType = indexAddress;
            *hasError = noError;
            return true;
        } else {
            *hasError = indexAddressingIllegalRegisterError;
            return true;
        }
    }
    return false;
}


/* ==== Section 2/3: WORD CAPSULE AND CODEIMG =====*/

/* create a list of machine words (machineWordCapsule) corresponding to one Sentence/line
 * mostly places data in the correct way inside a 20 bit array */
machineWordCapsule * getWordCapsule(opcode curr_opcode, funct curr_funct, operand *operandList, int amountOfOperands) {
    machineWordCapsule *codeCapsule = my_malloc(sizeof(machineWordCapsule));
    char *firstWord;
    int i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < wordSize; j++) codeCapsule->x[i].binaryWord[j] = '0';
        codeCapsule->x[i].wordType = nullMachineWord;
        codeCapsule->x[i].relevantLabel = NULL;
    }
    firstWord = codeCapsule->x[0].binaryWord;
    codeCapsule->x[0].wordType = completeCommandWord;
    firstWord[curr_opcode] = '1';
    firstWord[absoluteBit] = '1';

    if (amountOfOperands == 0) {
        codeCapsule->machineWordCount = 1;
        return codeCapsule;
    } else {

        /* Create the code word by the data:*/
        codeCapsule->machineWordCount = 2;
        createSecondMachineWordForCmd(codeCapsule->x + 1, curr_funct, operandList, amountOfOperands);

        createExtraMachineWordForCmd(operandList, amountOfOperands, codeCapsule);


        /* If has operands, we need at least 2 machine words*/
        return codeCapsule;
    }
}
/* returns the second machine word, which is the main one-
 * containing func and addressingType and possible regs*/
void createSecondMachineWordForCmd(machineWord *secondMachineWord, funct curr_funct, operand *operandList, int amountOfOperands) {
    /* print func into the machine word*/
    char *secondWord = secondMachineWord->binaryWord;
    /* Mark as Absolute*/
    secondWord[absoluteBit] = '1';
    /* fill func*/
    fillNumIntoArrayInBinary(secondWord + 12, curr_funct, 4);

    if (amountOfOperands == 1) { /* ONE OPERAND*/

        createSecondMachineWordDesOperand(secondWord, operandList[0]);

    } else { /* TWO OPERANDS*/
        /*print operand type into machine word*/
        fillSecondMachineWordOriginOperand(secondWord, operandList[0]);

        createSecondMachineWordDesOperand(secondWord, operandList[1]);
    }
    secondMachineWord->wordType = completeCommandWord;

}
void createSecondMachineWordDesOperand(char *secondWord, operand disOperand) {
    fillNumIntoArrayInBinary(secondWord + 0, disOperand.addressingType, 2);

    if (disOperand.addressingType == registerAddress) {
        fillNumIntoArrayInBinary(secondWord + 2, disOperand.regDirOp.reg, 4);
    }
    if (disOperand.addressingType == indexAddress) {
        fillNumIntoArrayInBinary(secondWord + 2, disOperand.indexOp.reg, 4);

    }
}
void fillSecondMachineWordOriginOperand(char *secondWord, operand originOperand) {
    fillNumIntoArrayInBinary(secondWord + 6, originOperand.addressingType, 2);

    if (originOperand.addressingType == registerAddress) {
        fillNumIntoArrayInBinary(secondWord + 8, originOperand.regDirOp.reg, 4);
    }
    if (originOperand.addressingType == indexAddress) {
        fillNumIntoArrayInBinary(secondWord + 8, originOperand.indexOp.reg, 4);

    }
}
/* fill the extra machine words that are just data created
 * depending on addressing type*/
void createExtraMachineWordForCmd(operand *operandList, int amountOfOperands, machineWordCapsule *machineWordList) {
    int i, amountOfExtraWords = 0, currI;

    /* For each operand. each one can make between 0-2 extra words*/
    for (i = 0; i < amountOfOperands; i++) {
        if (operandList[i].addressingType == immediateAddress) {
            currI = 2 + amountOfExtraWords;
            fillNumIntoArrayInBinary(machineWordList->x[currI].binaryWord,
                                     operandList[i].imOp.num, wSizeWithoutARE);
            machineWordList->x[currI].wordType = completeCommandWord;
            machineWordList->x[currI].binaryWord[absoluteBit] = '1';
            amountOfExtraWords++;
        }

        if (operandList[i].addressingType == indexAddress) {
            machineWordList->x[2 + amountOfExtraWords].wordType = fillBaseAddressMachineWord;
            machineWordList->x[3 + amountOfExtraWords].wordType = fillOffsetMachineWord;
            machineWordList->x[2 + amountOfExtraWords].relevantLabel = operandList[i].indexOp.label;
            machineWordList->x[3 + amountOfExtraWords].relevantLabel = operandList[i].indexOp.label;

            amountOfExtraWords = amountOfExtraWords + 2;
        }

        if (operandList[i].addressingType == directAddress) {
            machineWordList->x[2 + amountOfExtraWords].wordType = fillBaseAddressMachineWord;
            machineWordList->x[3 + amountOfExtraWords].wordType = fillOffsetMachineWord;
            machineWordList->x[2 + amountOfExtraWords].relevantLabel = operandList[i].dirOp.label;
            machineWordList->x[3 + amountOfExtraWords].relevantLabel = operandList[i].dirOp.label;


            amountOfExtraWords = amountOfExtraWords + 2;
        }

        if (operandList[i].addressingType == registerAddress) {
            /* do nothing*/
        }
    }
    machineWordList->machineWordCount = machineWordList->machineWordCount +amountOfExtraWords;
}
void addToCodeImg(machineWordCapsule codeImg[codeImgMaxSize], machineWordCapsule x, int *amountOfCodeCapsules) {
    codeImg[*amountOfCodeCapsules] = x;
}

/* ==== Section 3/3: MISC =====*/

void commandToOptAndFunc(Command cmd, opcode *opcodeOut, funct *functOut) {
    commandTableElement currElement;
    int i;
    *opcodeOut = nullOP;
    *functOut = nullOP;
    /* iterate through the lookup cmdTable, if commands are same return the opcodeOut of found. */
    for (i = 0; i < amountOfCommands; i++) {
        currElement = commandTable[i];
        if (cmd == currElement.cmd) {
            *opcodeOut = currElement.op;
            *functOut = currElement.fun;
        }
    }
}

Error checkIfAddressingTypeFitCommand(Command cmd, addressingType firstAdType, addressingType secondAdType) {
    commandTableElement tableElement;
    int amountOfOperands = 0, i;
    /* check how many operands the command has*/
    if (firstAdType != nullAddress) {
        amountOfOperands++;
        if (secondAdType != nullAddress) {
            amountOfOperands++;
        }
    }
    /* find the cmd in the lookup table*/
    for (i = 0; i < amountOfCommands; i++) {
        tableElement = commandTable[i];
        if (cmd == tableElement.cmd) {
            break;
        }
    }
    /* if incorrect amount of operands*/
    if (amountOfOperands != tableElement.amountOfOperands) {
        return incorrectAmountOfOperandsError;
    }
    /* if the operand has two*/
    if (amountOfOperands == 1) {
        if (checkIfAdTypeInArray(tableElement.legalAdrsTypesDestinationOp, firstAdType)) {
            return noError;
        } else return firstOperatorIncorrectTypeError;
    }
    if (amountOfOperands == 2) {
        if (checkIfAdTypeInArray(tableElement.legalAdrsTypesOrigin, firstAdType)) {
            if (checkIfAdTypeInArray(tableElement.legalAdrsTypesDestinationOp, secondAdType)) {
                return noError;
            } else return secondOperatorIncorrectTypeError;
        } else return firstOperatorIncorrectTypeError;
    }
    return noError;
}

bool checkIfAdTypeInArray(const addressingType *addressingTypeList, addressingType x) {
    int i;
    for (i = 0; i < 4; i++) {
        if (addressingTypeList[i] == x) {
            return true;
        }
    }
    return false;
}
