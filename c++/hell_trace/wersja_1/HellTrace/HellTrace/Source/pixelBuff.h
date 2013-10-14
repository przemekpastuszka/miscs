/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Pixel buffer is used to store info about all pixels of the image which consist of: color and depth of the pixel
*/

#ifndef HELLPIXBUFF
#define HELLPIXBUFF

#include "color.h"

class PixelBuff
{
public:
	struct Pixel
	{
		Color color;
		float depth;
		Pixel() {}
		Pixel(const Color& c,float d): color(c),depth(d) {}
	};
private:
	unsigned int width,height;
	Pixel* buff;
public:
	PixelBuff(unsigned int w,unsigned int h);
	PixelBuff(const PixelBuff& o);
	~PixelBuff();

	PixelBuff& operator=(const PixelBuff &o);
	
	//color operations
	Color GetColor(unsigned int x) const;
	Color GetColor(unsigned int x,unsigned int y) const;
	void SetColor(unsigned int x,const Color& value);
	void SetColor(unsigned int x,unsigned int y,const Color& value);

	//depth operations
	float GetDepth(unsigned int x) const;
	float GetDepth(unsigned int x,unsigned int y) const;
	void SetDepth(unsigned int x,float value);
	void SetDepth(unsigned int x,unsigned int y,float value);
	
	//pixel operations
	Pixel Get(unsigned int x) const;
	Pixel Get(unsigned int x,unsigned int y) const;
	void Set(unsigned int x,const Pixel& value);
	void Set(unsigned int x,unsigned int y,const Pixel& value);

	unsigned int Width() const {return width;}
	unsigned int Height() const {return height;}

	//clear the buffer with the specified color and the infinite (FLT_MAX) depth
	void Clear(const Color& a);

	//it squeezes buffer into smaller one. It is used when supersampling is on.
	void Downsample(PixelBuff& result) const;
};

#endif