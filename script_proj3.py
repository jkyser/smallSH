import os

compile = "gcc --std=gnu99 -O3 -g -o proj3 kyserj_proj3.c parseComm.c builtInCommands.c"

os.system(compile)