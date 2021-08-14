/*
*   builtInCommands.h
*/

#include "structs.h"

#include <stdbool.h>
#include <unistd.h>

/*
*   Checks to see if the user command is a built-in command for smallsh
*/
bool isBuiltIn(char *command);

/*
*   Checks which built in command the user wants to run and runs that specific command
*/
void runCommand (struct userCommand *inputStruct, struct cidLinkedList *head, int *exitStatus);

/*
*   Built in command to show the exit status of the last foreground process; If no foreground
*   processes have been run yet, exit status stays at 0; if process was signal terminated,
*   status will show the signal that terminated the process
*/
void statusCommand (int *exitStatus);