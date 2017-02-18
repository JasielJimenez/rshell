CC = g++
CFLAGS = -Wall -Werror -ansi -pedantic

all:
	mkdir -p ./bin
	$(CC) $(CFLAGS) ./src/main.cpp -o ./bin/rshell

rshell:
	$(CC) $(CFLAGS) ./src/rshell.cpp

CommandLine:
	$(CC) $(CFLAGS) ./src/CommandLine.cpp
