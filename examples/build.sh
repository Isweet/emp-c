#!/bin/bash

# Remember to delete /usr/local/include/empc.h and /usr/local/lib/libempc.dylib before running
gcc -L../src -lempc add.c -o add
