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