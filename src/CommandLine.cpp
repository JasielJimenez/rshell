#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
//#include <sys/types.h>
#include <sys/stat.h>

#include "CommandLine.h"

using namespace std;

bool parenWorked = false;
bool global_connect = true;


string trimElements(string original, string elmToTrim)
{
	string str = original + "\0";
	size_t p = str.find_first_not_of(elmToTrim);
	str.erase(0,p);
	p = str.find_last_not_of(elmToTrim);
	if(string::npos != p)
	{
		str.erase(p + 1);
	}	

	return str;
}

void CommandLine::precedence(string comLine)
{
	int level = 0;
	for(unsigned int j = 0; j < comLine.size(); j++)
	{
		if(j == 0 && comLine.at(j) != '(')
		{
			for(unsigned int l = j; l < comLine.size(); l++)
			{
				//search for the first half of the perentheses
				if(comLine.at(l) == '(')
				{
					string temp = comLine.substr(j, l - j);
					cout << "->" << temp << "!" << endl;
					split(temp);
					continue;
				}
			}
		}
		if(comLine.at(j) == '(' && comLine.at(j + 1) != '(')
		{
			level++;
			for(unsigned int k = j + 1; k < comLine.size(); k++)
			{
				//search for the second half of the parentheses
				if(comLine.at(k) == '(')// || comLine.at(k) == ')')
				{
					level++; //?
				}
				else if(comLine.at(k) == ')')
				{
					string temp = comLine.substr(j, k - j + 1);
					cout << "temp:" << temp << "!" << endl;
					continue;
					//split(temp);
				}
			}
		}
	}
}


void CommandLine::split(string comLine)
{
	connect = vector<string>();
	this->comLine = comLine;
	Symbol obj;
 	//puts connectors into vector
	for(unsigned int i = 0; i < comLine.size(); i++)
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
		else if(i < comLine.size() - 3 && comLine.substr(i,4) == "exit") 
		{
			connect.push_back("exit");
		}
	}

	

	//converts string into cstring
	char* cstring = new char [comLine.size() + 1];
	strcpy(cstring, comLine.c_str());

	// create vector to store tokens
        vector<string> vecChar;
	char* pointer = strtok(cstring," ;||&&");
	char* temp = pointer;
	while(temp != NULL)
	{
	 	//removes leading whitespace
		string str(temp);//Converts cstring to string
		str = str + "\0";
		size_t p = str.find_first_not_of(" \t");
		str.erase(0,p);
		p = str.find_last_not_of(" \t");
		if(string::npos != p)
		{
			str.erase(p + 1);
		}	

//		string str(temp);//Converts cstring to string
//		str = trimElements(str, " \t");
		
		// percentage represents end of a command
		if(str == "%")
		{	
			vecChar.push_back("\0");
		}
		else
		{
			vecChar.push_back( str   );
		}
		temp = strtok(NULL," ;||&&");
	}
		
	
	obj.Reader(vecChar,connect); //calls the reader function
	

	for(unsigned int u = 0; u < connect.size(); u++) //clears connect vector
	{
		connect.pop_back();
	}

	
}

// vecChar = vector of strings representing tokenized strings
// connect = vector of strings, lists all connectors of current substring
void Symbol::Reader(vector<string> vecChar,  vector<string> connect)
{
	
	bool comWorked = false; // did the command run successfully?

	bool testExist = false;	//Used to skip executing the replaced test command
	bool testWorked = false;

	Command temp;
	

	unsigned int counter = 0; // goes through connector list in while loop
	int track = 0;
	int trackCount = 0;
	int pCharSize = vecChar.size() + 1;
	int adjSize = pCharSize;
	int var = 0;
	bool connectBool = true;
	char** pointChar = new char*[pCharSize];

	while(counter < connect.size() + 1)
	{
		if(var == -1)
		{
			break;
		}

		if(vecChar.at(var) == "exit")
		{
			exit(0);
		}		
		if(vecChar.at(var) == "FALSE")
		{
			cout << "(False)" << endl;
			testExist = true;
		}
		else if(vecChar.at(var) == "TRUE")
		{
			cout << "(True)" << endl;
			testExist = true;
			testWorked = true;
		}
		//Fills char**, y is used to keep track of the vector, z is used to keep track of the char**
		for(unsigned int y = var, z=0; y < vecChar.size(); y++, z++)
		{
			// reached end of command
			if(vecChar[y] == "\0")
			{
				pointChar[y] = '\0';
				if(y < vecChar.size() - 1)
				{
					var = y + 1;
				}
				else
				{
					var = -1;	//ERROR: echo hello && ls	results in a error
				}
				break;
			}
			else //copies over current vector element
			{
				string st_temp = vecChar[y];
				char* ch_temp = (char *)st_temp.c_str();
				pointChar[z] = ch_temp;
				if(vecChar.size() == 1)// || y == vecChar.size() - 1)
				{
					//cout << "y = " << y << endl; 
					//cout << vecChar.size() << endl;
					//cout << vecChar.at(y) << endl;
					pointChar[y + 1] = '\0';
				}
				else if(y == vecChar.size() - 1)
				{
					//cout << vecChar.at(y) << endl;
					//cout << "blah" << endl;
					pointChar[y + 1] = '\0';
				}
			}
		}		
		
		if(connectBool == true && testExist == false)	//Runs commands <------------------------------------------------------------------------
		{	
			comWorked = temp.run(pointChar, track);
		}
		connectBool = true;

		
		if(connect.size() > 0 && counter < connect.size()) //goes through here only if there are connectors
		{
			
			string currRead = connect.at(counter);
			
			//perform normally if test wasn't done
			if(testExist == false)
			{
				if(currRead == ";")
				{
					semicolon();
				}
				else if(currRead == "&&")
				{
					connectBool = ampersand(comWorked);
				}
				else if(currRead == "||")
				{
					connectBool = doubleLine(comWorked);
				}
				else if(currRead == "exit")
				{
					exit(0);
				}
			}
			//use results of test to determine effects of connectors
			else
			{
				if(currRead == ";")
                                {
                                        semicolon();
                                }
                                else if(currRead == "&&")
                                {
                                        connectBool = ampersand(testWorked);
                                }
                                else if(currRead == "||")
                                {
                                        connectBool = doubleLine(testWorked);
                                }
                                else if(currRead == "exit")
                                {
                                        exit(0);
                                }	
			}
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
	
		testWorked = false;
		testExist = false;
	} // end of while-loop
}

void Symbol::semicolon()
{
	return;		//implement next command
}

bool Symbol::ampersand(bool comWorked) //implement next command only if last command succeeded
{
	if(comWorked == false)
	{
		return false;
	}	
	else
	{
		return true;
	}
}

bool Symbol::doubleLine(bool comWorked) //implement next command only if last command failed
{
	if(comWorked == true)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Command::run(char** pointChar, int track) //run commands correctly
{		
	pid_t pid;	
	int temp;	
	pid = fork();
	
	if(pid == 0)
	{		
		if(execvp(pointChar[0],pointChar) == -1)
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
		if (waitpid(pid, &temp, 0) == -1)
		{
			perror("waitpid error");
			exit(0);
		}
	}
	return true;
}


bool Command::test(string test)	
{
	string test_flag;
	if(test.find("-d") != string::npos) //Finding the flag
	{
		test_flag = "-d";
	} 
	else if(test.find("-f") != string::npos)
	{
		test_flag = "-f";
	}
	
	if(test.find("-d") == string::npos && test.find("-f") == string::npos)
	{
		test_flag = "-e";
	}

	string path;					//Trimming the string to leave only the path
	for(unsigned int p = 0; p < test.size(); p++)
	{
		if(test.at(p) == '/')
		{
			path = test.substr(p,test.size() - p);
			cout << "path: " << path << "!" << endl;
			break;
		}
	}  
	
	//Removes leading whitespace --------------------------------------------------
	size_t p = path.find_first_not_of(" \t");
        path.erase(0,p);
        p = path.find_last_not_of(" \t");
        if(string::npos != p)
       	{
        	path.erase(p + 1);
        }
	//-----------------------------------------------------------------------------


	char* charTest = new char[path.size() + 1];
	strcpy(charTest, path.c_str());

	struct stat s;
	int i = stat(charTest, &s);
	if(i == -1)
	{
		perror("Stat Failure");
		//cout << "(False)" << endl;
		return false;
	}

	if(test_flag == "-d")
	{
		if(S_ISDIR(s.st_mode))
		{
			//cout << "(True)" << endl;
			return true;
		}
		else
		{
			//cout << "(False)" << endl;
			return false;
		}
	}
	else if(test_flag == "-f")
	{
		if(S_ISREG(s.st_mode))
		{
			//cout << "(True)" << endl;
			return true;
		}
		else
		{
			//cout << "(False)" << endl;
			return false;
		}
	}
	else
	{
		//cout << "(True)" << endl;
		return true;
	}
	return false;
}

