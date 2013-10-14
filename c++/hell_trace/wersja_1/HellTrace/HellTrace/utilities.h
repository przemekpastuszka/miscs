#ifndef HELLUTS
#define HELLUTS

#include "pixelBuff.h"
#include <string>

/*Funkcje, kt�re przydadz� si� w trakcie dzia�ania raytracera*/

//zapis bufora pikseli do pliku RAW
void ToRAW(const PixelBuff& buff,std::string fileName);
void ToBMP(const PixelBuff& buff,std::string fileName);

float Power(float base,int exp);

#endif