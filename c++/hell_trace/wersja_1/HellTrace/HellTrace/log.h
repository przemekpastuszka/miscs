#ifndef HELLLOG
#define HELLLOG

#include <list>
#include <string>

/*Najzwyklejszy w �wiecie log*/

class Log
{
public:
	enum priority {LOW=0,NORMAL,HIGH,ERR}; //priotytety wiadomo�ci
private:
	struct Msg
	{
		priority importance;
		std::string txt;
	};
	static std::list<Msg> messages;
public:
	static void AddMessage(std::string txt,priority a); //dodaje wiadomo�� o danym priorytecie
	static void Clear(); //czy�ci log
	static void ToFile(priority a,const char* name); //zapisuje wiadomo�ci o priorytecie >= a do pliku name
	static void ToConsole(priority a,int max); //podobnie co wy�ej, tyle �e do konsoli i tylko max ostatnich wiadomo�ci
};

#endif