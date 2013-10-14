/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Having infos about color and depth of each pixel we can apply some awesome
effects to our image, which do not depend on raytracing algorithm.
All implemented effects are provided by this class
*/

#ifndef HELLPOSTPROCESS
#define HELLPOSTPROCESS

#include "pixelBuff.h"

class PostProcess
{
	//just checks is the difference between two values enough to perform antialiasing
	static bool CheckDptDiff(float a,float b,float treshold);

	//tone mapping
	static float CalculateLuminance(const Color& c);
	static float CalculateAverageLum(const PixelBuff* c);
	static Color ToneMap(const Color& c, float avLum, float middleGrey, float whitePoint);
	
	//gaussian blur
	static void FillTheMask(float* mask,int radius,float sigma);
	static float BumpCoord(float coord,float min,float max);

public:
	//depth-based antialiasing - it gives results similar to supersampling, but is a lot faster
	static PixelBuff* DepthAntialiasing(const PixelBuff* sourceColor,const PixelBuff* sourceDepth,float treshold);

	//really simple tone mapping
	static PixelBuff* SimpleToneMapping(const PixelBuff* source,float exposure);

	//tone mapping using Reinhard operator
	static PixelBuff* AdvToneMapping(const PixelBuff* source,
		float middleGrey,float whitePoint);

	//applies gaussian blur to the in the image
	//affected pixels lie in a sqare which left upper corner is (leftUpCorner.x,leftUpCorner.y)
	//and right bottom is (leftBCorner.x,leftBCorner.y)
	//sigma is the standard deviation
	static PixelBuff* GaussianBlur(const PixelBuff* source,/*const Vector3& leftUpCorner,
		const Vector3& rightBCorner,*/ float sigma);

	//linear interpolation between two pixel buffers
	static PixelBuff* Lerp(const PixelBuff* source1,const PixelBuff* source2, float value);
	
	//compresses dark pixel, leaves bright areas
	static PixelBuff* BrightPass(const PixelBuff* source,float middleGrey,float whitePoint,
		float treshold,float offset);
};

#endif
