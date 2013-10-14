/*
HellTrace by Przemysław Pastuszka
University of Wrocław, Poland

Implementation of resource manager
*/

#include "resourceManager.h"
#include "log.h"

void ResourceManager::Add(std::string name,Resource *pt)
{
	std::map<std::string,Resource*>::iterator pos=res.find(name);
	if(pos!=res.end())
		Log::AddMessage("ResourceManager: Resource "+name+" already exisits",Log::ERR);
	res[name]=pt;
}
Resource* ResourceManager::Get(std::string name)
{
	std::map<std::string,Resource*>::iterator pos=res.find(name);
	if(pos!=res.end())
		return (*pos).second;
	return NULL;
}
ResourceManager::~ResourceManager()
{
	for(std::map<std::string,Resource*>::iterator it=res.begin();it!=res.end();++it)
		delete (*it).second;
}

