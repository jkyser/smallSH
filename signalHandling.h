#include <signal.h>
#include <stdbool.h>

extern bool foregroundMode;

/*
*   Function to register a signal handler to ignore the SIGINT signal. Returns the struct
*   for main to register the handler
*/
struct sigaction ignoreStructSIGINT ();

/*
*   Function to register a signal handler to ignore the SIGTSTP signal. Returns the struct
*   for main to register the handler
*/
struct sigaction ignoreStructSIGTSTP ();

/*
*   Restore default settings to SIGINT handler for foreground children processes
*/
struct sigaction childSIGINThandler ();

/*
*   Set all children to ignore the SIGTSTP signal
*/
struct sigaction childSIGTSTPhandler ();