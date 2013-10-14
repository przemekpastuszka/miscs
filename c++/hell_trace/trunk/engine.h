/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

The heart of raytracing system.
Detailed comments can be found inside engine.cpp file
*/

#ifndef ENGINEHELL
#define ENGINEHELL

#include "resource.h"
#include "camera.h"
#include "scene.h"
#include "pixelBuff.h"

class Engine
{
	Scene* scene;
	PixelBuff::Pixel RayTrace(const Ray& a,int dpt) const;
public:
	Engine(Scene* s) : scene(s) {} //at the beginning you need to set a scene
	~Engine();
	//render using Camera c to buffer p
	PixelBuff* Render(const Camera* c,int width,int height,int tracingDepth) const;
};

#endif
