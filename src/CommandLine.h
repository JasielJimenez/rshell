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
 //int counter; //keeps track of current connector
 public:
 //void setCounter(int num); //sets counter to zero
 //void updateCounter(); //increments counter by one after reading a command
 void split(string comLine); //splits comLine into commands and connectors
 void precedence(string comLine); //Deals with precedence operator
};

class Symbol : public CommandLine {
 public:
 void Reader(vector<string> vecChar,vector<string> connect); //Reads in a connector and calls function
 void semicolon(); //deals with semicolons
 bool ampersand(bool comWorked); //deals with ampersands
 bool doubleLine(bool comWorked); //deals with double lines
};

class Command : public CommandLine {
 public:
 bool run(char** pointChar, int track); //runs commands
 bool run_redirect(char** pointChar);
 bool test(string test);
 //bool precedence(string line);
};

#endif
