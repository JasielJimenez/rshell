#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

#include "CommandLine.h"

using namespace std;

void CommandLine::setCounter(int num)
{
	counter = num;
}

void CommandLine::updateCounter()
{
	counter++;
}

void CommandLine::split(string comLine)
{
	int num = 0;
	setCounter(num);
	this->comLine = comLine;
	Symbol obj;
	for(unsigned int i = 0; i < comLine.size(); i++) //puts connectors into vector
	{
		if(comLine.at(i) == ';')
		{
			connect.push_back(";");
		}
		else if(comLine.at(i) == '&')
		{
			if(comLine.at(i+1) == '&')
			{
				connect.push_back("&&");
			}
		}
		else if(comLine.at(i) == '|')
		{
			if(comLine.at(i+1) == '|')
			{
				connect.push_back("||");
			}
		}
		else if(i < comLine.size() - 3 && comLine.substr(i,4) == "exit")  //comLine.find("exit")!=std::string::npos)
		{
			connect.push_back("exit");
		}
	}

	
	for(int x = 0; x < connect.size(); x++)
	{
		cout << connect.at(x) << endl;
	}
	cout << endl;
	

int w = 0;
string store;
	char* cstring = new char [comLine.size() + 1];
	strcpy(cstring, comLine.c_str()); 		//converts string into cstring

        vector<string> vecChar;
	//vector<char*> vecChar;
	char* pointer = strtok(cstring," ;||&&");
	char* temp = pointer;
	//char* temp2 = new char [comLine.size() + 1];
	while(temp != NULL)
	{
		string str(temp);		//Converts cstring to string
		str = str + "\0";
		size_t p = str.find_first_not_of(" \t");	 //removes leading whitespace
		str.erase(0,p);
		p = str.find_last_not_of(" \t");
		if(string::npos != p)
		{
			str.erase(p + 1);
		}	
		


	
		if(str == "%")
		 {	
		 	//temp2 = '\0';
//		 	cout << "BLAH BLAH " << endl;
		 	vecChar.push_back("\0");
		 }
		 else
		 {
			//strcpy(temp2, str.c_str()); 		//converts string to cstring

//			cout << str << "!" << endl;

	//		const char * b =  str.c_str();
			vecChar.push_back( str   );

// --------------------------- BLAH BLAH HI -------------------------
//			if(vecChar.size() > 1)
//			{
//				cout << "IN WHILE LOOP" << endl;
//				cout << vecChar.at(0) << " " << vecChar.at(1) << endl;
//				w++;
//			}
// -----------------------------------------------------------------
		}
		temp = strtok(NULL," ;||&&");
	}
		
	//cout << vecChar.at(0) << " " << vecChar.at(1) << endl;
	//temp2 = NULL;
	//vecChar.push_back("\0");
		
	cout << vecChar.size() << endl << endl;
	for (int h = 0; h < vecChar.size(); h++)
	{
		cout << vecChar.at(h) << endl;
	}
	
	//vector<char*> newVec;
	//for(unsigned int z = 0; z < vecChar.size(); z++)
	//{
	//	string tempStr(vecChar.at(z));
	//	int b = 0;
	//	for(unsigned int a = 0; a < vecChar.at(z).size(); a++)
	//	{	
	//		if(tempStr.at(a) == ' ' && b == 0)
	//		{
	//			strcpy(temp2, vecChar.at(z).substr(b,a).c_str());
	//			newVec.push_back(temp2);
	//			b = a + 1;
	//		}
	//		else if(tempStr.at(a) == ' ')
	//		{
	//			strcpy(temp2, vecChar.at(z).substr(b,a - b).c_str());
	//			newVec.push_back(temp2);
	//			b = a + 1;
	//		}
	//		else if(tempStr.at(a) == tempStr.size() - 1)
	//		{
	//			strcpy(temp2, vecChar.at(z).substr(b, a - b).c_str());
	//			newVec.push_back(temp);
	//		}
	//	}
	//	newVec.push_back('\0');
	//}
	//
	//for(unsigned int c = 0; c < newVec.size(); c++)
	//{
	//	cout << newVec.at(c) << endl;
	//	//convert all strings in newVec to cstrings
	//}


	//CONVERT STRING TO CHAR*

	int pCharSize = vecChar.size() + 1;
	char** pointChar = new char*[vecChar.size()+1];
	for(unsigned int y = 0; y < vecChar.size(); y++) //converts vector to char**
	{
		string st_temp = vecChar[y];
		char * blah = (char *)st_temp.c_str();
		pointChar[y] = blah;

	}	
	pointChar[vecChar.size()] = '\0';
	
	obj.Reader(pointChar,pCharSize,connect); //calls the reader function
	
	//ipointChar[vecChar.size()] = NULL;	

	for(unsigned int u = 0; u < connect.size(); u++) //clears connect vector
	{
		connect.pop_back();
	}

	//delete cstring;
	//delete temp2;

	//char* pointer = strtok(cstring, ";||&&");
	//char** holdPoint = 
	//while(pointer != 0)
	//{
	//	cout << pointer << endl;
	//	
	//	obj.Reader(pointer,connect);
	//
	//	pointer = strtok(NULL,";||&&");
	//}

	//delete cstring;
}

void Symbol::Reader(char** pointChar, int pCharSize, vector<string> connect)
{
	bool comWorked = false;
	Command temp;
	//for(int x = 0; x < pCharSize; x++)
	//{
		comWorked = temp.run(pointChar);
		//cout << comWorked << endl;
		//cout << "comWorked up (1 is true, 0 is false)" << endl;
		if(connect.size() > 0) //goes through here only if there are connectors
		{
			string currRead = connect.at(counter);
			//cout << currRead << endl;
			//cout << "currRead up" << endl;
			if(currRead == ";")
			{
				semicolon();
			}
			else if(currRead == "&&")
			{
				ampersand(comWorked);
			}
			else if(currRead == "||")
			{
				doubleLine(comWorked);
			}
			else if(currRead == "exit")
			{
				exit(0);
			}
			updateCounter();
		}
	//}
}

void Symbol::semicolon()
{
	return;		//implement next command
}

void Symbol::ampersand(bool comWorked) //implement next command only if last command succeeded
{
	if(comWorked == false)
	{
		exit(0);
	}	
}

void Symbol::doubleLine(bool comWorked) //implement next command only if last command failed
{
	if(comWorked == true)
	{
		exit(0);
	}
}

bool Command::run(char** pointChar) //run commands correctly
{	
	//char **pointer = malloc(bufsize * sizeof(pointer*));
	//char *token;
	pid_t pid;
	//pid_t waitPid;	
	int temp;	
	
	//if (!tokens) {
		//fprintf(stderr, "lsh: allocation error\n");
		//exit(0);
	//}
	
	pid = fork();
	//cout << pid << endl;
	if(pid == 0)
	{
		if(execvp(*pointChar,pointChar) == -1)
		{
			perror("execvp error");
			return false;
		}
	}
	else if(pid < 0)
	{
		perror("fork error");
		return false;
	}
	else
	{
		//waitPid = waitpid(pid, &temp, WUNTRACED);
		//while (!WIFEXITED(temp) && !WIFSIGNALED(temp));
		if (waitpid(pid, &temp, 0) == -1)
		{
			perror("waitpid error");
			exit(0);
		}
	}
	return true;
}
