#include <stdlib.h>

typedef struct node
{
	void* key,*value;
	node* next;
}NODE;

typedef struct list
{
	NODE* first;
}LIST;

void Add(LIST* n,void* key,void* value)
{
	NODE* x=(NODE*)malloc(sizeof(NODE));
	x->key=key;
	x->value=value;
	x->next=n->first;
	n->first=x;
}
int IsEmpty(LIST* n)
{
	if(n->first==NULL)
		return 1;
	return 0;
}
bool IsMember(LIST* ls,void* ptr)
{
	NODE* temp=ls->first;
	while(temp!=NULL)
	{
		if(temp->key==ptr)
			return true;
		temp=temp->next;
	}
	return false;
}
void* Get(LIST* ls,void* key)
{
	NODE* temp=ls->first;
	while(temp!=NULL)
	{
		if(temp->key==key)
			return temp->value;
		temp=temp->next;
	}
	return NULL;
}
void Destroy(NODE* n)
{
	if(n==NULL)
		return;
	Destroy(n->next);
	free(n);
}
void DestroyList(LIST* n)
{
	if(n==NULL)
		return;
	Destroy(n->first);
	free(n);
}

LIST* NewList()
{
	LIST* temp=(LIST*)malloc(sizeof(LIST));
	temp->first=NULL;
	return temp;
}

typedef struct vertex
{
	int value;
	LIST* succesors;
} VERTEX;

VERTEX* Map(VERTEX* wsk,int (*f)(int),LIST* visited)
{
	if(wsk==NULL)
		return NULL;
	VERTEX* temp=(VERTEX*)Get(visited,wsk);
	if(temp!=NULL)
		return temp;
	temp=(VERTEX*)malloc(sizeof(VERTEX));
	Add(visited,wsk,temp);
	temp->value=f(wsk->value);
	temp->succesors=NewList();
	NODE* ptr=wsk->succesors->first;
	while(ptr!=NULL)
	{
		Add(temp->succesors,NULL,Map((VERTEX*)ptr->value,f,visited));
		ptr=ptr->next;
	}
	return temp;
}

typedef struct graph
{
	LIST* vertices;
} GRAPH;

GRAPH* MapGraph(GRAPH* wsk,int (*f)(int))
{
	LIST* visited=NewList();
	GRAPH* result=(GRAPH*)malloc(sizeof(GRAPH));
	result->vertices=NewList();
	NODE* ptr=wsk->vertices->first;
	while(ptr!=NULL)
	{
		Add(result->vertices,NULL,Map((VERTEX*)ptr->value,f,visited));
		ptr=ptr->next;
	}
	DestroyList(visited);
	return result;
}

int main()
{
	return 0;
}
