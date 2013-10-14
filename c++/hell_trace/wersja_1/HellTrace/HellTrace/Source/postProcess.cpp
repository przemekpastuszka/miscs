/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Implementation of postprocessing
*/

#include "postProcess.h"
#include "log.h"
#include "common.h"

bool PostProcess::CheckDptDiff(float a, float b)
{
	float max=a>b?a:b;
	float min=a>b?b:a;
	if(max/min>ALIASTRESHOLD) //if difference is higher than treshold, antialiasing should be performed
		return true;
	return false;
}

//depth-based antialiasing
//it divides the image into 2x2 sqares and foreach it checks whether the difference between depth is big
//enough to suppose that it lies on the object's edge. Then: antialiasing is done by simple mixing of colors
void PostProcess::DepthAntialiasing(const PixelBuff &source, PixelBuff &dst)
{
	if(source.Width()!=dst.Width()||source.Height()!=dst.Height())
		Log::AddMessage("PostProcess::DepthAntialiasing - Wrong size of destination buffer",Log::ERR);

	dst=source;
	for(int i=0;i<source.Width()-1;++i)
	{
		for(int j=0;j<source.Height()-1;++j)
		{
			//check if antialiasing is needed
			if(CheckDptDiff(source.GetDepth(i,j),source.GetDepth(i,j+1))||
				CheckDptDiff(source.GetDepth(i,j),source.GetDepth(i+1,j+1))||
				CheckDptDiff(source.GetDepth(i,j),source.GetDepth(i+1,j))||
				CheckDptDiff(source.GetDepth(i,j+1),source.GetDepth(i+1,j))||
				CheckDptDiff(source.GetDepth(i,j+1),source.GetDepth(i+1,j+1)))
			{ //mix colors
				dst.SetColor(i,j,(source.GetColor(i,j)+source.GetColor(i,j+1)
					+source.GetColor(i+1,j)+source.GetColor(i+1,j+1))*0.25);
				dst.SetDepth(i,j,(source.GetDepth(i,j)+source.GetDepth(i,j+1)
					+source.GetDepth(i+1,j)+source.GetDepth(i+1,j+1))*0.25);
			}
		}
	}
}