#include "structs.h"

#include <stdbool.h>

/*
*   Removes the newline from the user inputted command and returns that string
*/
void removeNewline(char *userIn);

/*
*   Writes ": " as the command prompt to the user
*   write to fd = 1 in case stdout is changed at some point
*/
void commPrompt();

/*
*   Parse the user input string into the command and its arguments. Take in to account the
*   special characters '<', '>', '&' for I/O redirection and running in background
*/
struct userCommand *parseStr(char *inputStr, bool foregroundMode);

/*
*   Checks to see if there is a blank line entered by user
*/
bool blankOrComment(char *inputStr);