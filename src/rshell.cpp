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

//void precedence(string line, CommandLine tempObj)
//{
//	int start;
//        for(unsigned int j = 0; j < line.size(); j++)
//        {
//                if(line.at(j) == '(' && level == 0)
//                {
//                        level++;
//			line = line.substr(j,line.size() - 1);
//                        precedence(line,level);
//                }
//                else if(line.at(j) == '(')
//		{
//			level++;
//		}
//		if(line.at(j) == ')')
//                {
//                        level--;
//                        if(level == 0)
//                        {
//				//run
//			}
//		}
//	}
//	for(unsigned int j = 0; j < line.size(); j++)
//	{
//		if(line.at(j) == '(' && line.at(j + 1) != '(')
//		{
//			for(unsigned int k = j; k < line.size(); k++)
//			{
//				if(line.at(k) == '(' || line.at(k) == ')')
//				{
//					string temp = line.substr(j, k - j);
//					tempObj.split(temp);
//					
//				}
//			}
//		}
//		else if(line.at(j) == '(' && line.at(j + 1) == '(')
//		{
//			
//		}
//	}
//}



void nextStep(string cLine)
{
 CommandLine object;
//Adds a % character to separate from connectors------------------------------------------------------------------------------------ 
unsigned int g = cLine.size() - 1;
 int e = 0;
 string newStr = cLine;
 for(unsigned int j = 0; j < g; j++)     //Adds % character to separate commands from connectors
 {
        if (cLine.at(j) == ';' || (cLine.at(j) == '&' && cLine.at(j+1) == '&') || (cLine.at(j) == '|' && cLine.at(j+1) == '|'))
        {
               newStr = newStr.substr(0,j + e);
               newStr = newStr + " %";
               newStr = newStr + cLine.substr(j, cLine.size() - j);

               e = e + 2;
        }
 }

unsigned int q = newStr.size() - 1;	//Replaces pipe symbol with @ to make tokenizing easier
int w = 0;
string temp = newStr;
for(unsigned int l = 0; l < q; l++)
{
	if(newStr.at(l) == '|' && newStr.at(l + 1) != '|')
	{
		temp = temp.substr(0, l + w);
		temp = temp + " @";
		temp = temp + newStr.substr(l, cLine.size() - l);

		w = w + 2;
	}
}

newStr = temp;

//cout <<  "TEST FOR @ SYMBOL: " << newStr << endl;

//Isolates parentheses--------------------------------------------------------------------------------------------------------------------------------
 //int f = 0;
 bool isParen = false;
 unsigned int h = newStr.size();
 string newStr2 = newStr;
 for(unsigned int k = 0; k < h; k++)
 {
	if(newStr.at(k) == '(' || newStr.at(k) == ')')// || newStr.at(k) == '[' || newStr.at(k) == ']')	//Adds % character to separate parentheses from commands
	{
		if(isParen == false)
		{
			isParen = true;
		}
//		newStr2 = newStr2.substr(0, k + f);
//		newStr2 = newStr2 + " ";					//IF WE ARE KEEPING IN PARENTHESES, DO WE NEED PERCENT SIGN?
//		newStr2 = newStr2 + newStr.substr(k, 1);
//		newStr2 = newStr2 + " ";
//		newStr2 = newStr2 + newStr.substr(k + 1, newStr.size() - k);	//NEED SPACES ON RIGHT SIDE OF PARENTHESES
//		f = f + 2;
	}
 }

//Finds either test or []-------------------------------------------------------------------------------------------------
 int start;
 int end;
 string testStore;
 string temptestString;
 bool result;
 Command testObject; 
 for(unsigned int x = 0; x < newStr2.size(); x++)
 {
	if(newStr2.at(x) == '[')
	{
		start = x;
		for(unsigned int y = x; y < newStr2.size(); y++)
		{
			if(newStr2.at(y) == ']')
			{
				end = y;
				testStore = newStr2.substr(start, end - start);

				result = testObject.test(testStore);
				temptestString = newStr2.substr(0, start);
				if(result == true)
				{
					temptestString = temptestString + " TRUE ";
				}
				else
				{
					temptestString = temptestString + " FALSE ";
				}
				temptestString = temptestString + newStr2.substr(end - start + 1, newStr2.size() - 1);
				newStr2 = temptestString;
				break;
			}
		}
	}
	else if(newStr2.at(x) == 't')
	{
		if(x < newStr2.size() - 4)
		{
			if(newStr2.at(x + 1) == 'e' && newStr2.at(x + 2) == 's' && newStr2.at(x + 3) == 't' && newStr2.at(x + 4) == ' ')
			{
				for(unsigned int q = x; q < newStr2.size(); q++)
				{
					if(newStr2.at(q) == '%' || newStr2.at(q) == '(' || newStr2.at(q) == ')')
					{
						testStore = newStr2.substr(x + 5, q - x - 6);
						result = testObject.test(testStore);
						temptestString = newStr2.substr(0, x);
                                		if(result == true)
                                		{
                                		        temptestString = temptestString + " TRUE ";
                                		}
                                		else
                                		{
                                		        temptestString = temptestString + " FALSE ";
                                		}
                                		temptestString = temptestString + newStr2.substr(q,newStr2.size() - 1);
                                		newStr2 = temptestString;

						break;
					}
					else if(q == newStr2.size() - 1)
					{
						testStore = newStr2.substr(x + 5, q - x - 4);
						result = testObject.test(testStore);
						temptestString = newStr2.substr(0, x);
                               			if(result == true)
                                		{
                                        		temptestString = temptestString + " TRUE ";
                                		}
                                		else
                                		{
                                        		temptestString = temptestString + " FALSE ";
                                		}
                                		//temptestString = temptestString + newStr2.substr(q, newStr2.size() - 1);
                                		newStr2 = temptestString;

						break;
					}
				}
			}
		}
	}
	testStore = "";
 }
 if(isParen == false)
 {
	object.split(newStr2); //Splits the string into tokens and then runs it -------------------------------------------------------------------------
 }
 else
 {
	object.precedence(newStr2);
 }

}




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
	//CommandLine object;
	cout << name1 << "@" << name2  << " $ "; //prints login and host info	//MAKE SURE TO FIX
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


	for(unsigned int a = 0; a < cLine.size(); a++)	//checks for uneven amount of parentheses
        {
                if(cLine.at(a) == '(')
                {
                        for(unsigned int b = a + 1; b < cLine.size(); b++)
                        {
				if(cLine.at(b) == ')')
				{
					break;	
				}
				else if(b == cLine.size() - 1)
				{
					perror("Uneven amount of parentheses");
					exit(0);
				}
                        }
                }
        }

	for(unsigned int c = 0; c < cLine.size(); c++) //checks for uneven amount of brackets
        {
                if(cLine.at(c) == '[')
                {
                        for(unsigned int d = c + 1; d < cLine.size(); d++)
                        {
                                if(cLine.at(d) == ']')
                                {
                                        break;
                                }
                                else if(d == cLine.size() - 1)
                                {
                                        perror("Uneven amount of brackets");
                                        exit(0);
                                }
                        }
                }
        }

	
	nextStep(cLine);

//	unsigned int g = cLine.size() - 1;
//	int e = 0;
//	string newStr = cLine;
//	for(unsigned int j = 0; j < g; j++)	//Adds % character to separate commands
//	{
//		if (cLine.at(j) == ';' || (cLine.at(j) == '&' && cLine.at(j+1) == '&') || (cLine.at(j) == '|' && cLine.at(j+1) == '|'))
//		{
//			newStr = newStr.substr(0,j + e);
//			newStr = newStr + " %";
//			newStr = newStr + cLine.substr(j, cLine.size() - j);
//			
//			e = e + 2;
//		}
//	}
	
	
//	object.split(newStr);	//splits the string into tokens

	cLine = ""; //Clears string to take in next command
 }
 return 0;
}
