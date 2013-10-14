/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

"No log, no fun"
*/

#ifndef HELLLOG
#define HELLLOG

#include <list>
#include <string>


class Log
{
public:
	enum priority {LOW=0,NORMAL,HIGH,ERR}; //avalible priorities of messages
private:
	struct Msg
	{
		priority importance;
		std::string txt;
	};
	static std::list<Msg> messages;
public:
	static void AddMessage(std::string txt,priority a); //add message with specified priority
	static void Clear(); 
	static void ToFile(priority a,const char* name); //save msgs which priority is higher than a to file name
	static void ToConsole(priority a,int max); //same as above, but writes msgs from [1,max] range only to console
};

#endif