import os

compile = "gcc --std=gnu99 -O3 -g -o smallsh kyserj_proj3.c parseComm.c builtInCommands.c systemCommands.c"

os.system(compile)