#!/bin/sh
cat < existingInputFile | tr A-Z a-z | tee newOutputFile1 | tr a-z A-Z > newOutputFile2
echo hello > boop
echo goodbye >> boop
cat < rshell.cpp && echo hello
