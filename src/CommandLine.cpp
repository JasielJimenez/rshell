#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

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

	vector<string> storeCom;

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
		for(unsigned int y = var, z=0; y < vecChar.size(); y++, z++)
		{
			currRedirect = false;
			for(unsigned int a = var; a != vecChar.size() && vecChar[a] != "\0"; a++)
			{
				//cout << "vecChar.size(): " << vecChar.size() << endl;
				//cout << "vecChar[a]: " << vecChar[a] << endl;
				//cout << "a: " << a << endl;
				if(vecChar[a] == "<" || vecChar[a] == ">" || vecChar[a] == ">>" || vecChar[a] == "|")
				{
					currRedirect = true;
					commandRedirect.push_back(currRedirect);
					break;					//Make sure it only breaks outs of inner for loop
				}
			}
			if(currRedirect == false)
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
						commandRedirect.push_back(false);
						pointChar[y + 1] = '\0';
						//var = y + 1;
					}
					else if(y == vecChar.size() - 1)
					{
						commandRedirect.push_back(false);
						pointChar[y + 1] = '\0';
						//var = y + 1;
					}
					//var = y + 1;
				}
			}
			else
			{	
				unsigned int jTrack = 0;
				for(unsigned int j = var;j != vecChar.size() && vecChar[j] != "\0"; j++)
				{
					jTrack = j;
				}
				jTrack++;
				for(unsigned int k = var; k < jTrack; k++)
				{
					storeCom.push_back(vecChar[k]);
				}
				var = jTrack + 1;
				break;
			}

			currRedirect = false;
		}		
		//cout << "commandRedirect size: " << commandRedirect.size() << endl;
		//cout << "comRed: " << comRed << endl;
		if(global_connect == true && testExist == false && commandRedirect[comRed] == false)	//Runs commands <------------------------------------------------------------------------
		{
			comWorked = temp.run(pointChar, track);
		}
		else if(global_connect == true && testExist == false && commandRedirect[comRed] == true)//Runs commands with redirection <------------------------------------------------------- 
		{
			comWorked = temp.run_redirect(storeCom);
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
	pid_t pid;	
	int temp;
	
	pid = fork();

	if(pid == 0)
	{		
		if(execvp(pointChar[0],pointChar) == -1)
		{
			perror("execvp error");
			//return false;
			exit(0);
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
bool Command::run_redirect(vector<string> storeCom)
{
	//Checks for each symbol used in redirection
	bool isInput = false;
	bool isOutput = false;
	bool isOutput2 = false;
	bool isPipe = false;
	vector< vector<string> > allComs;	//2D vector that stores each part of the entire command, but keeps them separated
	vector<string> comPart; 		//part that will contain up to redirection symbols

	//variables required for opening files for input and output
	int redirect[2];
	int input = 0;
	int output = 1;
	int redirect_input;
	bool input_close = false;
	bool output_close = false;
	pid_t pid;

	//Variables used for execvp()
	int pCharSize = storeCom.size() + 1; 
	char** pointChar = new char*[pCharSize];
	int wait = 0;


	for(unsigned int q = 0; q < storeCom.size(); q++)
	{
		if(storeCom[q] == "<")
		{
			allComs.push_back(comPart);	//adds everything up to symbol
			isInput = true;			//Found symbol needed for later part
			while(comPart.empty() == false)	//Clears out vector for next loop
			{
				comPart.pop_back();
			}
		}
		else if(storeCom[q] == ">")
		{
			allComs.push_back(comPart);
			isOutput = true;
			while(comPart.empty() == false)
			{
				comPart.pop_back();
			}
		}
		else if(storeCom[q] == ">>")
		{
			allComs.push_back(comPart);
			isOutput2 = true;
			while(comPart.empty() == false)
                        {
                                comPart.pop_back();
                        }
		}
		else if(storeCom[q] == "@")
		{
			allComs.push_back(comPart);
			isPipe = true;
			while(comPart.empty() == false)
                        {
                                comPart.pop_back();
                        }
		}
		else
		{
			comPart.push_back(storeCom.at(q));	//continues adding to vector until symbol is found
	
		//	Tests to see what is pushed back
		//	for(unsigned int e = 0; e < comPart.size(); e++)
		//	{
		//		cout << comPart[e] << " ";
		//	}
		//	cout << endl;
		}
	}
	allComs.push_back(comPart);	//adds remaining part of original command
	

	if(isInput == true)
	{
		input = open(allComs[1].front().c_str(), O_RDONLY);
		redirect_input = input;
		allComs.erase(allComs.begin() + 1);
		input_close = true;		
	}

	if(isOutput == true)
	{
		output = open(allComs.back().front().c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR );
		allComs.pop_back();
		output_close = true;
	}
	else if(isOutput2 == true)
	{
		output = open(allComs.back().front().c_str(), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR );
		allComs.pop_back();
		output_close = true;
	}

//	Tests to see what is in allComs vector	
//	cout << endl;
//	for(unsigned int i = 0; i < allComs.size(); i++)
//	{
//		for(unsigned int j = 0; j < allComs[i].size(); j++)
//		{
//			cout << allComs[i][j] << " ";
//		}
//		cout << endl;
//	}
//	cout << endl;

	for(unsigned int z = 0; z < allComs.size(); z++)
	{
		pipe(redirect);			//Starts piping
		pid = fork();
		if(pid == 0)
		{
			dup2(redirect_input,0);
			if(z < allComs.size() - 1)			//ERROR COULD BE HERE --------------------------------------------------------------------
			{
				dup2(redirect[1],1);
			}
			else
			{
				dup2(output,1);
			}
			close(redirect[0]);

			for(unsigned int g = 0; g < allComs[z].size(); g++)
			{
				string st_temp = allComs[z][g];
                		char* ch_temp = (char *)st_temp.c_str();
                		pointChar[g] = ch_temp;
               			if(allComs[z].size() == 1)// || y == vecChar.size() - 1)
                		{
                			pointChar[g + 1] = '\0';
                		}
                		else if(g == allComs[z].size() - 1)
                		{
               	 			pointChar[g + 1] = '\0';
		      		}
			}
			wait = execvp(pointChar[0], pointChar);
			perror("execvp error");
			return false;	
		}
		else if(pid < 0)
        	{
        	        perror("fork error");
	                return false;
	        }
		else
		{
			if(waitpid(pid, &wait, 0) != pid)
			{
				perror("wait error");
				
			}
			close(redirect[1]);
			redirect_input = redirect[0];			
		}
	}
	if(input_close == true)
	{
		close(input);
	}
	if(output_close == true)
	{
		close(output);
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

