/*
*   redirectIO.h
*/

#include "structs.h"

/*
*   Redirect I/O based on the parsed user input. On error, sets exitStatus to 1 and returns -1, otherwise
*   returns 0.
*/
int redirectIO (struct userCommand *input, int *exitStatus, int *stdinSavedFD, int *stdoutSavedFD);

/*
*   Used to reset stdin and stdout after IO redirection from redirectIO function
*/
void resetIO (struct userCommand *input, int *stdinSavedFD, int *stdoutSavedFD);