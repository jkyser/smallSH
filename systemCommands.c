/*
*   systemCommands.c
*/

#include "structs.h"
#include "systemCommands.h"
#include "builtInCommands.h"
#include "signalHandling.h"

#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

/*
*   Adds a child process node to the cidLinkedList to track children processes
*/
void _addChildNode (struct cidLinkedList **head, pid_t childID) {
    struct cidLinkedList *childNode = malloc(sizeof(struct cidLinkedList));
    childNode->cid = malloc(sizeof(childID));
    *(childNode->cid) = childID;
    
    if (*head == NULL) {
        // this is the first process in the linked list
        childNode->next = NULL;
    } else {
        // add to the front of the linked list to keep insertions O(1)
        childNode->next = *head;
    }

    // set new head of linked list
    *head = childNode;
}

/*
*   Prints out which background process completed and the termination status of that process
*/
void _backProcComplete (struct cidLinkedList *completedProc, int status) {
    printf("background pid %d is done: ", *(completedProc->cid));
    fflush(stdout);
    statusCommand(&status);  // reusing code from builtInCommands header because it prints the correctly formatted thing here
}

/*
*   Removes a child process node from the cid linked list
*/
void _removeCompleteProc(struct cidLinkedList **curr, struct cidLinkedList **prev, struct cidLinkedList **head) {
    // if prev is null, just need to change head pointer and free previous linked list head
    if (*prev == NULL) {
        *head = (*curr)->next;
        *prev = *curr;
        *curr = (*curr)->next;
        free(*prev);
        *prev = NULL;
    } else {
        // middle of linked list, need to free node and change pointers
        (*prev)->next = (*curr)->next;
        free(*curr);
        *curr = (*prev)->next;
    }
}

/*
*   Iterates through the cid linked list to check if there are any zombie processes to reap.
*   If functions finds a zombie, outputs completion status to terminal with call to
*   _backProcComplete function and removes node referring to child process in the cid
*   linked list
*/
void _processCompleteCheck (struct cidLinkedList **head) {
    // if head is null, there are no background processes to check
    if (*head == NULL) {
        return;
    }

    // loop through cid linked list to see if there are any zombie processes to reap
    struct cidLinkedList *curr = *head;
    struct cidLinkedList *prev = NULL;
    int backgroundStatus;
    while (curr != NULL) {
        if (waitpid(*(curr->cid), &backgroundStatus, WNOHANG) > 0) {
            // output completion to terminal
            _backProcComplete(curr, backgroundStatus);

            // remove completed process from cid linked list
            _removeCompleteProc(&curr, &prev, head);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

/*
*   Check if child background process needs IO redirected to /dev/null
*/
void _checkChildIO(struct userCommand *inputStruct) {
    // if no input redirection, set to /dev/null
    if (!(*(inputStruct->inRedir))) {
        int inFD = open("/dev/null", O_RDONLY);
        dup2(inFD, 0);
    }

    // if no output redirection, set to /dev/null
    if (!(*(inputStruct->outRedir))) {
        int outFD = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0660);
        dup2(outFD, 1);
    }
}

/*
*   Checks if foreground child process was terminated by SIGINT signal
*/
void _checkSigTermination (int exitStatus) {
    if (WIFSIGNALED(exitStatus)) {
        printf("terminated by signal %d\n", WTERMSIG(exitStatus));
        fflush(stdout);
    }
}

/*
*   Runs a command that is not built in to smallsh. Forks a child to run the process. Function also
*   responsible for cleaning up zombie processes. Determines if process needs to be run in the
*   background or not.
*/
void runSysCommand (struct userCommand *inputStruct, int *exitStatus, struct cidLinkedList **head) {
    pid_t spawnPID = fork();
    switch (spawnPID) {
        case -1:
            printf("Forking the child process went wrong, please try again!");
            fflush(stdout);
            *exitStatus = 1;
            break;
        case 0:
            // check if IO needs to be redirected to /dev/null
            if (*(inputStruct->runBackground)) {
                _checkChildIO(inputStruct);
            }

            // check if child is running in foreground; if true, signal handler
            // needs to be reset to be specific for foreground child
            if (!(*(inputStruct->runBackground))) {
                struct sigaction childSIGINTstruct = childSIGINThandler();
                sigaction(SIGINT, &childSIGINTstruct, NULL);
            }
            struct sigaction childSIGTSTPstruct = childSIGTSTPhandler();
            sigaction(SIGTSTP, &childSIGTSTPstruct, NULL);

            // child process will run the command that is not built in to smallsh
            execvp(inputStruct->command, inputStruct->argArr);

            // execute on execvp error
            perror(inputStruct->command);
            *exitStatus = 1;
            exit(1);
            break;
        default:
            // check if child process should be run in background
            if (*(inputStruct->runBackground)) {
                printf("background pid is %d\n", spawnPID);
                fflush(stdout);

                // add child process to linked list to track children processes
                _addChildNode(head, spawnPID);

                // check for any completed zombie processes
                _processCompleteCheck(head);

            } else {
                // check for any completed zombie processes
                _processCompleteCheck(head);

                // child process runs in the foreground
                waitpid(spawnPID, exitStatus, 0);

                // check if foreground process was interrupted by SIGINT
                // if it was, print a message saying it was interrupted
                _checkSigTermination(*exitStatus);
            }
            break;
    }
}