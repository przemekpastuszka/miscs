#include <stdio.h>

int main()
{
	int array[]={1,2,5,1,3,5,534,34,6,3,23,3,876,7,6,567567,32,34,4,3,4,5,6,15,34,32,37,4,65,6,7,32,244};
	FILE* wyj=fopen("in","wb");
	fwrite(array,sizeof(int),33,wyj);
	fclose(wyj);
	return 0;
}
