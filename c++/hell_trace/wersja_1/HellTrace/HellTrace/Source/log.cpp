/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Implementation of log
*/

#include "log.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

list<Log::Msg> Log::messages=list<Log::Msg>();

void Log::AddMessage(string txt, priority a)
{
	Msg temp;
	temp.txt=txt;
	temp.importance=a;
	messages.push_front(temp);
	//when error message comes exception is thrown
	//quite messy, to be changed
	if(a==ERR)
		throw txt;
	ToConsole(NORMAL,10);
}

void Log::Clear()
{
	messages.clear();
}

void Log::ToConsole(priority a, int max)
{
	system("cls");
	int counter=0;
	for(list<Msg>::iterator it=messages.begin();it!=messages.end()&&counter<max;++it)
		if((*it).importance>=a)
		{
			cout<<(*it).txt<<endl;
			++counter;
		}
	cout<<endl<<endl;
}

//saves msgs as html file
void Log::ToFile(priority a, const char *name)
{
	ofstream wyj(name);
	for(list<Msg>::iterator it=messages.begin();it!=messages.end();++it)
		if((*it).importance>=a)
		{
			switch((*it).importance)
			{
				case LOW:
				{
					wyj<<(*it).txt<<"<br>";
					break;
				}
				case NORMAL:
				{
					wyj<<"<u>"<<(*it).txt<<"</u><br>";
					break;
				}
				case HIGH:
				{
					wyj<<"<u><b>"<<(*it).txt<<"</b></u><br>";
					break;
				}
				case ERR:
				{
					wyj<<"<u><b><font color=red>"<<(*it).txt<<"</font></u><br>";
					break;
				}
			}
			
		}
	wyj.close();
}