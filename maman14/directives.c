
#include "directives.h"

Error processDataDirective(Sentence currSentence, table *symbolTable, machineWord *dataImg, int *dc);
Error processExternDirective(Sentence currSentence, table *symbolTable, machineWord *data_img, int *dc);
Error processStringDirective(Sentence currSentence, table *symbolTable, machineWord *data_img, int *dc);

/* ===== Section 1/3: Main Section =====*/
/*
 * MAIN PROGRAM: receives a sentence of type directive.
 * add the parameter to the dataImg (depends on what kind of directive sentence is it)
 * add to symbol table if sentence has label
 * */
Error processDirective(Sentence currSentence, table *symbolTable, machineWord *dataImg, int *dc) {

    if (currSentence.directiveName == stringDirective) return processStringDirective(currSentence, symbolTable, dataImg,
                                                                                     dc);
    else if (currSentence.directiveName == dataDirective) return processDataDirective(currSentence, symbolTable,
                                                                                      dataImg, dc);
    else if (currSentence.directiveName == externDirective) return processExternDirective(currSentence, symbolTable,
                                                                                          dataImg, dc);
    else return noError; /* if entry type*/
}
/* ===== Section 2/3: directiveName Case Handling =====*/
/*
 * Process data directive
 */
Error processDataDirective(Sentence currSentence, table *symbolTable, machineWord *dataImg, int *dc) {
    int i;
    char *currentParameter;
    Error hasError =  noError;
    /* == Check Correctness ==*/
    if (currSentence.amountOfParameters == 0) {
        return tooLittleParametersError;
    }
    for (i = 0; i < currSentence.amountOfParameters; i++) {
        /*Iterate all parameters, and check if they are integers*/
        currentParameter = currSentence.inputParameters[i];
        if (!isInteger(currentParameter)) {
            return parameterIsNotIntegerError;
        }
    }
    /* Add dataImg & possibly symbolTable*/
    if (currSentence.label != NULL) {
        hasError = addSentenceToSymbolTable(currSentence, symbolTable, *dc);}
    for (i = 0; i < currSentence.amountOfParameters; i++) {
        addToDataImg(dataImg, atoi(currSentence.inputParameters[i]), dc);
    }
    return hasError;
}
/*
 * Process extern directive
 */
Error processExternDirective(Sentence currSentence, table *symbolTable, machineWord *data_img, int *dc) {
    char *labelOperand;
    /* == Check Correctness*/
    if (currSentence.amountOfParameters < 1) {
        return tooLittleParametersError;
    }
    if (currSentence.amountOfParameters > 1) {
        return tooManyParametersError;
    }
    labelOperand = currSentence.inputParameters[0];
    if (!isLegalLabelName(labelOperand)) {
        return illegalLabelError;
    }
    /* == If the symbol is already in symbolTable without extern Att return error, If
     * in symbolTable with extern att do nothing, else add to symbolTable
     * (the logic is done inside) */
    return addSentenceToSymbolTable(currSentence, symbolTable, 0);
}
/*
 * Process data directive - First analyze its correctness, Then add it into dataImg.
 *  If it contains a label add it into symbolTable as well.
 */
Error processStringDirective(Sentence currSentence, table *symbolTable, machineWord *data_img, int *dc) {
    int i, firstWordAddress= *dc;
    char *parameter;
    Error hasError = noError;
    /* == Check Correctness*/
    if (currSentence.amountOfParameters > 1) {
        return tooManyParametersError;
    }
    if (currSentence.amountOfParameters < 1) {
        return tooLittleParametersError;
    }
    parameter = currSentence.inputParameters[0];
    /* check if the parameter is string "X"*/
    if (parameter[0] != '"') {
        return missingOpeningQuoteError;
    }
    if (parameter[strlen(parameter) - 1] != '"') {
        return missingClosingQuoteError;
    }
    for (i = 1; i < strlen(parameter) - 1; i++) {
        addToDataImg(data_img, parameter[i], dc);
    }
    /* == Add to dataImg & symbolTable*/
    if (currSentence.label != NULL) {
        hasError = addSentenceToSymbolTable(currSentence, symbolTable, firstWordAddress);
    }
    /* If found no errors inserting the sentence into the symbolTable:*/
    if (!hasError) {
        addToDataImg(data_img, '\0', dc);
        return noError;
    }
    else return hasError;
}
/* ===== Section 3/3: Misc =====*/
/*
 * Transforms given integer into binary and add it to the dataImg as a machineWord
 */
void addToDataImg(machineWord *dataImg, int data, int* dc) {
    int i;
    char *bin;
    bin = dec2bin(abs(data), wSizeWithoutARE);
    if (data < 0 ) {
        swapArray(bin, wSizeWithoutARE);
        binTo2Complement(bin);
        swapArray(bin,wSizeWithoutARE);
    }
    for (i=0;i<16;i++) {
        dataImg[(*dc)].binaryWord[i] =bin[i];
    }
    /* The ARE attribute is always set to Absolute in directives*/
    dataImg[*dc].binaryWord[externBit] = '0';
    dataImg[*dc].binaryWord[relativeBit] = '0';
    dataImg[*dc].binaryWord[absoluteBit] = '1';
    dataImg[*dc].binaryWord[lastBit] = '0';
    dataImg[*dc].wordType = completeDataWord;
    dataImg[*dc].address = *dc;
    dataImg[*dc].relevantLabel = NULL;
    (*dc)++;
}