#include "secondPass.h"
#include <stdio.h>

/* ==== Section 1/4: MAIN SECTION ====*/
/*
 * Process the entry directives (that were unprocessed in the first pass)
 * and fill the codeImg with missing label addresses and offset.
 * OUTPUT: print all 4 output files if fpass and spass were successful.
 */
void
secondPass(dataStructures *a, iteratorCapsule *b, char **lines, Sentence *sentenceList, char *fileName) {
    int i, exTableI = 0;
    Error hasError;
    bool hadError = false;
    initExternTable(a,b);

    for (i = 0; i < *b->amountOfSentences; i++) {
        Sentence currSentence = sentenceList[i];
        /* If entry */
        if (currSentence.directiveName == entryDirective) {

            hasError = processEntryDirective(currSentence, a->symbolTable);
            if (hasError) {
                hadError = true;
                printError(hasError, i + 1);
            }
        }
            /* If command*/
        else if (currSentence.type == commandType) {

            machineWordCapsule *wordCapsule =
                    sentenceToAssociatedMachineWord(currSentence, a->codeImg, *b->amountOfCodeCapsules);

            hasError = fillCodeImg(currSentence, a->symbolTable, wordCapsule);
            if (hasError) {
                hadError = true;
                printError(hasError, i + 1);
            }
            updateExternalTable(a->exTable, *wordCapsule, &exTableI);
        }


    }
    *b->amountOfExOp = exTableI;
    if (!hadError) writeOutputFiles(lines, a, b, fileName);
}

/*
 * Process an entry directive (left unprocessed from fpass)
 */
Error processEntryDirective(Sentence currSentence, table *symbolTable) {
    table *symbolEntry;
    if (currSentence.amountOfParameters > 1) {
        return tooManyParametersError;
    }
    if (currSentence.amountOfParameters == 0) {
        return tooLittleParametersError;
    }
    if (!isLegalLabelName(currSentence.inputParameters[0])) {
        return illegalLabelError;
    }

    /* Look for the label operand in the symbol table and update the
     * symbolTable entry's attribute. if not found in table send error.*/
    symbolEntry = findLabelInSymbolTable(currSentence.inputParameters[0], symbolTable);
    if (symbolEntry == NULL) {
        return labelIsNotInSymbolTable;
    } else {
        (*symbolEntry)->attributes[1] = entryAttribute;
        return noError;
    }
}

/*
 * Receives a command. If the command has operands which are labels,
 * its wordCapsule is incomplete. fill its wordCapsule
 */
Error fillCodeImg(Sentence currSentence, table *symbolTable, machineWordCapsule *wordCapsule) {

    int i;
    table *symbolEntry;
    for (i = 0; i < wordCapsule->machineWordCount; i++) {
        machineWord *currWord = wordCapsule->x + i;
        if (currWord->wordType == fillOffsetMachineWord) {
            symbolEntry = findLabelInSymbolTable(currWord->relevantLabel, symbolTable);
            if (symbolEntry == NULL)
                return labelIsNotInSymbolTable;
            if ((*symbolEntry)->attributes[0] == externAttribute)
                currWord->binaryWord[externBit] = '1';
            else currWord->binaryWord[relativeBit] = '1';

            fillNumIntoArrayInBinary(currWord->binaryWord, (*symbolEntry)->offset, externBit);
            currWord->wordType = completeCommandWord;
        }
        if (currWord->wordType == fillBaseAddressMachineWord) {
            currWord->wordType = completeCommandWord;
            symbolEntry = findLabelInSymbolTable(currWord->relevantLabel, symbolTable);
            if (symbolEntry == NULL)
                return labelIsNotInSymbolTable;
            if ((*symbolEntry)->attributes[0] == externAttribute) currWord->binaryWord[externBit] = '1';
            else currWord->binaryWord[relativeBit] = '1';

            fillNumIntoArrayInBinary(currWord->binaryWord, (*symbolEntry)->baseAddress, 16);
        }
    }
    return noError;
}

/* ==== Section 2/4: WRITE OUTPUT FILES ====*/
/*
 * Main Program to print all the 4 output files.
 * "ps.am", "ps.ob", "ps.ent", "ps.ext"
 */
void writeOutputFiles(char **lines, dataStructures *a, iteratorCapsule *b, char *fileName) {
    writeExternalFile(a->exTable, *b->amountOfExOp, fileName);
    writeObjectFile(a->codeImg, a->dataImg, *b->amountOfCodeCapsules, *b->dc, *b->ic, fileName);
    writeEntryFile(a->symbolTable, fileName);
}
/*
 * Writes the external file that contains all the address of extern symbols
 * that needs to be filled later.
 */
void writeExternalFile(externalTableEntry *exTable, int tableSize, char *fileName) {
    int i;
    externalTableEntry currEntry;
    /* dont open a new file if there is no externs to write*/
    if (tableSize != 0) {
        FILE *fileDesc = openFile(fileName, ".ext", "w", "files/");

        for (i = 0; i < tableSize; i++) {
            currEntry = exTable[i];
            fprintf(fileDesc, "%s BASE %d\n", currEntry.label, currEntry.machineWordBaseAddress);
            fprintf(fileDesc, "%s OFFSET %d\n", currEntry.label, currEntry.machineWordOffset);
            if (i != tableSize) fprintf(fileDesc, "\n");
        }
        fclose(fileDesc);
    }
}
/*
 * Writes the object file that contains the machine code
 */
void writeObjectFile(machineWordCapsule *codeImg, machineWord *dataImg, int wordCapsuleCount, int dc, int ic,
                     char *fileName) {
    int i, j;
    FILE *fileDesc =openFile(fileName, ".ob", "w", "files/");
    machineWord currWord;
    fprintf(fileDesc, "%d, %d\n", ic, dc);
    for (i = 0; i < wordCapsuleCount; i++) {
        for (j = 0; j < codeImg[i].machineWordCount; j++) {
            currWord = codeImg[i].x[j];
            printMachineWord(currWord, fileDesc);
        }
    }
    for (i = 0; i < dc; i++) {
        currWord = dataImg[i];
        printMachineWord(currWord, fileDesc);
    }
    fclose(fileDesc);
}

/*
* Writes the Entry file that contains all Symbols that were declared as
 * entry.
*/
void writeEntryFile(table *tabPointer, char *fileName) {
    /* if table contains no entry symbols don't open a new file */
    if (areThereEntrySymbols(tabPointer)) {

        FILE *fileDesc = openFile(fileName, ".ent", "w", "files/");
        if (*tabPointer != NULL) {
            table *currSymbolEntry = tabPointer;
            while ((*currSymbolEntry)->value != nullVal) {
                if ((*currSymbolEntry)->attributes[1] == entryAttribute) {
                    fprintf(fileDesc, "%s, %d, %d\n",
                            (*currSymbolEntry)->key, (*currSymbolEntry)->baseAddress, (*currSymbolEntry)->offset);
                }
                currSymbolEntry = &(*currSymbolEntry)->next;
            }
        }
        fclose(fileDesc);
    }
}
/*
 * Checks if there are any symbols declared as entry in the symbolTable.
 */
bool areThereEntrySymbols(table *tabPointer) {
    if (*tabPointer != NULL) {
        table *currSymbolEntry = tabPointer;
        while ((*currSymbolEntry)->value != -1) {
            if ((*currSymbolEntry)->attributes[1] == entryAttribute) {
                return true;
            }

            currSymbolEntry = &(*currSymbolEntry)->next;
        }
    }
    return false;
}
/* ==== Section 3/4: HANDLE EXTERNAL ====*/

void initExternTable(dataStructures *a, iteratorCapsule *b) {
    int i;
    /*b->amountOfExOp = amountOfExternalOperands(a->codeImg, *b->amountOfSentences);
    */a->exTable = my_malloc(sizeof(externalTableEntry)* *b->ic);
    for (i=0;i<*b->amountOfExOp;i++) {
        a->exTable[i].label = NULL;
        a->exTable[i].machineWordBaseAddress = -1;
        a->exTable[i].machineWordOffset = -1;
    }

}

/*
 * If the given wordCapsule has machineWords marked as external, add its addresses to
 * the exTable.
 */
void updateExternalTable(externalTableEntry *exTable, machineWordCapsule wordCapsule, int *exTableIterator) {
    int i;
    /* Find a word that is EXTERN. add its address and the next word address (base addresses are followed by
     * offset) to the exTable as ONE entry.*/
    for (i = 0; i < wordCapsule.machineWordCount; i++) {
        if (wordCapsule.x[i].binaryWord[externBit] == '1') {
            exTable[*exTableIterator].label = wordCapsule.x[i].relevantLabel;
            exTable[*exTableIterator].machineWordBaseAddress = wordCapsule.x[i].address;
            exTable[*exTableIterator].machineWordOffset = wordCapsule.x[i + 1].address;
            (*exTableIterator)++;
            i++;
        }
    }
}
/* ==== Section 4/4: PRINT MACHINE WORD ====*/
/*
 * Receives a machine word. prints it in a specific formula
 * explained in the maman (page 55).
 */
void printMachineWord(machineWord word, FILE *stream) {
    specialBase spbaseWord = machineWordToSpecialBase(word);
    char address[5] = "0000";
    sprintf(address + 1, "%i", word.address);
    if (address[4] == '\0') address[4] = 0; /* fix snprintf putting \0 into address*/
    fprintf(stream, "%s A%x-B%x-C%x-D%x-E%x\n",address, spbaseWord.A, spbaseWord.B,
            spbaseWord.C, spbaseWord.D, spbaseWord.E);
}
/*
 * Transforms each machineWords to a special base explained in the maman (page 55)
 */
specialBase machineWordToSpecialBase(machineWord x) {
    specialBase output;
    char *binaryWordCopy = malloc(wordSize + 1);
    binaryWordCopy[wordSize] = '\0';
    strncpy(binaryWordCopy, x.binaryWord, wordSize);
    swapArray(binaryWordCopy, wordSize);
    output.A = FourBinToDecimal(binaryWordCopy);
    output.B = FourBinToDecimal(binaryWordCopy + 4);
    output.C = FourBinToDecimal(binaryWordCopy + 8);
    output.D = FourBinToDecimal(binaryWordCopy + 12);
    output.E = FourBinToDecimal(binaryWordCopy + externBit);
    free(binaryWordCopy);
    return output;
}

/*
 * Each sentence has a corresponding machineWords. Find it in the codeImg.
 */
machineWordCapsule *sentenceToAssociatedMachineWord(Sentence x, machineWordCapsule *codeImg, int codeImgSize) {
    int i;
    for (i = 0; i < codeImgSize; i++) {
        if (codeImg[i].associatedSentence.line == x.line) {
            return codeImg + i;
        }
    }
    printf("error1");
    return NULL;
}