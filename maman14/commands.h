#ifndef ASSEMBLERPROJECT_COMMANDS_H
#define ASSEMBLERPROJECT_COMMANDS_H
#include "global.h"
#include "firstPass.h"
static const char commands[16][6] = {"mov", "cmp", "add", "sub", "lea", "clr", "not",
                                     "inc", "dec", "jmp", "bne",
                                     "jsr", "red", "prn", "rts", "stop"};

/** Commands opcode */
typedef enum {
    /* First Group */
    movOp = 0,
    cmpOp = 1,
    addOp = 2,
    subOp = 2,
    leaOp = 4,
    /* Second Group */
    clrOp = 5,
    notOp = 5,
    incOp = 5,
    decOp = 5,
    jmpOp = 9,
    bneOp = 9,
    jsrOp = 9,
    redOp = 12,
    prnOp = 13,
    /* Third Group */
    rtsOp = 14,
    stopOp = 15,
    /** null operand */
    nullOP = -1
} opcode;

/** Commands funct */
typedef enum {
    /* OPCODE 2 */
    addFunct = 10,
    subFunct = 11,

    /* OPCODE 5 */
    clrFunct = 10,
    notFunct = 11,
    incFunct = 12,
    decFunct = 13,

    /* OPCODE 9 */
    jmpFunct = 10,
    bneFunct = 11,
    jsrFunct = 12,

    /** Default (No need/Error) */
    nullFunct = 0
} funct;

typedef struct  {
    Command cmd;
    opcode op;
    funct fun;
    addressingType legalAdrsTypesDestinationOp[4];
    addressingType legalAdrsTypesOrigin[4];
    int amountOfOperands;
} commandTableElement;

Error stringToOperand(char *opString, operand *operand);
bool isIndexOperand(char *opString, operand *outputOperand, Error *hasError);

void createSecondMachineWordForCmd(machineWord *secondMachineWord, funct curr_funct, operand *operandList, int amountOfOperands);
void createExtraMachineWordForCmd(operand *operandList, int amountOfOperands, machineWordCapsule *machineWordList);
Error checkIfAddressingTypeFitCommand(Command cmd, addressingType firstAdType, addressingType secondAdType);
bool checkIfAdTypeInArray(const addressingType *addressingTypeList, addressingType x);
void createSecondMachineWordDesOperand(char *secondWord, operand disOperand);
void fillSecondMachineWordOriginOperand(char *secondWord, operand originOperand);

void commandToOptAndFunc(Command cmd, opcode *opcodeOut, funct *functOut);
Error processOperands(Sentence currSentence, operand **operandListOutput);
machineWordCapsule * getWordCapsule(opcode curr_opcode, funct curr_funct, operand *operandList, int amountOfOperands);
void addToCodeImg(machineWordCapsule codeImg[codeImgMaxSize], machineWordCapsule x, int *amountOfCodeCapsules);

#endif
