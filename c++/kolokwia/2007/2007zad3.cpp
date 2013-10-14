#include <stdlib.h>


typedef struct wezel 
{
	int x;
	struct wezel *lewy, *prawy;
} WEZEL;

WEZEL* mapTree(WEZEL* wsk,int (*f)(int))
{
	if(wsk==NULL)
		return NULL;
	WEZEL* nowy=(WEZEL*)malloc(sizeof(WEZEL));
	nowy->x=f(wsk->x);
	nowy->lewy=mapTree(wsk->lewy,f);
	nowy->prawy=mapTree(wsk->prawy,f);
	return nowy;
}

/*typedef struct vertex 
{
	int x;
	struct wezel *next;
} VERTEX;*/

int main(int argc, char* argv[])
{

	return 0;
}

