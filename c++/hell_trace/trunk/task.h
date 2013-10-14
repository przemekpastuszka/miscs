/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

At first I need to explain what is my concept of the input
I consider sphere, buffer, gaussian blur, raytracing and so on as 'tasks', so
when the parser finds something like this in the input file:
Sphere S1
{
	point 0,0,-7;
	radius 3;
}
it produces the task named 'S1' of type 'Sphere' with some attributes. It does not know, that Sphere will be an object.
This information is known only to the function that performs tasks of 'Sphere' type. And this function will create an object.
OK, so as far it looks tiny, but... there is one problem:
tasks may not appear in the appropriate order. Imagine: we can try perform raytracing before creating scene!
Here's a solution: 
When one task will use a product of another task you need to tell the parser about it by using 'input' keyword, so it
can create dependecy graph and perform topological sort to determine the order the task should be done.
*/

#ifndef HELLTASK
#define HELLTASK

#include <string>
#include <list>
#include <map>

struct Task
{
	struct TaskElement
	{
		std::string type;
		std::string value;
		bool isInput; //'input' keyword used

		TaskElement(std::string t,std::string v,bool is) : type(t),value(v),isInput(is) {}
		TaskElement() : isInput(false) {}
	};
	std::string name;
	std::string type;
	bool visited,needed; //template variables used by topological sort
	std::map<std::string,TaskElement> elements;
	std::list<std::string> succesors; //list of tasks which require this one to be done earlier 

	Task(std::string n,std::string t) : name(n),type(t),visited(false),needed(false) {}
	Task() : visited(false),needed(false) {}
};

class TaskManager
{
	std::map<std::string,Task*> unsorted; 
	std::list<Task*> sorted;

	//topological sort is done by DFS
	bool Visit(std::string name); 
public:
	TaskManager() {}
	~TaskManager();

	void FromFile(std::string name); //reads tasks from file
	void AddTask(Task* t); //you can add tasks manually
	void SortTasks(); //topological sort
	
	Task* GetTask(); //returns next task. NULL when there is none
};
#endif