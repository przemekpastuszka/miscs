#ifndef HELLPIXBUFF
#define HELLPIXBUFF

#include "color.h"

class PixelBuff
{
	unsigned int width,height;
	Color* buff;
public:
	PixelBuff(unsigned int w,unsigned int h);
	PixelBuff(const PixelBuff& o);
	~PixelBuff();

	PixelBuff& operator=(const PixelBuff &o);
	
	Color Get(unsigned int x) const;
	Color Get(unsigned int x,unsigned int y) const;
	void Set(unsigned int x,const Color& value);
	void Set(unsigned int x,unsigned int y,const Color& value);

	unsigned int Width() const {return width;}
	unsigned int Height() const {return height;}

	void Clear(const Color& a);

	void Downsample(PixelBuff& result) const;
};

#endif