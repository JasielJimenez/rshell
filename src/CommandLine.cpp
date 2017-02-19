#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

#include "CommandLine.h"

using namespace std;

//void CommandLine::setCounter(int num)
//{
//	counter = 0;
//}
//
//void CommandLine::updateCounter()
//{
//	counter++;
//}

void CommandLine::split(string comLine)
{
	connect = vector<string>();
	//counter = 0;
	int num = 0;
	//setCounter(num);
	//cout << "counter1 is: " << counter << endl;
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

	//cout << "counter2 is: " << counter << endl;
	for(int x = 0; x < connect.size(); x++)
	{
		cout << connect.at(x) << endl;
	}
	cout << endl;
	

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
	//cout << "counter3 is: " << counter << endl;	
	


//	cout << vecChar.size() << endl << endl;
//	for (int h = 0; h < vecChar.size(); h++)
//	{
//		cout << vecChar.at(h) << endl;
//	}


	
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

//	int pCharSize = vecChar.size() + 1;
//	char** pointChar = new char*[vecChar.size()+1];
//	for(unsigned int y = 0; y < vecChar.size(); y++) //converts vector to char**
//	{
//		if(vecChar[y] == "\0")
//			pointChar[y] = '\0';
//
//		else
//		{
//			string st_temp = vecChar[y];
//			char * blah = (char *)st_temp.c_str();
//			pointChar[y] = blah;
//		}
//
//	}	
//	pointChar[vecChar.size()] = '\0';
	
	//cout << "counter4 is: " << counter << endl;
	obj.Reader(vecChar,connect); //calls the reader function
	
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

void Symbol::Reader(vector<string> vecChar,  vector<string> connect)
{
	//cout << "STARTING THE READER METHOD" << endl;
	//cout << "counter5 is: " << counter << endl;
	bool comWorked = false;
	Command temp;
	//for(int x = 0; x < pCharSize; x++)
	//{

	int counter = 0;
	int track = 0;
	int trackCount = 0;
	int pCharSize = vecChar.size() + 1;
	int adjSize = pCharSize;
	int var = 0;
	
	char** pointChar = new char*[pCharSize];

	while(counter < connect.size() + 1)
	{
		//char** pointChar = new char*[pCharSize];
		cout << "var is " << var << endl;
		for(unsigned int y = var; y < vecChar.size(); y++)
		{
			if(vecChar[y] == "\0")
			{
				pointChar[y] = '\0';
				if(y < vecChar.size() - 1)
				{
					var = y + 1;
				}
				break;
			}
			else
			{
				string st_temp = vecChar[y];
				char* ch_temp = (char *)st_temp.c_str();
				pointChar[y] = ch_temp;
			}
		}
		//pointChar[var] = '\0';
	
		cout << "pCharsize: " << pCharSize << endl;
		cout << "adjSize: " << adjSize << endl;
	//	for(unsigned int l = 0; l < adjSize; l++)
	//	{
	//		cout << pointChar[l] << endl;
	//	}
		//cout << "pointChar[5]" << pointChar[5] << endl;
		comWorked = temp.run(pointChar, track);
		//cout << "FINISHED CALLING RUN" << endl;
		//cout << "CONNECT SIZE IS " << connect.size() << endl;
		//cout << comWorked << endl;
		//cout << "comWorked up (1 is true, 0 is false)" << endl;

		
		if(connect.size() > 0 && counter < connect.size()) //goes through here only if there are connectors
		{
			//cout << "COUNTER IS " << counter << endl;
			string currRead = connect.at(counter);
			//cout << currRead << endl;
			//cout << "currRead up" << endl;
			//cout << "BRUH U HERE?" << endl;
			//

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
			else if(currRead == "exit") 	//GET EXIT TO WORK
			{
				exit(0);
			}
			//updateCounter();
			//cout << "WOW I MADE IT" << endl;
		}
		counter++;
		trackCount = track;
		while(trackCount < pCharSize && pointChar[trackCount] != '\0')
		{
			trackCount++;
			adjSize--;	
		}
		adjSize--;
		track = trackCount + 1;
		for(int i = trackCount, j = 0; i < adjSize; i++,j++)
		{
			pointChar[j] = pointChar[i] + 1;
		}
	}
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

bool Command::run(char** pointChar, int track) //run commands correctly
{	
	//char **pointer = malloc(bufsize * sizeof(pointer*));
	//char *token;
	pid_t pid;
	cout << "PUP" << endl;
	//pid_t waitPid;	
	int temp;	
	pid = fork();
	//cout << pid << endl;
	if(pid == 0)
	{
		cout << "SURE" << endl;
		if(execvp(pointChar[0],pointChar) == -1)
		{
			perror("execvp error");
			return false;
		}
	cout << "pid == 0" << endl;
	}
	else if(pid < 0)
	{
		perror("fork error");
		return false;
	}
	else
	{
		cout << "pid > 0" << endl;
		//waitPid = waitpid(pid, &temp, WUNTRACED);
		//while (!WIFEXITED(temp) && !WIFSIGNALED(temp));
		if (waitpid(pid, &temp, 0) == -1)
		{
			perror("waitpid error");
			exit(0);
		}
	}
	cout << "DONE" << endl;
	return true;
}
