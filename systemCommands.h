#include "structs.h"

/*
*   Runs a command that is not built in to smallsh. Forks a child to run the process. Function also
*   responsible for cleaning up zombie processes. Determines if process needs to be run in the
*   background or not.
*/
void runSysCommand(struct userCommand *inputStruct, int *exitStatus, struct cidLinkedList **head);