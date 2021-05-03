#include "parseComm.h"
#include "builtInCommands.h"
#include "structs.h"
#include "systemCommands.h"
#include "redirectIO.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define INLEN 2048

int main(void) {

    int flag = true;
    char *inputStr = malloc(sizeof(char) * INLEN+1);    // variable to hold command line input
    struct userCommand *inputStruct;

    // variable to store exit status of foreground processes
    int exitStatus = 0;

    // set up head pointer for linked list of child processes
    struct cidLinkedList *head = NULL;

    // variables to hold file descriptors for stdin and stdout for IO redirection
    int stdoutSavedFD;
    int stdinSavedFD;
    // int inRedirSavedFD;
    // int outRedirSavedFD;

    while (flag) {
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

        // check for I/O redirection and set it if user specified
        if (*(inputStruct->outRedir) || *(inputStruct->inRedir)) {
            if (redirectIO(inputStruct, &exitStatus, &stdinSavedFD, &stdoutSavedFD) == -1) {
                continue;
            }
        }

        // check if command is a built in command or "other" command
        if (isBuiltIn(inputStruct->command)) {
            runCommand(inputStruct, head, &exitStatus);
        } else {
            runSysCommand(inputStruct, &exitStatus, &head);
        }

        // check to see if IO needs to be reset for stdin and stdout
        if (*(inputStruct->outRedir) || *(inputStruct->inRedir)) {
            resetIO(inputStruct, &stdinSavedFD, &stdoutSavedFD);
        }
    }

    exit(0);
}