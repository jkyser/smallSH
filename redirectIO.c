/*
*   redirectIO.c
*/

#include "redirectIO.h"
#include "structs.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
*   Redirect I/O based on the parsed user input. On error, sets exitStatus to 1 and returns -1, otherwise
*   returns 0.
*/
int redirectIO (struct userCommand *input, int *exitStatus, int *stdinSavedFD, int *stdoutSavedFD) {
    // set input redirection
    if (*(input->inRedir)) {
        int inFD = open(input->inFile, O_RDONLY);

        // if error, set error status to -1 and don't implement command
        if (inFD == -1) {
            perror(NULL);
            *exitStatus = 1;
            return inFD;
        }

        // set input redirection of stdin; if error return -
        *stdinSavedFD = dup(0);     // store original stdin fd
        int inResult = dup2(inFD, 0);
        if (inResult == -1) {
            perror(NULL);
            *exitStatus = 1;
            return inResult;
        }
    }

    // set output redirection
    if (*(input->outRedir)) {
        int outFD = open(input->outFile, O_WRONLY | O_CREAT | O_TRUNC, 0660);

        // if error, set error status to -1 adn don't implement command
        if (outFD == -1) {
            perror(NULL);
            *exitStatus = 1;
            return outFD;
        }

        // set output redirection of stdout; if error return -1
        *stdoutSavedFD = dup(1);    // store original stdout fd
        int outResult = dup2(outFD, 1);
        if (outResult == -1) {
            perror(NULL);
            *exitStatus = 1;
            return outResult;
        }
    }

    return 0;
}

/*
*   Used to reset stdin and stdout after IO redirection from redirectIO function
*/
void resetIO (struct userCommand *input, int *stdinSavedFD, int *stdoutSavedFD) {
    if (*(input->inRedir)) {
        dup2(*stdinSavedFD, 0);
        close(*stdinSavedFD);
    }
    if (*(input->outRedir)) {
        dup2(*stdoutSavedFD, 1);
        close(*stdoutSavedFD);
    }
}