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

…
/src

CommandLine.cpp

CommandLine.h

hw2

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

Follow these directions to use this program:
---
First, enter this line: git clone https://github.com/JasielJimenez/rshell.git

Then: cd rshell

Then: git checkout hw2

Then: make

Lastly: bin/rshell

Project Overview
---

This program is a command shell that accepts commands from the user and executes them according to the order they are given and what connectors are used.

The “;” connector executes the next command normally.

The “&&” connector executes the next command only if the previous command executes correctly.

The “||” connector executes the next command only if the previous command fails to execute correctly.

Everything entered after the “#” character will be considered a comment and will not be considered a command.

Typing exit will allow the user to quit the program.

The program displays the user’s username and the hostname of the machine the user is logged in from.

Bugs
---
None of the user’s commands can include a % sign. Our code changes all % signs into null characters, thus if the user enters one, the program will crash.

If the only input from the user is “exit,” then the program may not exit. May sometimes require multiple exits.

