#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char* argv[])
{
	FILE* wej=fopen(argv[1],"rb");
	if(!wej)
		printf("Cannot open a file");
	int maxLen=0,currPos=0,firstPos=0,currFirst=0,lastN=-32767;
	while(!feof(wej))
	{
		int x;
		fread(&x,sizeof(int),1,wej);
		if(x<=lastN)
		{
			if(currPos-currFirst>maxLen)
			{
				maxLen=currPos-currFirst;
				firstPos=currFirst;
			}
			currFirst=currPos;
		}
		lastN=x;
		currPos++;
	}
	fclose(wej);

	char* output=(char*)malloc(strlen(argv[1])+5);
	strcpy(output,argv[1]);
	strcpy(&output[strlen(argv[1])],".out");

	FILE *wyj;
	wyj=fopen(output,"wb");
	wej=fopen(argv[1],"rb");

	int x;
	for(int i=0;i<firstPos;++i)
		fread(&x,sizeof(int),1,wej);
	for(int i=0;i<maxLen;++i)
	{
		fread(&x,sizeof(int),1,wej);
		fwrite(&x,sizeof(int),1,wyj);
		//printf("%d ",x);
	}

	free(output);
	fclose(wej);
	fclose(wyj);
	return 0;
}

