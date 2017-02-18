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
	for(int i = 0; i < comLine.size(); i++)
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

	char* cstring = new char [comLine.size() + 1];
	strcpy(cstring, comLine.c_str());



	vector<char*> vecChar;
	char* pointer = strtok(cstring,";||&&");
	char* temp = pointer;
	char* temp2 = new char [comLine.size() + 1];
	while(temp != NULL)
	{
		string str(temp);

		//cout << str << endl;		

		strcpy(temp2, str.c_str());

		//cout << temp2 << endl;

		vecChar.push_back(temp);

		//cout << temp << endl;				

		temp = strtok(NULL,";||&&");
	}
	
	char** pointChar = new char*[vecChar.size()+1];
	for(int y = 0; y < vecChar.size(); y++)
	{
		pointChar[y] = vecChar[y];
	}
	
	//obj.Reader(pointChar,connect);

	pointChar[vecChar.size()] = NULL;
	
	for(int u = 0; u < connect.size(); u++)
	{
		connect.pop_back();
	}

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

void Symbol::Reader(char** pointChar, vector<string> connect)
{
	bool comWorked = true;
	Command temp;
	comWorked = temp.run(pointChar);
	string currRead = connect.at(counter);
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

void Symbol::semicolon()
{
	return;
}

void Symbol::ampersand(bool comWorked)
{
	if(comWorked == false)
	{
		exit(0);
	}	
}

void Symbol::doubleLine(bool comWorked)
{
	if(comWorked == true)
	{
		exit(0);
	}
}

bool Command::run(char** pointChar)
{	
	//char **pointer = malloc(bufsize * sizeof(pointer*));
	//char *token;
	pid_t pid;
	pid_t waitPid;	
	int temp;	
	
	//if (!tokens) {
		//fprintf(stderr, "lsh: allocation error\n");
		//exit(0);
	//}
	
	pid = fork();
	if(pid == 0)
	{
		if(execvp(pointChar[0],pointChar) == -1) //FINISH
		{
			perror("error");
		}
	}
	else if(pid < 0)
	{
		perror("error");
	}
	else
	{
		//waitPid = waitpid(pid, &temp, WUNTRACED);
		//while (!WIFEXITED(temp) && !WIFSIGNALED(temp));
	}
	return true;
}
