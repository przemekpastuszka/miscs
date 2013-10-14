/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Implementation of 'useful' functions :)
*/

#include "utilities.h"
#include <stdio.h>
#include <fstream>
#include "log.h"
#include <math.h>
#include <sstream>
#include <cstdlib>

using namespace std;

PixelBuff* FromRAW(std::string fileName,int width,int height)
{
	std::string name=fileName+".raw";
	FILE* wej=fopen(name.c_str(),"rb");
	if(!wej)
		Log::AddMessage("Function FromRAW: Cannot open: '"+fileName+"'",Log::ERR);
		
	PixelBuff* buff=new PixelBuff(width,height);
	for(unsigned int i=0;i<height;++i)
		for(unsigned int j=0;j<width;++j)
		{
			unsigned char r,g,b;
			fread(&r,sizeof(char),1,wej);
			fread(&g,sizeof(char),1,wej);
			fread(&b,sizeof(char),1,wej);
			buff->SetColor(j,i,Color(((float)r)/255.0,((float)g)/255.0,((float)b)/255.0));
		}
	fclose(wej);
	return buff;
}

//save image from pixel buffer to file in raw format
void ToRAW(const PixelBuff* buff,std::string fileName)
{
	std::string name=fileName+".raw";
	FILE* wyj=fopen(name.c_str(),"wb");
	if(!wyj)
		Log::AddMessage("Function ToRAW: Cannot open: '"+fileName+"'",Log::ERR);
	for(unsigned int i=0;i<buff->Height();++i)
		for(unsigned int j=0;j<buff->Width();++j)
		{
			Vector3 rgb=buff->GetColor(j,i).GetRGB();
			char red=rgb.r;
			char green=rgb.g;
			char blue=rgb.b;
			fwrite(&red,sizeof(char),1,wyj);
			fwrite(&green,sizeof(char),1,wyj);
			fwrite(&blue,sizeof(char),1,wyj);
		}
	fclose(wyj);
}

PixelBuff* FromBMP(std::string fileName)
{
	std::string name=fileName+".bmp";
	FILE* wej=fopen(name.c_str(),"rb");
	if(!wej)
		Log::AddMessage("Function FromBMP: Cannot open: '"+fileName+"'",Log::ERR);
	
	fseek(wej,18,SEEK_SET);
	int width,height;
	fread(&width,sizeof(int),1,wej);
	fread(&height,sizeof(int),1,wej);
	fseek(wej,54,SEEK_SET);

	unsigned int rowSize=24*width;
	unsigned int realRowSize=rowSize;
	if(rowSize%32!=0)
		realRowSize=(rowSize/32+1)*32;;


	PixelBuff* buff=new PixelBuff(width,height);
	for(int i=height-1;i>=0;--i)
	{
		for(int j=0;j<width;++j)
		{
			unsigned char r,g,b;
			fread(&b,sizeof(char),1,wej);
			fread(&g,sizeof(char),1,wej);
			fread(&r,sizeof(char),1,wej);
			buff->SetColor(j,i,Color(((float)r)/255.0,((float)g)/255.0,((float)b)/255.0));
		}
		fseek(wej,(realRowSize-rowSize)/8,SEEK_CUR);
	}
	fclose(wej);
	return buff;

}

//save image from pixel buffer to file in bmp format
//if you want to know how it works check the wiki
void ToBMP(const PixelBuff* buff,std::string fileName)
{
	std::string name=fileName+".bmp";
	FILE* wyj=fopen(name.c_str(),"wb");
	if(!wyj)
		Log::AddMessage("Function ToBMP: Cannot open: '"+fileName+"'",Log::ERR);

	unsigned int rowSize=24*buff->Width();
	unsigned int realRowSize=rowSize;
	if(rowSize%32!=0)
		realRowSize=(rowSize/32+1)*32;;
	unsigned int fileSize=54+(realRowSize*buff->Height())/8;
	unsigned int imgSize=fileSize-54;
	unsigned int offset=54;
	unsigned int header=40;
	int width=buff->Width();
	int height=buff->Height();
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
	
	for(int i=buff->Height()-1;i>=0;--i)
	{
		for(int j=0;j<buff->Width();++j)
		{
			Vector3 rgb=buff->GetColor(j,i).GetRGB();
			char red=rgb.r;
			char green=rgb.g;
			char blue=rgb.b;
			fwrite(&blue,sizeof(char),1,wyj);
			fwrite(&green,sizeof(char),1,wyj);
			fwrite(&red,sizeof(char),1,wyj);
		}
		for(unsigned int i=rowSize;i<realRowSize;i+=8)
			fwrite("0",sizeof(char),1,wyj);
	}
	fclose(wyj);
}

//it's explained on wiki
float Power(float base,int exp)
{
	float result=base;
	exp=exp>>1;
	while(exp!=0)
	{
		if(exp%2)
			result*=result;
		else
			result*=result*base;
		exp=exp>>1;
	}
	return result;
}

//very simple format - stores uncompressed info about color (32-bit per channel) and depth value
void ToHLT(const PixelBuff* buff,std::string fileName)
{
	std::string name=fileName+".hlt";
	FILE* wyj=fopen(name.c_str(),"wb");
	if(!wyj)
		Log::AddMessage("Function ToHLT: Cannot open: '"+fileName+"'",Log::ERR);

	unsigned int width=buff->Width(),height=buff->Height();
	fwrite(&width,sizeof(unsigned int),1,wyj);
	fwrite(&height,sizeof(unsigned int),1,wyj);
	
	for(int i=0;i<buff->Width();++i)
		for(int j=0;j<buff->Height();++j)
		{
			Vector3 color=buff->GetColor(i,j).GetUncompressed();
			float depth=buff->GetDepth(i,j);
			fwrite(&color.r,sizeof(float),1,wyj);
			fwrite(&color.g,sizeof(float),1,wyj);
			fwrite(&color.b,sizeof(float),1,wyj);
			fwrite(&depth,sizeof(float),1,wyj);
		}
		
	fclose(wyj);
}

//reads HLT file
PixelBuff* FromHLT(std::string fileName)
{
	std::string name=fileName+".hlt";
	FILE* wej=fopen(name.c_str(),"rb");
	if(!wej)
		Log::AddMessage("Function FromHLT: Cannot open: '"+fileName+"'",Log::ERR);
	
	unsigned int width,height;
	fread(&width,sizeof(unsigned int),1,wej);
	fread(&height,sizeof(unsigned int),1,wej);
	
	PixelBuff* result=new PixelBuff(width,height);
	
	for(int i=0;i<result->Width();++i)
		for(int j=0;j<result->Height();++j)
		{
			float r,g,b,depth;
			fread(&r,sizeof(float),1,wej);
			fread(&g,sizeof(float),1,wej);
			fread(&b,sizeof(float),1,wej);
			fread(&depth,sizeof(float),1,wej);
			result->Set(i,j,PixelBuff::Pixel(Color(r,g,b),depth));
		}
	fclose(wej);
	return result;
}

std::string ToString(float a)
{
	std::ostringstream ss;
	ss<<a;
	return ss.str();
}

float ToFloat(std::string a)
{
	return atof(a.c_str());
}

std::vector<std::string> Split(std::string txt, std::string splitter)
{
	std::vector<std::string> result;
	for(;;)
	{
		size_t pos=txt.find(splitter);
		if(pos==string::npos)
			break;
		result.push_back(txt.substr(0,pos));
		txt=txt.substr(pos+splitter.length());
	}
	if(txt!="")
		result.push_back(txt);
	return result;
}
