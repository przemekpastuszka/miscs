#ifndef ENGINEHELL
#define ENGINEHELL

#include "camera.h"
#include "scene.h"
#include "pixelBuff.h"

class Engine
{
	Scene* scene;
	Color RayTrace(const Ray& a,int dpt) const;
public:
	Engine(Scene* s) : scene(s) {}
	~Engine();
	void Render(const Camera* c,PixelBuff& p) const;
};

#endif