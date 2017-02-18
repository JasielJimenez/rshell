#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include<iostream>
#include<vector>
using namespace std;

class CommandLine {
 protected:
 string comLine; //stores the line that is passed in
 vector<string> connect; //stores all connectors
 vector<string> com; //stores all commands
 int counter; //keeps track of current connector
 public:
 void setCounter(int num); //sets counter to zero
 void updateCounter(); //increments counter by one after reading a command
 void split(string comLine); //splits comLine into commands and connectors
};

class Symbol : public CommandLine {
 public:
 void Reader(char** pointChar,int pCharSize,vector<string> connect); //Reads in a connector and calls function
 void semicolon(); //deals with semicolons
 void ampersand(bool comWorked); //deals with ampersands
 void doubleLine(bool comWorked); //deals with double lines
};

class Command : public CommandLine {
 public:
 bool run(char** pointChar); //runs commands
};

//class Exit : CommandLine {
// public:
// ExitCom(); //exits
//};

#endif
