# SmallSH
---
This is a shell implementation written purely in C. It is a close mimic to a regular bash shell. SmallSH has a lot of basic features (as seen below) 
including foreground/background processes, I/O redirection, and signal processing.

## How to Run
---
compile program with following gcc command:
`gcc --std=gnu99 -O3 -g -o smallsh kyserj_proj3.c parseComm.c builtInCommands.c systemCommands.c redirectIO.c signalHandling.c`

can optionally be compiled with the python script:
`python3 script_proj3.py`

run the program with the following command from a terminal
`smallsh`

## About the Project
---
The following is a list of features that were coded for SmallSH


- Provides a prompt for running commands
- Handles blank lines and comments (lines starting with '#')

![prompt_comments](https://github.com/jkyser/smallSH/blob/main/images/prompt_comments.jpg)


- Provides expansion for the variable $$ to the process ID of smallSH itself

![variable_expansion](https://github.com/jkyser/smallSH/blob/main/images/variable_expansion.jpg)


- Built-in commands

- *exit* – exits the smallSH shell and kills any jobs or processes the shell has started before terminating itself

![exit](https://github.com/jkyser/smallSH/blob/main/images/exit.jpg)


- *cd* – changes the working directory of smallSH
    - no arguments – changes to directory specified in HOME variable
    - one argument – path of a directory to change to (relative or absolute)

![cd](https://github.com/jkyser/smallSH/blob/main/images/cd.jpg)


- *status* – prints out either the exit status or the terminating signal of the last foreground process ran by smallSH

![status](https://github.com/jkyser/smallSH/blob/main/images/status.jpg)


- Execution of commands other than built-in commands by forking a child process

![other_commands](https://github.com/jkyser/smallSH/blob/main/images/other_commands.jpg)


- Handles I/O redirection

![io_redirection](https://github.com/jkyser/smallSH/blob/main/images/io_redirection.jpg)


- Running commands in either the foreground or the background

![foreground_process](https://github.com/jkyser/smallSH/blob/main/images/foreground_process.jpg)
![background_process](https://github.com/jkyser/smallSH/blob/main/images/background_process.jpg)


- Handles SIGINT and SIGSTP signals

- *SIGINT* (CTRL-C) – kills foreground running processes (does not affect background running processes)

![sigint](https://github.com/jkyser/smallSH/blob/main/images/sigint.jpg)


- *SIGSTP* (CTRL-Z) – enters a foreground only state (does not allow processes to be ran in the background, forces all processes to foreground (ignores &))

![sigstp-1](https://github.com/jkyser/smallSH/blob/main/images/sigstp-1.jpg)
![sigstp-2](https://github.com/jkyser/smallSH/blob/main/images/sigstp-2.jpg)
