/*
*   smallsh
*
*   Josh Kyser
*   CS344
*   Project 3
*
*   Implementation of a small shell with the built-in commands cd, exit, and status. Can also use
*   non-built in commands by forking child processes and using the exec() family of functions
*   to call from the PATH environment variables. Smallsh also handles IO redirection, parsing
*   input from the user, variable expansion on $$, and handling certain signals. CTRL-C will
*   be ignored by parent process and children processes being run in the background, but will
*   terminate processes running in the foreground. CTRL-Z will cause the program too enter
*   foreground only mode, where no processes will be able to be run in the background (despite
*   user input).
*/

#include "parseComm.h"
#include "builtInCommands.h"
#include "structs.h"
#include "systemCommands.h"
#include "redirectIO.h"
#include "signalHandling.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#define INLEN 2048

bool foregroundMode = false;

int main(void) {

    char *inputStr = malloc(sizeof(char) * INLEN+1);    // variable to hold command line input
    struct userCommand *inputStruct;

    // variable to store exit status of foreground processes
    int exitStatus = 0;

    // set up head pointer for linked list of child processes
    struct cidLinkedList *head = NULL;

    // variables to hold file descriptors for stdin and stdout for IO redirection
    int stdoutSavedFD;
    int stdinSavedFD;

    // register signal handlers for SIGINT and SIGTSTP
    struct sigaction SIGINTaction = ignoreStructSIGINT();
    sigaction(SIGINT, &SIGINTaction, NULL);
    struct sigaction SIGTSTPaction = ignoreStructSIGTSTP();
    sigaction(SIGTSTP, &SIGTSTPaction, NULL);

    // boolean value to determine if background processes can be run
    bool backgroundDisable = false;

    while (true) {
        // get user input
        commPrompt();
        fgets(inputStr, INLEN+1, stdin);

        // check for blank line or comment line
        if (blankOrComment(inputStr)) {
            continue;
        }

        // parse input into command and arguments
        removeNewline(inputStr);
        inputStruct = parseStr(inputStr, foregroundMode);

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