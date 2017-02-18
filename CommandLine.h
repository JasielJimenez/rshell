#ifndef COMMANDLINE_H
#define COMMANDLINE_H

class CommandLine {
 protected:
 string comLine; //stores the line that is passed in
 vector<string> connect; //stores all connectors
 char* com[comLine + 1]; //stores all commands
 int counter; //keeps track of current connector
 public:
 void setCounter(num); //sets counter to zero
 void updateCounter(); //increments counter by one after reading a command
 void Split(comLine); //splits comLine into commands and connectors
};

class Symbol : CommandLine {
 public:
 void Reader(connect); //Reads in a connector and calls appropriate function
 void semicolon(); //deals with semicolons
 void ampersand(); //deals with ampersands
 void doubleLine(); //deals with double lines
 void comment(); // deals with comments
};

class Command : Commandline {
 public:
 Run(com); //runs commands
};

//class Exit : CommandLine {
// public:
// ExitCom(); //exits
//};

#endif