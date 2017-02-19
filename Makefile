CC = g++
CFLAGS = -Wall -Werror -ansi -pedantic

all:
	mkdir -p ./bin
	$(CC) $(CFLAGS) ${PWD}/src/rshell.cpp ${PWD}/src/CommandLine.cpp -o ${PWD}/bin/rshell

rshell:
	$(CC) $(CFLAGS) ${PWD}/src/rshell.cpp -o ${PWD}/bin/rshell
