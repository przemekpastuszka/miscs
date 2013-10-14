/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Useful functions
*/

#ifndef HELLUTS
#define HELLUTS

#include "pixelBuff.h"
#include <string>

//saving image to file
void ToRAW(const PixelBuff& buff,std::string fileName);
void ToBMP(const PixelBuff& buff,std::string fileName);

//quick exponentation (?) (I don't know what's correct english name for it)
float Power(float base,int exp);

#endif