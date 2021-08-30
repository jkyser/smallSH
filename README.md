# SmallSH
---
This is a shell implementation written purely in C. It is a close mimic to a regular bash shell. SmallSH has a lot of basic features (as seen below) 
including foreground/background processes, I/O redirection, and signal processing.

## How to Run
---
compile program with following gcc command:
`gcc --std=gnu99 -O3 -g -o smallsh kyserj_proj3.c parseComm.c builtInCommands.c systemCommands.c redirectIO.c signalHandling.c`

run the program with the following command from a terminal
`smallsh`

## About the Project
---
The following is a list of features that were coded for SmallSH


- Provides a prompt for running commands
- Handles blank lines and comments (lines starting with '#')

![prompt_comments]()


- Provides expansion for the variable $$ to the process ID of smallSH itself

![variable_expansion]()


- Built-in commands

- *exit* – exits the smallSH shell and kills any jobs or processes the shell has started before terminating itself

![exit]()


- *cd* – changes the working directory of smallSH
    - no arguments – changes to directory specified in HOME variable
    - one argument – path of a directory to change to (relative or absolute)

![cd]()


- *status* – prints out either the exit status or the terminating signal of the last foreground process ran by smallSH

![status]()


- Execution of commands other than built-in commands by forking a child process

![other_commands]()


- Handles I/O redirection

![io_redirection]()


- Running commands in either the foreground or the background

![foreground_process]()
![background_process]()


- Handles SIGINT and SIGSTP signals

- *SIGINT* (CTRL-C) – kills foreground running processes (does not affect background running processes)

![sigint]()


- *SIGSTP* (CTRL-Z) – enters a foreground only state (does not allow processes to be ran in the background, forces all processes to foreground (ignores &))

![sigstp-1]()
![sigstp-2]()