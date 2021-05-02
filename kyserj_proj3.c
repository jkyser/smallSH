#include "parseComm.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define INLEN 2048

int main() {

    int flag = true;
    char *inputStr = malloc(sizeof(char) * INLEN+1);    // variable to hold command line input
    struct userCommand *inputStruct;

    while (flag) {
        commPrompt();
        fgets(inputStr, INLEN+1, stdin);    // get user input

        removeNewline(inputStr); // remove newline
        inputStruct = parseStr(inputStr);   // parse input into command and arguments

        printf("%s\n", inputStruct->command);
        printf("%s %s %s\n", inputStruct->argArr[0], inputStruct->argArr[1], inputStruct->argArr[2]);
        printf("%d\n", *(inputStruct->outRedir));
        printf("%d\n", *(inputStruct->inRedir));
        printf("%d\n", *(inputStruct->runBackground));
    }

    exit(0);
}