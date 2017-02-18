#include<iostream>
#include<vector>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>

#include "CommandLine.h"

using namespace std;

int main(int argc, char** argv)
{
 CommandLine object;
 string cLine;
 char* name1;
 name1 = getlogin();

 char* name2;
 size_t size = 0;
 gethostname(name2,size);	//FIX THIS

 while(1)
 {
	cout << name1 << "@" << name2 << "$ ";
	getline(cin, cLine);

	if(cLine == "exit") //checks to see if only exit is inputted
        {
                return 0;
        }


	for(int i = 0; i < cLine.size(); i++)	//checks for comment symbol
	{
		if (cLine.at(i) == '#')
		{
			cLine = cLine.substr(0,i);	//removes everyhing after the comment symbol
		}
	}
	
	cout << cLine << endl;
	
	object.split(cLine);	//splits the string into tokens

	cLine = ""; //Clears string to take in next command
 }
 return 0;
}
