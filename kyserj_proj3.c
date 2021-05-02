#include "parseComm.h"
#include "builtInCommands.h"
#include "structs.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define INLEN 2048

int main(void) {

    int flag = true;
    char *inputStr = malloc(sizeof(char) * INLEN+1);    // variable to hold command line input
    struct userCommand *inputStruct;

    while (flag) {
        // set up head pointer for linked list of child processes
        struct cidLinkedList *head = NULL;

        // variable to store exit status of foreground processes
        int exitStatus = 0;

        // get user input
        commPrompt();
        fgets(inputStr, INLEN+1, stdin);

        // check for blank line or comment line
        if (blankOrComment(inputStr)) {
            continue;
        }

        // parse input into command and arguments
        removeNewline(inputStr);
        inputStruct = parseStr(inputStr);

        // check if command is a built in command or "other" command
        if (isBuiltIn(inputStruct->command)) {
            runCommand(inputStruct, head, &exitStatus);
        } else {
            exit(0);
        }
    }

    exit(0);
}