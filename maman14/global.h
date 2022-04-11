#ifndef ASSEMBLERPROJECT_GLOBAL_H
#define ASSEMBLERPROJECT_GLOBAL_H

/*
 * Declaring macros and data that is for project-wide use.
 */
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000
#define areStringsEqual(a, b) (strcmp(a,b) == 0)
#define isNull(a) (a==NULL)
#define isNullDirective(a) (a==nullDirective)
#define isNullCommand(a) (a==nullCommand)
#define isNotNull(a) (a!=NULL)
#define MAX_MACRO_NAME_LENGTH 100
#define MAX_LABEL_LENGTH 32
#define MAX_AMOUNT_OF_CODE_WORDS_PER_SENTENCE 6
#define wordSize 20
#define externBit 16
#define absoluteBit 18
#define relativeBit 17
#define wSizeWithoutARE 16
#define lastBit 19
#define nullVal -1


/* ====== Section 1/3: SENTENCE COMPONENTS ======*/
#define amountOfDirectives 4
#define amountOfCommands 16
#define amountOfRegisters 16
typedef enum {
    mov, cmp, add, sub, lea, clr, notCmd, inc, dec, jmp, bne, jsr, red, prn, rts, stop, nullCommand
} Command;

typedef enum {
    dataDirective, stringDirective, entryDirective, externDirective, nullDirective
} Directive;
static const char directives[amountOfDirectives][20] = {".data", ".string", ".entry", ".extern"};
typedef enum {
    emptyType, noteType, directiveType, commandType
} sentenceType;
typedef enum {
    /** Immediate addressing (0) */
    immediateAddress = 0,
    /** Direct addressing (1) */
    directAddress = 1,
    /** Index addressing (2) */
    indexAddress = 2,
    /** Register addressing */
    registerAddress = 3,
    /** Failed/Not detected addressing */
    nullAddress = -1
} addressingType;


typedef enum {
    R0 = 0,
    R1 = 1,
    R2 = 2,
    R3 = 3,
    R4 = 4,
    R5 = 5,
    R6 = 6,
    R7 = 7,
    R8 = 8,
    R9 = 9,
    R10 = 10,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15,
    nullRegister
} Register;
static const char regs[16][5] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13",
                                 "r14", "r15"};

typedef struct {
    char *label;
    Register reg;
} indexOperand;
typedef struct {
    Register reg;
} registerDirectOperand;
typedef struct {
    int num;
} immediateOperand;
typedef struct {
    char *label;
} directOperand;
typedef struct {
    indexOperand indexOp;
    registerDirectOperand regDirOp;
    immediateOperand imOp;
    directOperand dirOp;
    addressingType addressingType;
} operand;

typedef struct {
    Directive directiveName;
    Command commandName;
    sentenceType type;
    char *label;
    char **inputParameters;
    int amountOfParameters;
    int line;
    int containsSyntaxErrors;
} Sentence;


/* ===== Section 2/3: MACHINE WORD ======*/

typedef enum {
    completeCommandWord = 1,
    completeDataWord = 2,
    fillBaseAddressMachineWord = 3,
    fillOffsetMachineWord = 4,
    nullMachineWord = 0
} machineWordType;
typedef struct {
    char binaryWord[20];
    machineWordType wordType;
    char *relevantLabel;
    int address;
} machineWord;
typedef struct {
    machineWord x[6];
    int machineWordCount;
    Sentence associatedSentence;
} machineWordCapsule;

/* ===== DATA STRUCTURES ======*/
#define codeImgMaxSize 300
#define dataImgMaxSize 300

typedef enum {
    codeAttribute = 1,
    dataAttribute = 2,
    entryAttribute = 3,
    externAttribute = 4,
    nullAttribute = 0

} attribute;
/* A data structure used for printing Extern file*/
typedef struct {
    char *label;
    int machineWordOffset;
    int machineWordBaseAddress;
} externalTableEntry;

typedef struct table_entry *table;
struct table_entry {
    /** Next entry in table*/
    struct table_entry *next;
    /** Key (symbol name) is a string*/
    char *key;
    /** Address of the symbol */
    int value;
    /** baseAddress + offset = value */
    int baseAddress;
    int offset;

    attribute attributes[2];
};


typedef struct {
    int *ic;
    int *dc;
    int *amountOfCodeCapsules;
    int *amountOfSentences;
    /* for second pass use*/
    int *amountOfExOp;
} iteratorCapsule;
typedef struct {
    table *symbolTable;
    machineWord *dataImg;
    machineWordCapsule *codeImg;
    /* Second pass*/
    externalTableEntry *exTable;
} dataStructures;


/* ===== Section 3/3: MISC ======*/

typedef enum {
/* parameters - for directives, operators - for commands*/
    noError = 0,
    illegalLabelError = 1,
    unknownSentenceTypeError = 2,
    commaAfterOperationError = 3,
    commaInEndOfLineError = 4,
    twoConsecutiveCommasError = 5,
    parametersWithoutSeparatingCommaError = 6,
    tooLittleOperatorsError = 7,
    tooManyOperatorsError = 8,
    tooLittleParametersError = 9,
    tooManyParametersError = 10,
    missingClosingQuoteError = 11,
    missingOpeningQuoteError = 12,
    parameterIsNotIntegerError = 13,
    externLabelDuplicateError = 14,
    labelDuplicateError = 15,
    firstOperatorIncorrectTypeError = 16,
    secondOperatorIncorrectTypeError = 17,
    incorrectAmountOfOperandsError = 18,
    unrecognizableAddressingType = 19,
    indexAddressingIllegalRegisterError = 20,
    immediateOperandNotIntegerError = 21,
    indexIllegalLabelError = 22,
    labelIsNotInSymbolTable = 23,
    fileError = 24
} Error;


#endif
