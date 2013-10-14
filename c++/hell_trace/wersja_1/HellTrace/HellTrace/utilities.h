#ifndef HELLUTS
#define HELLUTS

#include "pixelBuff.h"
#include <string>

/*Funkcje, które przydadz¹ siê w trakcie dzia³ania raytracera*/

//zapis bufora pikseli do pliku RAW
void ToRAW(const PixelBuff& buff,std::string fileName);
void ToBMP(const PixelBuff& buff,std::string fileName);

float Power(float base,int exp);

#endif