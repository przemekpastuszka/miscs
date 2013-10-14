#ifndef HELLLOG
#define HELLLOG

#include <list>
#include <string>

/*Najzwyklejszy w œwiecie log*/

class Log
{
public:
	enum priority {LOW=0,NORMAL,HIGH,ERR}; //priotytety wiadomoœci
private:
	struct Msg
	{
		priority importance;
		std::string txt;
	};
	static std::list<Msg> messages;
public:
	static void AddMessage(std::string txt,priority a); //dodaje wiadomoœæ o danym priorytecie
	static void Clear(); //czyœci log
	static void ToFile(priority a,const char* name); //zapisuje wiadomoœci o priorytecie >= a do pliku name
	static void ToConsole(priority a,int max); //podobnie co wy¿ej, tyle ¿e do konsoli i tylko max ostatnich wiadomoœci
};

#endif