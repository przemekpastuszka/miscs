#include "utilities.h"
#include <stdio.h>
#include <fstream>
#include "log.h"
#include <math.h>

using namespace std;
void ToRAW(const PixelBuff& buff,std::string fileName)
{
	std::string name=fileName+".raw";
	FILE* wyj=fopen(name.c_str(),"wb");
	if(!wyj)
		Log::AddMessage("Function ToRAW: Cannot open: '"+fileName+"'",Log::ERR);
	for(unsigned int i=0;i<buff.Height();++i)
		for(unsigned int j=0;j<buff.Width();++j)
		{
			Vector3 rgb=buff.Get(j,i).GetRGB();
			char red=rgb.r;
			char green=rgb.g;
			char blue=rgb.b;
			fwrite(&red,sizeof(char),1,wyj);
			fwrite(&green,sizeof(char),1,wyj);
			fwrite(&blue,sizeof(char),1,wyj);
		}
	fclose(wyj);
}

void ToBMP(const PixelBuff& buff,std::string fileName)
{
	std::string name=fileName+".bmp";
	FILE* wyj=fopen(name.c_str(),"wb");
	if(!wyj)
		Log::AddMessage("Function ToBMP: Cannot open: '"+fileName+"'",Log::ERR);

	unsigned int rowSize=24*buff.Width();
	unsigned int realRowSize=rowSize;
	if(rowSize%32!=0)
		realRowSize=(rowSize/32+1)*32;;
	unsigned int fileSize=54+(realRowSize*buff.Height())/8;
	unsigned int imgSize=fileSize-54;
	unsigned int offset=54;
	unsigned int header=40;
	int width=buff.Width();
	int height=buff.Height();
	unsigned short int zero=0;
	unsigned short int one=1;
	unsigned short int dpt=24;
	unsigned int biggerZero=0;
	int res=0x130B;
	fwrite("BM",sizeof(char),2,wyj);
	fwrite(&fileSize,sizeof(unsigned int),1,wyj);
	fwrite(&zero,sizeof(unsigned short int),1,wyj);
	fwrite(&zero,sizeof(unsigned short int),1,wyj);
	fwrite(&offset,sizeof(unsigned int),1,wyj);
	fwrite(&header,sizeof(unsigned int),1,wyj);
	fwrite(&width,sizeof(int),1,wyj);
	fwrite(&height,sizeof(int),1,wyj);
	fwrite(&one,sizeof(unsigned short int),1,wyj);
	fwrite(&dpt,sizeof(unsigned short int),1,wyj);
	fwrite(&biggerZero,sizeof(unsigned int),1,wyj);
	fwrite(&imgSize,sizeof(unsigned int),1,wyj);
	fwrite(&res,sizeof(int),1,wyj);
	fwrite(&res,sizeof(int),1,wyj);
	fwrite(&biggerZero,sizeof(unsigned int),1,wyj);
	fwrite(&biggerZero,sizeof(unsigned int),1,wyj);
	
	for(int i=buff.Height()-1;i>=0;--i)
	{
		for(int j=0;j<buff.Width();++j)
		{
			Vector3 rgb=buff.Get(j,i).GetRGB();
			char red=rgb.r;
			char green=rgb.g;
			char blue=rgb.b;
			//wyj<<blue<<green<<red;
			fwrite(&blue,sizeof(char),1,wyj);
			fwrite(&green,sizeof(char),1,wyj);
			fwrite(&red,sizeof(char),1,wyj);
		}
		for(unsigned int i=rowSize;i<realRowSize;i+=8)
			fwrite("0",sizeof(char),1,wyj);
	}
	fclose(wyj);
}

float Power(float base,int exp)
{
	if(exp==0)
		return 1;
	float res=Power(base,exp/2);
	if(exp%2==0)
		return res*res;
	return res*res*base;
}