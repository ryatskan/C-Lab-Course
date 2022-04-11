#include "global.h"
#include "utils.h"

#include "preAssemblerRun.h"
#include "firstPass.h"
#include "secondPass.h"
void processFile(char *fileName);
void writeAmFile(char **lines, char *fileName);
void initData(dataStructures *a, iteratorCapsule *b);

/*
 * Beginning of the program.
 */
int main(int argc, char *argv[]) {
    int i;
    if (argc == 1) {
        printf("Error: program expects at least one parameter.\n");
    }
    for (i = 1; i < argc; i++) {
        processFile(argv[i]);
    }
    return 0;
}
/*
 * Process a single assembly file
 */
void processFile(char *fileName) {

    Error hadError;
    dataStructures a;
    iteratorCapsule b;
    FILE *currFile;
    Sentence *sentences;
    char **lines;
    initData(&a, &b);

    printf("File %s:\n", fileName);
    /* Pre-assembler run*/
    currFile = openFile(fileName, ".as", "r", "files/");
    if (currFile == NULL) {
        printf("Could not open file %s\n", fileName);
    } else {
        lines = preAssemblerRun(currFile);
        writeAmFile(lines, fileName);
        /* run first pass*/
        sentences = my_malloc(sizeof(Sentence) * linesLen(lines));

        hadError = firstPass(lines, &a, &b, sentences);
        if (hadError == noError) {
            /* run the second pass, only if no errors encountered in the fpass*/
            secondPass(&a, &b, lines, sentences, fileName);
        }
         my_free();
    }
}

void initData(dataStructures *a, iteratorCapsule *b) {
    a->codeImg = my_malloc(sizeof(machineWord) * codeImgMaxSize);
    a->dataImg = my_malloc(sizeof(machineWordCapsule) * dataImgMaxSize);
    a->symbolTable = my_malloc(sizeof(table));

    *(a->symbolTable) = my_malloc(sizeof(struct table_entry));
    (*(a->symbolTable))->value = -1;
    b->dc = my_malloc(sizeof(int));
    b->ic = my_malloc(sizeof(int));
    b->amountOfSentences = my_malloc(sizeof(int));
    b->amountOfExOp = my_malloc(sizeof(int));
    b->amountOfCodeCapsules = my_malloc(sizeof(int));
    *b->dc = 0;
    *b->ic = 0;
    *b->amountOfSentences=0;
    *b->amountOfExOp=0;
    *b->amountOfCodeCapsules=0;
}
void writeAmFile(char **lines, char *fileName) {
    linesToFile(lines, fileName, ".am");
}