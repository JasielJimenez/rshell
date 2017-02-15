#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

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

void CommandLine::Split(string comLine)
{
	int num = 0;
	setCounter(num);
	this->comLine = comLine;
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
			if(comLine.at(i+1) == '|'
			{
				connect.push_back("||");
			}
		}
		else if(comLine.at(i) == '#')
		{
			connect.push_back("#")
			break;
		}
	}
	//IMPLEMENT STRTOK FUNCTION
}

void Symbol::Reader()
{
	currRead = connect.at(counter);
	if(currRead = ";")
	{
		
	}
	else if(currRead = "&&")
	{
		
	}
	else if(currRead = "||")
	{
		
	}
	counter++;
}

void Symbol::semicolon()
{
	
}

void Symbol::ampersand()
{
	
}

void Symbol::doubleLine()
{
	
}

void Symbol::comment()
{
	
}
