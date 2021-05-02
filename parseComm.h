#include <stdbool.h>

// struct to track the command user inputted, any additional arguments provided for the  command,
// and if the user wants the command I/O redirected or run in the background
struct userCommand {
    char *command;
    char *argArr[512];
    bool *outRedir;
    char *outFile;  // where to redirect output
    bool *inRedir;
    char *inFile;   // where to redirect input
    bool *runBackground;
};

/*
* Removes the newline from the user inputted command and returns that string
*/
void removeNewline(char *userIn);

/*
* Writes ": " as the command prompt to the user
*   write to fd = 1 in case stdout is changed at some point
*/
void commPrompt();

/*
*
*/
struct userCommand *parseStr(char *inputStr);