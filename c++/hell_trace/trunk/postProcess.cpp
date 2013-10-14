/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Implementation of postprocessing
*/

#define _USE_MATH_DEFINES
#include "postProcess.h"
#include "log.h"
#include "common.h"
#include <math.h>

bool PostProcess::CheckDptDiff(float a, float b,float treshold)
{
	float max=a>b?a:b;
	float min=a>b?b:a;
	if(max/min>treshold) //if difference is higher than treshold, antialiasing should be performed
		return true;
	return false;
}

//depth-based antialiasing
//it divides the image into 2x2 sqares and foreach it checks whether the difference between depth is big
//enough to suppose that it lies on the object's edge. Then: antialiasing is done by simple mixing of colors
PixelBuff* PostProcess::DepthAntialiasing(const PixelBuff* sColor,const PixelBuff* sDepth, float treshold)
{
	if(sColor->Width()!=sDepth->Width()||sColor->Height()!=sDepth->Height())
		Log::AddMessage("PostProcess::DepthAntialiasing - sizes of color and depth buffers don't match",Log::ERR);

	PixelBuff* dst=new PixelBuff(*sColor);
	
	for(int i=0;i<sColor->Width()-1;++i)
	{
		for(int j=0;j<sColor->Height()-1;++j)
		{
			//check if antialiasing is needed
			if(CheckDptDiff(sDepth->GetDepth(i,j),sDepth->GetDepth(i,j+1),treshold)||
				CheckDptDiff(sDepth->GetDepth(i,j),sDepth->GetDepth(i+1,j+1),treshold)||
				CheckDptDiff(sDepth->GetDepth(i,j),sDepth->GetDepth(i+1,j),treshold)||
				CheckDptDiff(sDepth->GetDepth(i,j+1),sDepth->GetDepth(i+1,j),treshold)||
				CheckDptDiff(sDepth->GetDepth(i,j+1),sDepth->GetDepth(i+1,j+1),treshold))
			{ //mix colors
				dst->SetColor(i,j,(sColor->GetColor(i,j)+sColor->GetColor(i,j+1)
					+sColor->GetColor(i+1,j)+sColor->GetColor(i+1,j+1))*0.25);
			}
		}
	}
	return dst;
}

//simple and naive tone mapping, but, still, better than clamping values to RGB
PixelBuff* PostProcess::SimpleToneMapping(const PixelBuff *source, float exposure)
{
	PixelBuff* dst=new PixelBuff(source->Width(),source->Height());

	for(int i=0;i<source->Width();++i)
		for(int j=0;j<source->Height();++j)
		{
			Vector3 tmp=source->GetColor(i,j).GetUncompressed();
			dst->SetColor(i,j,Color(1-expf(-exposure*tmp.r),1-expf(-exposure*tmp.g),1-expf(-exposure*tmp.b)));
		}
	return dst;
}

//calculates luminance. It's done by computing dot product of color and some magic value
float PostProcess::CalculateLuminance(const Color &c)
{
	Vector3 lumConverter(0.299,0.587,0.114);
	return Vector3::Dot(c.GetUncompressed(),lumConverter);
}

//logarithmic average of luminance
float PostProcess::CalculateAverageLum(const PixelBuff *c)
{
	double result=0;
	for(int i=0;i<c->Width();++i)
		for(int j=0;j<c->Height();++j)
			result+=log(1e-5+CalculateLuminance(c->GetColor(i,j)));
	result/=c->Width()*c->Height();
	return static_cast<float>(exp(result));
}

//use Reinhard operator to determine new color
Color PostProcess::ToneMap(const Color &c, float avLum,float middleGrey,float whitePoint)
{
	float colorLum=CalculateLuminance(c);
	float scaledLum=(colorLum*middleGrey)/avLum;
	float compLum=(scaledLum*(1+(scaledLum/(whitePoint*whitePoint))))/(1+scaledLum);
	return c*compLum;
}

//tone mapping with Reinhard operator done for whole scene
PixelBuff* PostProcess::AdvToneMapping(const PixelBuff *source,
	float middleGrey,float whitePoint)
{
	PixelBuff* dst=new PixelBuff(source->Width(),source->Height());

	float avLum=CalculateAverageLum(source);
	for(int i=0;i<source->Width();++i)
		for(int j=0;j<source->Height();++j)
			dst->SetColor(i,j,ToneMap(source->GetColor(i,j),avLum,middleGrey,whitePoint));
	return dst;
}

//fill blur mask with values of gaussian function
void PostProcess::FillTheMask(float *mask, int radius,float sigma)
{
	float g=1/(sigma*(float)sqrt(2*M_PI));
	for(int i=0;i<=radius;++i)
		mask[i+radius]=mask[radius-i]=g*expf(-(i*i)/(2*sigma*sigma));
}

//used to avoid situations when mask is on the edge of the image
float PostProcess::BumpCoord(float coord,float min, float max)
{
	coord=coord>0?coord:-coord;
	coord=coord<max?coord:2*max-coord;
	return coord;
}

//makes mask and applies it vertically and horizontaly to the image
PixelBuff* PostProcess::GaussianBlur(const PixelBuff *source,  
	/*const Vector3 &leftUpCorner, const Vector3 &rightBCorner,*/ float sigma)
{
	int radius=ceil(3*sigma);
	float *mask=new float[2*radius+1];
	FillTheMask(mask,radius,sigma);
	
	//just checking if we're still inside the image
	/*int leftX=(int)leftUpCorner.x,leftY=(int)leftUpCorner.y,
		rightX=(int)rightBCorner.x,rightY=(int)rightBCorner.y;
	leftX=leftX>0?(leftX<source.Width()?leftX:source.Width()-1):0;
	rightX=rightX>0?(rightX<source.Width()?rightX:source.Width()-1):0;
	leftY=leftY>0?(leftY<source.Height()?leftY:source.Height()-1):0;
	rightY=rightY>0?(rightY<source.Height()?rightY:source.Height()-1):0;*/
	int leftX=0,leftY=0;
	int rightX=source->Width()-1,rightY=source->Height()-1;

	PixelBuff vert(source->Width(),source->Height());
	PixelBuff* hor=new PixelBuff(source->Width(),source->Height());
	//applying the vertical mask
	for(int x=leftX;x<=rightX;++x)
		for(int y=leftY;y<=rightY;++y)
		{
			Color result;
			for(int i=-radius;i<=radius;++i)
				result+=source->GetColor(x,BumpCoord(y+i,0,source->Height()-1))*mask[i+radius];
			vert.SetColor(x,y,result);
		}

	//applying the horizontal mask
	for(int x=leftX;x<=rightX;++x)
		for(int y=leftY;y<=rightY;++y)
		{
			Color result;
			for(int i=-radius;i<=radius;++i)
				result+=vert.GetColor(BumpCoord(x+i,0,source->Width()-1),y)*mask[i+radius];
			hor->SetColor(x,y,result);
		}
		
	delete mask;
	return hor;
}

//blending two sources using linear interpolation
PixelBuff* PostProcess::Lerp(const PixelBuff *source1, const PixelBuff *source2, float value)
{
	if(source1->Width()!=source2->Width()||source1->Height()!=source2->Height())
		Log::AddMessage("PostProcess::Lerp - sources differs in size",Log::ERR);

	PixelBuff* dst=new PixelBuff(source1->Width(),source1->Height());

	for(int i=0;i<source1->Width();++i)
		for(int j=0;j<source1->Height();++j)
			dst->SetColor(i,j,Color::Lerp(source1->GetColor(i,j),source2->GetColor(i,j),value));
	return dst;
}

//compresses dark pixel, leaves bright areas
PixelBuff* PostProcess::BrightPass(const PixelBuff *source, float middleGrey,float whitePoint,
		float treshold,float offset)
{
	float avLum=PostProcess::CalculateAverageLum(source);
	
	PixelBuff* dst=new PixelBuff(source->Width(),source->Height());
	
	for(int i=0;i<source->Width();++i)
		for(int j=0;j<source->Height();++j)
		{
			float colorLum=CalculateLuminance(source->GetColor(i,j));
			float scaledLum=(colorLum*middleGrey)/avLum;
			float tmp=scaledLum*(1+scaledLum/(whitePoint*whitePoint))-treshold;
			tmp=tmp>0?tmp:0;
			dst->SetColor(i,j,(tmp/(offset+tmp))*source->GetColor(i,j));
		}
	return dst;
}
