/*
*   builtInCommands.c
*/

#include "builtInCommands.h"
#include "structs.h"

#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

/*
*   Checks to see if the user command is a built-in command for smallsh
*/
bool isBuiltIn(char *command) {
    int test1 = strcmp(command, "exit");
    int test2 = strcmp(command, "cd");
    int test3 = strcmp(command, "status");

    if (test1 == 0 || test2 == 0 || test3 == 0) {
        return true;
    }
    return false;
}

/*
*   Runs the exit command. If there are any child processes, first kills all of these processes
*   by sending the SIGKILL signal, then kills the smallsh process last
*/
void _exitCommand (struct cidLinkedList *head) {
    if (head != NULL) {
        char *cidStr = malloc(sizeof(char) * 20);

        // kill child processes by iterating through linked list
        struct cidLinkedList *prev = head;
        struct cidLinkedList *next = prev->next;
        while (prev != NULL) {
            sprintf(cidStr, "%d", *(prev->cid));
            execlp("kill", "kill", "-SIGKILL", cidStr, NULL);
            prev = next;
            next = next->next;
        }
    }

    // kill smallsh
    exit(0);
}

/*
*   Built in command to change the directory; cd with no arguments goes to the home directory;
*   cd with arguments goes to the path specified (can be absolute or relative)
*/
void _cdCommand (struct userCommand *inputStruct) {
    if (inputStruct->argArr[1] == NULL) {
        // change to environment HOME var if no path specified
        chdir(getenv("HOME"));
    } else {
        // handle the input argument whether it is relative or not
        // loop through until you have relative path
        char *savePtr;
        char *relPath = strtok_r(inputStruct->argArr[1], "/", &savePtr);
        char *nextDir = strtok_r(NULL, "/", &savePtr);

        while (nextDir != NULL) {
            relPath = nextDir;
            nextDir = strtok_r(NULL, "/", &savePtr);
        }

        // change directory to relative path
        chdir(relPath);
    }
}

/*
*   Built in command to show the exit status of the last foreground process; If no foreground
*   processes have been run yet, exit status stays at 0; if process was signal terminated,
*   status will show the signal that terminated the process
*/
void statusCommand (int *exitStatus) {
    if (WIFEXITED(*exitStatus)) {
        printf("exit value %d\n", WEXITSTATUS(*exitStatus));
        fflush(stdout);
    } else if (WIFSIGNALED(*exitStatus)) {
        printf("terminated by signal %d\n", WTERMSIG(*exitStatus));
        fflush(stdout);
    } else {
        printf("Oops something went wrong!\n");
        fflush(stdout);
    }
}

/*
*   Checks which built in command the user wants to run
*/
void runCommand (struct userCommand *inputStruct, struct cidLinkedList *head, int *exitStatus) {
    int test1 = strcmp(inputStruct->command, "exit");
    int test2 = strcmp(inputStruct->command, "cd");
    int test3 = strcmp(inputStruct->command, "status");

    if (test1 == 0) {
        _exitCommand(head);
    } else if (test2 == 0) {
        _cdCommand(inputStruct);
    } else if (test3 == 0) {
        statusCommand(exitStatus);
    } else {
        printf("Oops, something went wrong");
        fflush(stdout);
        exit(1);
    }
}