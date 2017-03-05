RSHELL
========
License Information: BSD-3-Clause
---
Project can be downloaded from:  //github.com/JasielJimenez/rshell.git
---
Authors and Contributors:
---
Jasiel Jimenez

Alexious Jimenez

name1= Jasiel Jimenez  
ucrnetid1= jjime028  
name2= Alexious Jimenez  
ucrnetid2= ajime023  
repourl= //github.com/JasielJimenez/rshell.git  


File List
---
…
.rshell

./src

./tests

LICENSE

Makefile

README.md

hw2

hw3

…
/src

CommandLine.cpp

CommandLine.h

hw2

hw3

rshell.cpp

…
/tests

commented_command.sh

commented_command_test

exit.sh

exit_test

multi_command.sh

multi_command_test

single_command.sh

single_command_test

precedence_test.sh

precedence_test_test

test_test.sh

test_test_test

Follow these directions to use this program:
---
First, enter this line: git clone https://github.com/JasielJimenez/rshell.git

Then: cd rshell

Then: git checkout hw3

Then: make

Lastly: bin/rshell

Project Overview
---

This program is a command shell that accepts commands from the user and executes them according to the order they are given and what connectors are used.

The “;” connector executes the next command normally.

The “&&” connector executes the next command only if the previous command executes correctly.

The “||” connector executes the next command only if the previous command fails to execute correctly.

Everything entered after the “#” character will be considered a comment and will not be considered a command.

The “test” command tests a file and sees if it works when tested with a specified file. If the test is successful, it will return a 0. If it fails, it will return a 1. This will result in the terminal printing out a “True” if the value is 0 and a “False” if the value is 1. It can alternatively be called by replacing “test” with brackets [] around what the user wishes to test. The command should work in conjunction with connecters listed above.

The “test” command can be run with the flags -e, -f, and -d. The -e command checks if the file/directory exists. The -f command checks if the file/directory exists and is a regular file. The -d command checks if the file/directory exists and is a directory.

Using parentheses () around a command or series of commands will change the precedence of the commands.

Typing exit will allow the user to quit the program.

The program displays the user’s username and the hostname of the machine the user is logged in from.

Bugs
---

None of the user’s commands can include a % sign. Our code changes all % signs into null characters, thus if the user enters one, the program will crash.

If the only input from the user is “exit,” then the program may not exit. May sometimes require multiple exits.

If the user inputs a command that isn’t recognized by the execvp, then all following commands will be executed improperly.

If the final command in a multi-command line has no arguments, then the program attempts to use the last command in conjunction with the last argument used in the command line.
Ex:
echo hello && ls
ls: cannot access hello: No such file or directory

mkdir hello && ls
//ls will print nothing since directory hello is empty
