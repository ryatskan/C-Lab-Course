
#include "firstPass.h"

void updateAttributes(Sentence currSentence, attribute *attOutput1, attribute *attOutput2);

Error processCommand(Sentence inputSentence, dataStructures *a, iteratorCapsule *b);

Error processSentence(Sentence currSentence, dataStructures *a, iteratorCapsule *b);

Error sentenceToSymbolTableLabel(char *key, Sentence currSentence, table *symbolTablePointer);

char *extractKey(Sentence x);

void fixDataAddresses(table *symbolTable, machineWord *dataImg, int ic, int dc);

/* ===== Section 1/3: Main Section =====*/
/*
 * Runs the first pass.
 * First analyze syntax. (process line -> Sentence, check for syntax errors)
 * Then handle semantics. (fill dataImg, codeImg, symbolTable depending on operator and input parameters
 * , check for semantic errors)
 * If encountered an error, after finishing iterating return the error.
 */
Error
firstPass(char **lines, dataStructures *a, iteratorCapsule *b,
          Sentence *sentencesOutput) {

    int i;
    Error hasError, hadError = noError;
    *b->amountOfSentences = linesLen(lines);
    for (i = 0; i < linesLen(lines); i++) {
        /* extract all relevant data from line into a Sentence type,
         * leaving input parameters unprocessed (list of strings)*/

        hasError = lineToSentence(lines[i], &sentencesOutput[i]);
        sentencesOutput[i].line = i;

        if (hasError != noError) {
            printError(hasError, i + 1);
            hadError = hasError;
            sentencesOutput[i].containsSyntaxErrors = true;
        } else {


            hasError = processSentence(sentencesOutput[i], a, b);
            if (hasError != noError) {
                printError(hasError, i + 1);
                hadError = hasError;
            }
        }
    }
    fixDataAddresses(a->symbolTable, a->dataImg, *b->ic, *b->dc);
    return hadError;
}

Error processSentence(Sentence currSentence, dataStructures *a, iteratorCapsule *b) {
    /* no point in analyzing an incorrect sentence*/
    if (currSentence.containsSyntaxErrors == true)
        return noError;
    if (currSentence.type != noteType && currSentence.type != emptyType) {

        if (currSentence.type == directiveType) { /*If a directive*/
            /* process input parameters and update the directive in symbolTable + dataImg*/
            return processDirective(currSentence, a->symbolTable, a->dataImg, b->dc);

        } else { /* A COMMAND*/

            return processCommand(currSentence, a, b);
            /* add to symbol table*/
        }
    }
    return noError;
}
/* ===== Section 2/3: SYMBOL TABLE FUNCTIONS =====*/
/*
 * Add a new entry to the end of the symbolTable.
 */
void addEntryToSymbolTable(table *symbolTable, table newSymbolEntry) {
    table currSymbolEntry = *symbolTable;
    table nullEntry;
    if (currSymbolEntry->value == nullVal) {
        *symbolTable = newSymbolEntry;
        newSymbolEntry->next = currSymbolEntry;
    } else {
        while (currSymbolEntry->next->value != nullVal) {
            currSymbolEntry = currSymbolEntry->next;
        }
        nullEntry = currSymbolEntry->next;
        currSymbolEntry->next = newSymbolEntry;
        newSymbolEntry->next = nullEntry;
    }
}

/*
 * Add a new Sentence to the symbol table.
 * Receives the value to add to the table, fill rest of the new symbolTable entry parameters
 * depending on the sentence
 */
Error addSentenceToSymbolTable(Sentence currSentence, table *symbolTablePointer, int value) {
    attribute attributes[2];
    table newSymbolEntry;
    Error hasError;
    char *key = extractKey(currSentence);
    /* checks it is in line with previous table entries, depending on type of Sentence*/
     hasError = sentenceToSymbolTableLabel(key, currSentence, symbolTablePointer);
    if (hasError != noError) {
        return hasError;
    }

    updateAttributes(currSentence, attributes, (attributes + 1));

    newSymbolEntry = my_malloc(sizeof(struct table_entry));
    /* create a new entry to the table*/
    *newSymbolEntry = createSymbolEntry(key, value, attributes[0], attributes[1]);

    addEntryToSymbolTable(symbolTablePointer, newSymbolEntry);
    return noError;
}
/*
 * If sentence is of extern type, the (only) relevant label is the first op.
 * Else, the relevant label is the sentence label.
 */
char *extractKey(Sentence x) {
    if (x.directiveName == externDirective) {
        return x.inputParameters[0];
    } else {
        return x.label;
    }
}

/*
 * Find which label to insert to table (if extern - add operand, else add the line label),
 * while also doing error handing
 */
Error sentenceToSymbolTableLabel(char *key, Sentence currSentence, table *symbolTablePointer) {
    if (currSentence.directiveName == externDirective) {

        table *entry = findLabelInSymbolTable(key, symbolTablePointer);
        if (entry == NULL)
            return noError;

        else if ((*entry)->attributes[0] == externAttribute) {
            return noError;
        } else { /* If found label in symbolTable AND it wasn't marked there as Extern*/
            return externLabelDuplicateError;
        }
    } else { /* cmd OR .data OR .string (entry doesnt reach here)*/
        if (isNull(findLabelInSymbolTable(currSentence.label, symbolTablePointer))) {
            return noError;
        } else {
            return labelDuplicateError;
        }
    }


}

void updateAttributes(Sentence currSentence, attribute *attOutput1, attribute *attOutput2) {
    if (currSentence.type == commandType) {
        *attOutput1 = codeAttribute;
    }
    if (currSentence.directiveName == dataDirective || currSentence.directiveName == stringDirective) {
        *attOutput1 = dataAttribute;
    }
    if (currSentence.directiveName == externDirective) {
        *attOutput1 = externAttribute;
    }
}
/*
 * Iterate over all the symbol table to look for a specific label.
 * return true if found it, false otherwise.
 */
bool isLabelInsideSymbolTable(table *symbolTable, char *label) {


    table currSymbolEntry = *symbolTable;
    while (currSymbolEntry->value != nullVal) {
        if (areStringsEqual(currSymbolEntry->key, label)) {
            return true;
        }
        currSymbolEntry = currSymbolEntry->next;
    }
    return false;


}

/*
 * create a new symbolEntry based on given parameters.
 */
struct table_entry createSymbolEntry(char *key, int value, attribute attribute1, attribute attribute2) {
    struct table_entry newSymbolEntry;
    char *symbolEntryKey;
    newSymbolEntry.next = NULL;
    newSymbolEntry.value = value;
    newSymbolEntry.offset = value % 16;
    newSymbolEntry.baseAddress = value - newSymbolEntry.offset;/*(int) pow(2, highestPowerOf2(value));*/
    symbolEntryKey = my_malloc(strlen(key) + 1);
    strcpy(symbolEntryKey, key);
    newSymbolEntry.key = symbolEntryKey;
    newSymbolEntry.attributes[0] = attribute1;
    newSymbolEntry.attributes[1] = attribute2;
    return newSymbolEntry;
}
/* ===== Section 3/3: MISC ===== */
/*
 * The input code has directives and commands mixed, In output commands are before directives.
 * Fix the value (aka address) so the start of the data directives will be instantly after
 * end of code
 */
void fixDataAddresses(table *symbolTable, machineWord *dataImg, int ic, int dc) {
    int i;
    /* FIX SYMBOL TABLE*/
    table *currSymbolEntry = symbolTable;
    while ((*currSymbolEntry)->value != nullVal) {
        if ((*currSymbolEntry)->attributes[0] == dataAttribute) {
            (*currSymbolEntry)->value = (*currSymbolEntry)->value + ic + 100;
            (*currSymbolEntry)->offset = (*currSymbolEntry)->value % 16;
            (*currSymbolEntry)->baseAddress = (*currSymbolEntry)->value - (*currSymbolEntry)->offset;
        }
        currSymbolEntry = &(*currSymbolEntry)->next;
    }
    /* FIX DATA MACHINE WORD ADDRESSES*/
    for (i = 0; i < dc; i++)
        dataImg[i].address = dataImg[i].address + 100 + ic;
}