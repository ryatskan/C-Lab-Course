#include "lineToSentence.h"

/* Main Function:
 * Receive a line. extract the relevant information from it -
 * command name, possible label, all input parameters
 * outputs a Sentence representing all that information.
 * */
Error lineToSentence(char *line, Sentence *sentenceOutput) {
    char *newLine, *inputParamaters, **splits;
    Error hasError;
    int amountOfSplits;
    sentenceOutput->amountOfParameters = 0;
    sentenceOutput->inputParameters = NULL;
    sentenceOutput->directiveName = nullDirective;
    sentenceOutput->commandName = nullCommand;
    sentenceOutput->containsSyntaxErrors = false;
    newLine = my_malloc(MAX_LINE_LENGTH + 1);
    strcpy(newLine, line);

    inputParamaters  = my_malloc(strlen(newLine) + 100);

    splits  = splitStringByDelimiter(line, " ", &amountOfSplits);

    if (isNoteOrEmptySentence(sentenceOutput, line)) {
        return noError;
    }
    /* Check for label*/
    hasError = checkLineForLabel(&splits, sentenceOutput, &newLine);
    if (hasError) return hasError;
    /* Check the line for operator*/
    hasError = checkLineForOperator(splits, sentenceOutput, &newLine);
    if (hasError) {
        return hasError;
    }

    strcpy(inputParamaters, newLine);
    /* Check the line for input parameters*/
    hasError = extractInputParameters(inputParamaters, sentenceOutput);


    if (hasError) return hasError;
    else {
        return noError;
    }
}
/*
 * Checks if the line has an operator (directive/cmd).
 * If so, update the sentence accordingly.
 */
Error checkLineForOperator(char **splits, Sentence *sentenceOutput, char **linePointer) {
    /* check for name*/
    Directive directiveName;
    Command commandName;
    if (isNotNull(sentenceOutput->label)) {/* if has a label, the first split is the label so ignore it*/
        splits++;
    }
    /* check if is directive */
    if ((directiveName = checkIfStringIsDirective(splits[0])) != nullDirective) {
        sentenceOutput->type = directiveType;
        sentenceOutput->directiveName = directiveName;
        *linePointer = *linePointer + strlen(splits[0]);
        return noError;
    }
        /* check if is command*/
    else if ((commandName = checkIfStringIsCommand(splits[0])) != nullCommand) {
        sentenceOutput->type = commandType;
        sentenceOutput->commandName = commandName;
        *linePointer = *linePointer + strlen(splits[0]);
        return noError;
    } else {
        return unknownSentenceTypeError;
    }
}
/*
 * Checks if the line has a (legal) label.
 * If so, update the sentence accordingly.
 */
Error checkLineForLabel(char ***splitsPointer, Sentence *sentenceOutput, char **linePointer) {

    char *potentialLabel = (*splitsPointer)[0];
/* check for potentialLabel*/
    if (RemoveEndingColon(potentialLabel)) {

        if (isLegalLabelName(potentialLabel)) { /* check if legal*/
            sentenceOutput->label = potentialLabel;
            *linePointer = *linePointer + strlen(potentialLabel) + 2;
            return noError;
        } else {
            sentenceOutput->label = NULL;
            return illegalLabelError;
        }
    } else {
        sentenceOutput->label = NULL;
        return noError;
    }
}

/*
 * Receive a line. checks if the line represent a note
 * or empty sentence.
 * If so, edit sentenceOutput accordingly.
 */
int isNoteOrEmptySentence(Sentence *sentenceOutput, char *line) {
    if (strlen(line) == 0) {
        sentenceOutput->type = emptyType;
        return 1;
    }
    if (line[0] == ';') {
        sentenceOutput->type = noteType;
        return 1;
    }
    return 0;
}

/*
 * Receive a string, that represent the substring that only contains input parameters.
 * (ex "x: add a, b" - the given parameters is "a, b")
 * extracts all input parameters into sentenceOutput while looking for errors.
 */
Error extractInputParameters(char *parameters, Sentence *sentenceOutput) {
    int i, amountOfSplits, lineLen;
    char **splits;

    if (strlen(parameters) == 0) { /* should have a space as a first char*/
        return noError;
    }
    parameters++; /* ignore the first space*/
    parameters = trimSpacesSurroundingCommas(parameters);
    splits = splitStringByDelimiter(parameters, ",", &amountOfSplits);
    lineLen = (int) strlen(parameters);
    if (strlen(parameters) == 0) {
        return noError;
    }
    /* end with a comma*/
    if (parameters[0] == ',') return commaAfterOperationError;

    /* starts with a comma*/
    if (lineLen - 1 < 0) {
        printf("hey");
    }
    if (parameters[lineLen - 1] == ',') return commaInEndOfLineError;

    /* check for two conescutive commas*/
    for (i = 1; i < lineLen; i++) {
        if (parameters[i] == parameters[i - 1] && parameters[i] == ',') {
            return twoConsecutiveCommasError;
        }
    }
    /* checks for spaces between two commas, which means two parameters that arent seperated by comma*/
    for (i = 0; i < amountOfSplits; i++) {
        if (strchr(splits[i], ' ')) { /* contains a space*/
            return parametersWithoutSeparatingCommaError;
        }
    }
    sentenceOutput->inputParameters = splits;

    sentenceOutput->amountOfParameters = amountOfSplits;
    return noError;
}

/*
 * create a new line, where all spaces before and after each
 * comma are removed.
 */
char *trimSpacesSurroundingCommas(char *line) {
    int j, *commaPlacement, amountOfCommas = 0, i, lineLen = strlen(line);
    commaPlacement = locateAllChrInFile(line, &amountOfCommas, ',');
    for (i = 0; i < amountOfCommas; i++) {
        j = commaPlacement[i] - 1;
        /* search for spaces before comma, replace each one with '\0'*/
        while (j >= 0) {
            if (line[j] == ' ') {
                line[j] = '\0';
                j--;
            } else break;
        }
        /* search for spaces after comma, replace each one with '\0'*/
        j = commaPlacement[i] + 1;
        while (j < lineLen) {
            if (line[j] == ' ') {
                line[j] = '\0';
                j++;
            } else break;
        }
    }
    return removeAllCertainChr(line, '\0', lineLen);
}
/*
* Checks if the str is a label ending with colon.
* if so, return the name of the label (ex "HELLO")
*/
bool RemoveEndingColon(char *str) {
    char lastChar = str[strlen(str) - 1];
    if (lastChar == ':') {
        str[strlen(str) - 1] = '\0';
        return str;
    } else {
        return NULL;
    }
}


