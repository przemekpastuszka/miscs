#include "pixelBuff.h"
#include "log.h"

PixelBuff::PixelBuff(const PixelBuff &o)
{
	width=o.width;
	height=o.height;
	buff=new Color[width*height];
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
	buff=new Color[width*height];
	for(unsigned int i=0;i<width*height;++i)
		buff[i]=o.Get(i);
}

PixelBuff::PixelBuff(unsigned int w,unsigned int h) : width(w), height(h)
{
	buff=new Color[width*height];
}

PixelBuff::~PixelBuff()
{
	delete [] buff;
}

Color PixelBuff::Get(unsigned int x) const
{
	return buff[x];
}

Color PixelBuff::Get(unsigned int x, unsigned int y) const
{
	return buff[y*width+x];
}

void PixelBuff::Set(unsigned int x,const Color& value)
{
	buff[x]=value;
}

void PixelBuff::Set(unsigned int x,unsigned int y,const Color& value)
{
	buff[y*width+x]=value;
}

void PixelBuff::Clear(const Color& color)
{
	for(unsigned int i=0;i<width*height;++i)
		buff[i]=color;
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
			for(unsigned int x=0;x<scale;++x)
				for(unsigned int y=0;y<scale;++y)
					pix+=Get(i*scale+x,j*scale+y)*mul;
			result.Set(i,j,pix);
		}
}