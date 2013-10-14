#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	char x;
	node* next;
}NODE;

typedef struct list
{
	NODE* first;
}LIST;

void Add(LIST* n,char c)
{
	NODE* x=(NODE*)malloc(sizeof(NODE));
	x->x=c;
	x->next=n->first;
	n->first=x;
}
int IsEmpty(LIST* n)
{
	if(n->first==NULL)
		return 1;
	return 0;
}

char Get(LIST* n)
{
	if(n->first==NULL)
		return '-';
	char res=n->first->x;
	NODE* temp=n->first;
	n->first=n->first->next;
	free(temp);
	return res;
}

int main(int argc, char* argv[])
{
	LIST* ls=(LIST*)malloc(sizeof(LIST));
	ls->first=NULL;
	
	FILE* wej=fopen(argv[1],"r");
	int shut=0;
	while(!feof(wej))
	{
		char x=(char)fgetc(wej);
		switch(x)
		{
			case '{':
			case '[':
			case '(':
				{
					Add(ls,x);
					break;
				}
			case '}':
			{
				if(IsEmpty(ls)||Get(ls)!='{')
					shut=1;
				break;
			}
			case ']':
			{
				if(IsEmpty(ls)||Get(ls)!='[')
					shut=1;
				break;
			}
			case ')':
			{
				if(IsEmpty(ls)||Get(ls)!='(')
					shut=1;
				break;
			}
		}
		if(shut)
			break;
	}
	if(shut||(!IsEmpty(ls)))
		printf("Nawiasy zle rozstawione\n");
	else
		printf("Nawiasy dobrze rozstawione\n");
	fclose(wej);
	return 0;
}
