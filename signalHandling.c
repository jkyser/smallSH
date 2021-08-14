/* 
*   signalHandling.c
*/

#include "structs.h"
#include "signalHandling.h"

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>

/*
*   Function to register a signal handler to ignore the SIGINT signal. Returns the struct
*   for main to register the handler
*/
struct sigaction ignoreStructSIGINT () {
    // initialize sigaction struct
    struct sigaction SIGINTaction = {0};

    // set the signal action handler function to the macro to ignore the signal
    SIGINTaction.sa_handler = SIG_IGN;

    // initialize the set of signals to ignore (though none will be ignored)
    sigemptyset(&SIGINTaction.sa_mask);

    // set flag automatically restart any interrupted functions
    SIGINTaction.sa_flags = SA_RESTART;

    return SIGINTaction;
}

/*
*   Changes the boolean value on whether foreground mode should be enabled or disabled
*/
void _SIGTSTPhandler () {
    if (foregroundMode) {
        foregroundMode = false;

        // tell user foreground only mode disabled
        printf("\nExiting foreground-only mode\n: ");
        fflush(stdout);

    } else {
        foregroundMode = true;

        // tell user foreground only mode enabled
        printf("\nEntering foreground-only mode (& is now ignored)\n: ");
        fflush(stdout);
    }
}

/*
*   Function to register a signal handler to ignore the SIGTSTP signal. Returns the struct
*   for main to register the handler
*/
struct sigaction ignoreStructSIGTSTP () {
    // initialize sigaction struct
    struct sigaction SIGTSTPaction = {0};

    // set the signal action handler function to handle non-background mode
    SIGTSTPaction.sa_handler = _SIGTSTPhandler;

    // initialize set of signals to be ignored during execution
    sigfillset(&SIGTSTPaction.sa_mask);

    // set flag to automatically restart any interrupted functions
    SIGTSTPaction.sa_flags = SA_RESTART;

    return SIGTSTPaction;
}

/*
*   Restore default settings to SIGINT handler for foreground children processes
*/
struct sigaction childSIGINThandler () {
    // initialize sigaction struct
    struct sigaction childSIGINTstruct = {0};

    // set to default behavior
    childSIGINTstruct.sa_handler = SIG_DFL;

    // ignore other flags
    sigfillset(&childSIGINTstruct.sa_mask);

    // set flags to none
    childSIGINTstruct.sa_flags = SA_RESTART;

    return childSIGINTstruct;
}

/*
*   Set all children to ignore the SIGTSTP signal
*/
struct sigaction childSIGTSTPhandler () {
    // initialize sigaction struct
    struct sigaction childSIGTSTPstruct = {0};

    // set to ignore signal
    childSIGTSTPstruct.sa_handler = SIG_IGN;

    // ignore other flags
    sigfillset(&childSIGTSTPstruct.sa_mask);

    // set flags restart anything that was running
    childSIGTSTPstruct.sa_flags = SA_RESTART;

    return childSIGTSTPstruct;
}