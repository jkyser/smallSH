#include "structs.h"
#include "systemCommands.h"
#include "builtInCommands.h"

#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

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
*
*/
void runSysCommand (struct userCommand *inputStruct, int *exitStatus, struct cidLinkedList **head) {
    pid_t spawnPID = fork();
    switch (spawnPID) {
        case -1:
            // add something here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            break;
        case 0:
            // check where to redirect I/O!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
                // child process runs in the foreground
                waitpid(spawnPID, exitStatus, 0);

                // check for any completed zombie processes
                _processCompleteCheck(head);
            }
            break;
    }
}