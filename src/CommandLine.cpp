#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "CommandLine.h"

using namespace std;

bool parenWorked = false;
bool global_connect = true;
//bool redirectExist = false; 


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
		//base case: no beginning parentheses
		if(comLine.at(j) != '(')
		{
			for(unsigned int l = j; l < comLine.size(); l++)
			{
				//runs everything before parentheses
				if(comLine.at(l) == '(')
				{
					string temp = comLine.substr(j, l - j);
					split(temp);
					j = l - 1; //Not + 1 to account for j++
					break;
				} //runs everything inside parentheses
				else if(l == comLine.size() - 1)
				{
					//+ 1 accounts for size of entire string
					string temp = comLine.substr(j, l - j + 1);
					split(temp);
					j = l + 1;
					break;
				}
			}
		}
		else if(comLine.at(j) == '(')// && comLine.at(j + 1) != '(')
		{
			level++;
			for(unsigned int k = j + 1; k < comLine.size(); k++)
			{
				//search for the second half of the parentheses
				if(comLine.at(k) == '(')// || comLine.at(k) == ')')
				{
					level++;
				}
				else if(comLine.at(k) == ')')
				{
					level--;
					if(level == 0)
					{
						string temp = comLine.substr(j + 1, k - j - 1);
						precedence(temp);
						j = k + 1;
						break;
					}
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
	//bool connectBool = true;
	char** pointChar = new char*[pCharSize];
	bool currRedirect = false;
	int comRed = 0;
	vector<bool> commandRedirect;

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
			//global_connect = false;
		}
		else if(vecChar.at(var) == "TRUE")
		{
			cout << "(True)" << endl;
			testExist = true;
			testWorked = true;
			//global_connect = true;
		}
		
	//	for(unsigned int a = 0; a < vecChar.size(); a++)	//seaches for existance of redirect
	//	{
	//		for(unsigned int b = findRedirect; vecChar.at(b) != '\0'; b++)
	//		{
	//			if(vecChar.at(b) == "<" || vecChar.at(b) == ">" || vecChar.at(b) == ">>" || vecChar.at(b) == "|")
	//			{
	//				commandRedirect.push_back(1)
	//			}			
	//		}
	//		a = b;
	//		findRedirect = a + 1;	//skips over null character
	//	}

		//Fills char**, y is used to keep track of the vector, z is used to keep track of the char**
		
		for(unsigned int y = var, z=0; y < vecChar.size(); y++, z++)
		{
			for(unsigned int a = var; vecChar[a] != "\0"; a++)
			{
				if(vecChar[a] == "<" || vecChar[a] == ">" || vecChar[a] == ">>" || vecChar[a] == "|")
				{
					currRedirect = true;
					commandRedirect.push_back(currRedirect);
					break;					//Make sure it only breaks outs of inner for loop
				}
			}
			if(currRedirect = false)
			{
				
				// reached end of command
				if(vecChar[y] == "\0")
				{
					commandRedirect.push_back(false);

					pointChar[y] = '\0';
					if(y < vecChar.size() - 1)
					{
						var = y + 1;
					}
					else
					{
						var = -1;
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
						pointChar[y + 1] = '\0';
					}
					else if(y == vecChar.size() - 1)
					{
						pointChar[y + 1] = '\0';
					}
				}
			}
			else
			{
				//Sets up for run_redirect function
			}

			currRedirect = false;
		}		
		
		if(global_connect == true && testExist == false && commandRedirect[comRed] == false)	//Runs commands <------------------------------------------------------------------------
		{	
			comWorked = temp.run(pointChar, track);
		}
		else if(global_connect == true && testExist == false && commandRedirect[comRed] == true)
		{
			comWorked = temp.run_redirect(pointChar);
		}
		global_connect = true;
		//redirectExist = false;
		comRed++;		


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
					global_connect = ampersand(comWorked);
				}
				else if(currRead == "||")
				{
					global_connect = doubleLine(comWorked);
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
                                        global_connect = ampersand(testWorked);
                                }
                                else if(currRead == "||")
                                {
                                        global_connect = doubleLine(testWorked);
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
	int array[2];	
	pid_t pid;	
	int temp;

	//for(unsigned int q = 0; pointChar[q] != NULL; q++)
	//{
	//	for(unsigned int p = 0; pointChar[p] != '\0'; p++)
	//	{
	//		if(pointChar[q][p] == '<')
	//		{
	//
	//		}
	//		else if(pointChar[q][p] == '>' && pointChar[q][p+1] != '>')
	//		{
	//		
	//		}
	//		else if(pointChar[q][p] == '>' && pointChar[q][p+1] == '>')
	//		{
	//
	//		}
	//		else if(pointChar[q][p] == '|' && pointChar[q][p + 1] != '|')
	//		{
	//				
	//		}
	//	}
	//}

	
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

//Handles redirection
bool Command::run_redirect(char** pointChar)
{
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
	if(i == -1)	//checks to see if path exists
	{
		perror("Stat Failure");
		return false;
	}
	//Flag checks
	if(test_flag == "-d")
	{
		if(S_ISDIR(s.st_mode))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(test_flag == "-f")
	{
		if(S_ISREG(s.st_mode))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
	return false;
}

