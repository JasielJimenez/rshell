#include<iostream>
#include "CommandLine.h"

int main(int argc, char** argv)
{
 CommandLine object;
 string input;
 while(1)
 {
	cout << "$ ";
	cin input;

	if(input == "exit")
        {
                return 0;
        }


	object.split(input);


	input = ""; //Clears string to take in next command
 }
 return 0;
}
