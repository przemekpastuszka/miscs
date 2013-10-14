/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Implementation of task manager
*/

#include "task.h"
#include "log.h"
#include "lexer.h"
#include "utilities.h"

TaskManager::~TaskManager()
{
	for(std::map<std::string,Task*>::iterator it=unsorted.begin();it!=unsorted.end();++it)
		delete (*it).second;
}

void TaskManager::AddTask(Task *t)
{
	unsorted[t->name]=t;
}

Task* TaskManager::GetTask()
{
	if(sorted.empty())
		return NULL;
	Task* temp=*sorted.begin();
	sorted.erase(sorted.begin());
	return temp;
}

void TaskManager::SortTasks()
{
	//creating graph
	//foreach task check what other tasks need to be completed first
	for(std::map<std::string,Task*>::iterator i=unsorted.begin();i!=unsorted.end();++i)
	{
		Task* t=(*i).second;
		for(std::map<std::string,Task::TaskElement>::iterator it=t->elements.begin();it!=t->elements.end();++it)
			if((*it).second.isInput) //we need to add the edge
			{
				std::map<std::string,Task*>::iterator found=unsorted.find((*it).second.value);
				if(found==unsorted.end())
					Log::AddMessage("Cannot find "+(*it).second.value+".",Log::ERR);
				(*found).second->succesors.push_front(t->name); //adding the edge
			}
	}

	//sort
	for(std::map<std::string,Task*>::iterator i=unsorted.begin();i!=unsorted.end();++i)
		Visit((*i).first);
}

//topological sort using DFS
bool TaskManager::Visit(std::string name)
{
	std::map<std::string,Task*>::iterator found=unsorted.find(name);
	Task* t=(*found).second;
	if(t->visited)
		return t->needed;
	t->visited=true;

	//we need to check do we have any redundant tasks, which means that results of they work
	//will be not used to create final image. This check is really simple: we need only to determine
	//is there a path in our graph which leads from examined task to any task of 'ImageSaver' type.
	//Simple one.
	bool isNeeded=false;
	if(t->type=="ImageSaver")
		isNeeded=true;

	for(std::list<std::string>::iterator it=t->succesors.begin();it!=t->succesors.end();++it)
		if(Visit(*it))
			isNeeded=true;

	if(isNeeded)
	{
		t->needed=true;
		sorted.push_front(t);
	}
	return isNeeded;
}


//parsing the file...
//messy piece of code. I promise to recode it... some day ;)
void TaskManager::FromFile(std::string name)
{
	Lexer lex(name);
	Lexer::Token temp;
	while((temp=lex.GetToken()).type!=Lexer::END)
	{
		Task* t=new Task();

		//reads type of the task
		if(temp.type!=Lexer::WORD)
			Log::AddMessage("Unexpected token on line: "+ToString(temp.line)+".",Log::ERR);
		t->type=temp.value;

		//its name
		temp=lex.GetToken();
		if(temp.type!=Lexer::WORD)
			Log::AddMessage("Unexpected token on line: "+ToString(temp.line)+".",Log::ERR);
		t->name=temp.value;
		
		//opening bracket
		temp=lex.GetToken();
		if(temp.type!=Lexer::LBRACKET)
			Log::AddMessage("Unexpected token on line: "+ToString(temp.line)+".",Log::ERR);

		while((temp=lex.GetToken()).type!=Lexer::RBRACKET)
		{
			Task::TaskElement el;

			if(temp.type!=Lexer::WORD)
				Log::AddMessage("Unexpected token on line: "+ToString(temp.line)+".",Log::ERR);
			if(temp.value=="input")
			{
				el.isInput=true;

				temp=lex.GetToken();
				if(temp.type!=Lexer::WORD)
					Log::AddMessage("Unexpected token on line: "+ToString(temp.line)+".",Log::ERR);
			}
			el.type=temp.value;
	
			temp=lex.GetToken();
			if(temp.type!=Lexer::WORD)
				Log::AddMessage("Unexpected token on line: "+ToString(temp.line)+".",Log::ERR);
			el.value=temp.value;

			temp=lex.GetToken();
			if(temp.type!=Lexer::SEMICOLON)
				Log::AddMessage("Unexpected token on line: "+ToString(temp.line)+".",Log::ERR);

			t->elements[el.type]=el;
		}
		AddTask(t);
	}
}