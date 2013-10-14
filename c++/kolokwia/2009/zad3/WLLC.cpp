#include "WLLC.h"
#include <stdlib.h>

WLLC copy(WLLC ls)
{
	WLLC res=ls;
	WLLC* t1=&res,*t2=&ls;
	while(t2->next!=NULL)
	{
		t1->next=(WLLC*)malloc(sizeof(WLLC));
		t1->next->x=t2->next->x;
		t1->next->next=NULL;
		t1->next->child=NULL;
		if(t2->next->child!=NULL)
		{
			WLLC temp=copy(*t2->next->child);
			WLLC* temp2=(WLLC*)malloc(sizeof(WLLC));
			temp2->x=temp.x;
			temp2->child=temp.child;
			temp2->next=temp.next;
			t1->next->child=temp2;
		}
		t1=t1->next;
		t2=t2->next;
	}
	return res;
}

WLLC merge(WLLC l1,WLLC l2)
{
	WLLC t1=copy(l1),t2=copy(l2);
	WLLC* temp=&t1;
	while(temp->next!=NULL)
		temp=temp->next;
	temp->next=(WLLC*)malloc(sizeof(WLLC));
	temp->next->x=t2.x;
	temp->next->next=t2.next;
	temp->next->child=t2.child;
	return t1;
}

WLLC flat(WLLC li)
{
	WLLC p=copy(li);
	WLLC* temp=&p;
	WLLC* prev=NULL;
	while(temp!=NULL)
	{
		prev=temp;
		if(temp->child!=NULL)
		{
			if(prev!=NULL)
				prev->next=temp->child;
			WLLC* t=temp->child;
			while(t->next!=NULL)
				t=t->next;
			t->next=temp->next;
			prev=t;
		}
		temp=temp->next;
	}
	if(p.child!=NULL)
		return *p.child;
	return p;
}

