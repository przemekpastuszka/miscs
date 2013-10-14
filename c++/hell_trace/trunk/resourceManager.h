/*
HellTrace by Przemysław Pastuszka
University of Wrocław, Poland

Stores all resources. 
*/

#ifndef HELLRESMANAGER
#define HELLRESMANAGER

#include <map>
#include "resource.h"
#include <string>

class ResourceManager
{
	std::map<std::string,Resource*> res;
public:
	ResourceManager() {}
	~ResourceManager();

	void Add(std::string name,Resource* pt);
	
	Resource* Get(std::string name);
};

#endif
