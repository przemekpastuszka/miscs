/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Useful functions
*/

#ifndef HELLUTS
#define HELLUTS

#include "pixelBuff.h"
#include <string>
#include <vector>

//saving image to file
void ToRAW(const PixelBuff* buff,std::string fileName);
void ToBMP(const PixelBuff* buff,std::string fileName);
void ToHLT(const PixelBuff* buff,std::string fileName); //special, uncompressed format (32-bit per channel + depth)

//reading image from file
PixelBuff* FromBMP(std::string fileName);
PixelBuff* FromRAW(std::string fileName,int width,int height);
PixelBuff* FromHLT(std::string fileName);

//quick (fast?) exponentation (?) (I don't know what's correct english name for it)
float Power(float base,int exp);

//simple conversions
std::string ToString(float a);
float ToFloat(std::string a);

//string operations
std::vector<std::string> Split(std::string text,std::string splitter);

#endif
