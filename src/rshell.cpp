#include<iostream>
#include<vector>
#include<string>
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
 string cLine;
 char* name1;
 name1 = getlogin();

 char name2[200];
 size_t size = 199;
 gethostname(name2,size); //extra credit

 while(1)
 {
	CommandLine object;
	cout << name1 << "@" << name2  << "$ BLAH "; //prints login and host info	//MAKE SURE TO FIX
	getline(cin, cLine);

	if(cLine == "exit") //checks to see if only exit is inputted
        {
                return 0;
        }


	for(unsigned int i = 0; i < cLine.size(); i++)	//checks for comment symbol
	{
		if (cLine.at(i) == '#')
		{
			cLine = cLine.substr(0,i);	//removes everyhing after the comment symbol
		}
	}

	int g = cLine.size() - 1;
	int e = 0;
	string newStr = cLine;
	for(unsigned int j = 0; j < g; j++)	//Adds % character to separate commands
	{
		if (cLine.at(j) == ';' || (cLine.at(j) == '&' && cLine.at(j+1) == '&') || (cLine.at(j) == '|' && cLine.at(j+1) == '|'))
		{
			newStr = newStr.substr(0,j + e);
			newStr = newStr + " %";
			newStr = newStr + cLine.substr(j, cLine.size() - j);
			//cLine = newStr;
			//e = j + 2;
			cout << newStr << endl;
			e = e + 2;
		}
	}
	
	
	object.split(newStr);	//splits the string into tokens

	cLine = ""; //Clears string to take in next command
 }
 return 0;
}