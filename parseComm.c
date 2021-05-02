#include "parseComm.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/*
* Removes the newline from the user inputted command and returns that string
*/
void removeNewline(char *userIn) {
    userIn[strlen(userIn)-1] = '\0';
    return;
}

/*
* Writes ": " as the command prompt to the user
*   write to fd = 1 in case stdout is changed at some point
*/
void commPrompt() {
    write(1, ": ", 2);
}

/*
*   Parse the user input string into the command and its arguments. Take in to account the
*   special characters '<', '>', '&' for I/O redirection and running in background
*/
struct userCommand *parseStr(char *inputStr) {
    // create struct to store user input
    struct userCommand *userInput = malloc(sizeof(struct userCommand));

    // set userInput redirection and background booleans to false
    userInput->outRedir = malloc(sizeof(bool));
    userInput->inRedir = malloc(sizeof(bool));
    userInput->runBackground = malloc(sizeof(bool));
    *(userInput->outRedir) = false;
    *(userInput->inRedir) = false;
    *(userInput->runBackground) = false;

    // get command input by user
    char *savePtr;
    char *tempStr = strtok_r(inputStr, " ", &savePtr);

    //store command input in struct
    userInput->command = malloc(sizeof(tempStr));
    strcpy(userInput->command, tempStr);

    int i = 0;
    // loop through command input, parsing argument
    while((tempStr = strtok_r(NULL, " ", &savePtr)) != NULL) {
        if ((strcmp(tempStr, "&\0")) == 0) {
            // check if user wants to run process in background
            tempStr = strtok_r(NULL, " ", &savePtr);
            if (tempStr == NULL) {
                // if '&' character was the last argument then run process in background
                *(userInput->runBackground) = true;
                break;
            } else {
                // if '&' character was not end of arguments, then add to argArr and move on
                // to next argument
                userInput->argArr[i] = malloc(sizeof("&"));
                strcpy(userInput->argArr[i], "&");
                i++;
            }
        }
        
        if ((strcmp(tempStr, "<\0")) == 0) {
            // user wants input redirection, change flags and add where input
            // will be redirected to in userCommand struct
            *(userInput->inRedir) = true;

            // read in next value to get redirection file
            tempStr = strtok_r(NULL, " ", &savePtr);
            userInput->inFile = malloc(sizeof(tempStr));
            strcpy(userInput->inFile, tempStr);
            continue;

        } else if ((strcmp(tempStr, ">\0")) == 0) {
            // user wants output redirection, change flags and add where output
            // will be redirected to in userCommand struct
            *(userInput->outRedir) = true;

            // read in next value to get redirection file
            tempStr = strtok_r(NULL, " ", &savePtr);
            userInput->outFile = malloc(sizeof(tempStr));
            strcpy(userInput->outFile, tempStr);
            continue;

        } else {
            // tempStr is an argument for the command so add it to the arg array in the
            // user command struct
            userInput->argArr[i] = malloc(sizeof(tempStr));
            strcpy(userInput->argArr[i], tempStr);
            i++;
        }
    }

    // null terminate argument array
    userInput->argArr[i] = NULL;

    return userInput;
}