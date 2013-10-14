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
	static bool CheckDptDiff(float a,float b);
public:
	//depth-based antialiasing - it gives results similar to supersampling, but is a lot faster
	static void DepthAntialiasing(const PixelBuff& source,PixelBuff& destination); 
};

#endif