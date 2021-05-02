#include "parseComm.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

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
*   Expands '$$' into the process id
*/
char *_expandVar(int i, char *expandStr) {
    // copy characters on left side of $$
    char *tempLeftSide = malloc(sizeof(char) * (i+1));
    strncpy(tempLeftSide, expandStr, i);
    tempLeftSide[i+1] = '\0';

    //copy characters on right side of $$
    char *tempRightSide = malloc(sizeof(char) * (strlen(expandStr) - (i+1)));
    strcpy(tempRightSide, expandStr+i+2);

    // convert $$ to the process id
    char *pID = malloc(sizeof(char) * 20);
    sprintf(pID, "%d", getpid());

    // concatenate all the strings together to get the expanded variable
    char *newStr = malloc(sizeof(char) * (strlen(tempLeftSide) + strlen(tempRightSide) + strlen(pID) + 1));
    
    if (i > 0) {
        strcpy(newStr, tempLeftSide);
        strcat(newStr, pID);
    } else {
        strcpy(newStr, pID);
    }
    strcat(newStr, tempRightSide);

    // free up allocated memory
    free(tempLeftSide);
    free(tempRightSide);
    free(pID);
    
    return newStr;
}

/*
*   Check if string contains $$
*/
void _checkExpansion(char *checkStr) {
    for (int i = 0; i < strlen(checkStr); i++) {
        if (checkStr[i] == '$') {
            if (checkStr[i+1] == '$') {
                checkStr = _expandVar(i, checkStr);
                i++;
            }
        }
    }
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
    _checkExpansion(tempStr); // check for $$ string

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
            _checkExpansion(tempStr); // check for $$ string
            userInput->inFile = malloc(sizeof(tempStr));
            strcpy(userInput->inFile, tempStr);
            continue;

        } else if ((strcmp(tempStr, ">\0")) == 0) {
            // user wants output redirection, change flags and add where output
            // will be redirected to in userCommand struct
            *(userInput->outRedir) = true;

            // read in next value to get redirection file
            tempStr = strtok_r(NULL, " ", &savePtr);
            _checkExpansion(tempStr); // check for $$ string
            userInput->outFile = malloc(sizeof(tempStr));
            strcpy(userInput->outFile, tempStr);
            continue;

        } else {
            // check if argument needs variable expansion
            _checkExpansion(tempStr);

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

/*
*   Checks to see if there is a blank line or comment entered by user
*/
bool blankOrComment(char *inputStr) {
    if (inputStr[0] == '\n' || inputStr[0] == '#') {
        return true;
    }
    return false;
}