#include <unistd.h>
#include <stdbool.h>

#ifndef CID_LINKED_LIST
#define CID_LINKED_LIST

// struct for the nodes of the linked list for the child processes
struct cidLinkedList {
    pid_t *cid;
    struct cidLinkedList *next;
};

#endif

#ifndef USER_INPUT_STRUCT
#define USER_INPUT_STRUCT

// struct to track the command user inputted, any additional arguments provided for the  command,
// and if the user wants the command I/O redirected or run in the background
struct userCommand {
    char *command;
    char *argArr[514];
    bool *outRedir;
    char *outFile;  // where to redirect output
    bool *inRedir;
    char *inFile;   // where to redirect input
    bool *runBackground;
};

#endif