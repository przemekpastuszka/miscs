/*
HellTrace by Przemys�aw Pastuszka
University of Wroc�aw, Poland

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
	time(&temp.t);
	messages.push_front(temp);
	
	if(a>=NORMAL)
		cout<<txt<<endl;
	//ToConsole(NORMAL,1);
	
	//when error message comes exception is thrown
	//quite messy, to be changed
	if(a==ERR)
		throw txt.c_str();
	
}

void Log::Clear()
{
	messages.clear();
}

void Log::ToConsole(priority a, int max)
{
	int counter=0;
	for(list<Msg>::iterator it=messages.begin();it!=messages.end()&&counter<max;++it)
		if((*it).importance>=a)
		{
			cout<<(*it).txt<<endl;
			++counter;
		}
}

//saves msgs as html file
void Log::ToFile(priority a, const char *name)
{
	ofstream wyj(name);
	for(list<Msg>::reverse_iterator it=messages.rbegin();it!=messages.rend();++it)
		if((*it).importance>=a)
		{
			time_t t=(*it).t;
			struct tm* data=localtime(&t);
			char tInfo[80];
			strftime(tInfo,80,"%H:%M:%S - ",data);
			switch((*it).importance)
			{
				case LOW:
				{
					wyj<<tInfo<<(*it).txt<<"<br>";
					break;
				}
				case NORMAL:
				{
					wyj<<"<u>"<<tInfo<<(*it).txt<<"</u><br>";
					break;
				}
				case HIGH:
				{
					wyj<<"<u><b>"<<tInfo<<(*it).txt<<"</b></u><br>";
					break;
				}
				case ERR:
				{
					wyj<<"<u><b><font color=red>"<<tInfo<<(*it).txt<<"</font></u><br>";
					break;
				}
			}
			
		}
	wyj.close();
}
