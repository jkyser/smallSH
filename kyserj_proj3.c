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

        // check for blank line or comment line
        if (blankOrComment(inputStr)) {
            continue;
        }

        removeNewline(inputStr); // remove newline
        inputStruct = parseStr(inputStr);   // parse input into command and arguments
    }

    exit(0);
}