/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Nothing special, really
*/

#include "pixelBuff.h"
#include "log.h"
#include <cfloat>

PixelBuff::PixelBuff(const PixelBuff &o)
{
	width=o.width;
	height=o.height;
	buff=new Pixel[width*height];
	for(unsigned int i=0;i<width*height;++i)
		buff[i]=o.Get(i);
}

PixelBuff& PixelBuff::operator =(const PixelBuff &o)
{
	if(this==&o)
		return *this;
	width=o.width;
	height=o.height;
	delete [] buff;
	buff=new Pixel[width*height];
	for(unsigned int i=0;i<width*height;++i)
		buff[i]=o.Get(i);
}

PixelBuff::PixelBuff(unsigned int w,unsigned int h) : width(w), height(h)
{
	buff=new Pixel[width*height];
}

PixelBuff::~PixelBuff()
{
	delete [] buff;
}

Color PixelBuff::GetColor(unsigned int x) const
{
	return buff[x].color;
}

Color PixelBuff::GetColor(unsigned int x, unsigned int y) const
{
	return buff[y*width+x].color;
}

void PixelBuff::SetColor(unsigned int x,const Color& value)
{
	buff[x].color=value;
}

void PixelBuff::SetColor(unsigned int x,unsigned int y,const Color& value)
{
	buff[y*width+x].color=value;
}

float PixelBuff::GetDepth(unsigned int x) const
{
	return buff[x].depth;
}

float PixelBuff::GetDepth(unsigned int x, unsigned int y) const
{
	return buff[y*width+x].depth;
}

void PixelBuff::SetDepth(unsigned int x,float value)
{
	buff[x].depth=value;
}

void PixelBuff::SetDepth(unsigned int x,unsigned int y,float value)
{
	buff[y*width+x].depth=value;
}

PixelBuff::Pixel PixelBuff::Get(unsigned int x) const
{
	return buff[x];
}

PixelBuff::Pixel PixelBuff::Get(unsigned int x, unsigned int y) const
{
	return buff[y*width+x];
}

void PixelBuff::Set(unsigned int x,const PixelBuff::Pixel& value)
{
	buff[x]=value;
}

void PixelBuff::Set(unsigned int x,unsigned int y,const PixelBuff::Pixel& value)
{
	buff[y*width+x]=value;
}

void PixelBuff::Clear(const Color& color)
{
	for(unsigned int i=0;i<width*height;++i)
	{
		buff[i].color=color;
		buff[i].depth=FLT_MAX;
	}
}

void PixelBuff::Downsample(PixelBuff& result) const
{
	unsigned int scale=width/result.width;
	if(height/result.height!=scale||width%scale!=0||height%scale!=0)
		Log::AddMessage("PixelBuff::Downsample - Wrong scale",Log::ERR);

	float mul=1.0f/(scale*scale);
	for(unsigned int i=0;i<width/scale;++i)
		for(unsigned int j=0;j<height/scale;++j)
		{
			Color pix(0,0,0);
			float depth=0;
			for(unsigned int x=0;x<scale;++x)
				for(unsigned int y=0;y<scale;++y)
				{
					pix+=GetColor(i*scale+x,j*scale+y)*mul;
					depth+=GetDepth(i*scale+x,j*scale+y)*mul;
				}
			result.Set(i,j,Pixel(pix,depth));
		}
}