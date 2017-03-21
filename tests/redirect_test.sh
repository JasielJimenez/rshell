#!/bin/sh
cat < existingInputFile | tr A-Z a-z | tee newOutputFile1 | tr a-z A-Z > newOutputFile2
echo hello > boop
echo goodbye >> boop
cat < rshell.cpp && echo hello
cat < testME
cat < testME | tr a-z A-Z > bork
echo hello | tr a-z A-Z > oFile
ls -a && cat < testFile < store && echo hi || echo what
ls -a > beep
echo File >> beep
cat < testFile >> beep
cat < testFile | tr a-z A-Z | tee yup | tr A-Z a-z >> beep
(echo hi && echo hello > geez) || ls -a
ls -a | tee zip
ls -a | tr a-z A-Z | tee zap
touch hello; echo hello >> hello
mkdir directory && ls -a > lsFile || ls -a
echo boo; ls -a && touch file || mkdir direc; cat < testFile > newFile
cat < rshell.cpp | tee boo || echo fail
test -f notFile && cat < testFile | tr a-z A-Z > output

