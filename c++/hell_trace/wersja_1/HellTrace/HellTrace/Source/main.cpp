#include <stdio.h>
#include "utilities.h"
#include "simple.h"
#include "sphere.h"
#include "plane.h"
#include "phong.h"
#include "solid.h"
#include "scene.h"
#include "globalLight.h"
#include "pointLight.h"
#include "areaLight.h"
#include "orthoCam.h"
#include "conicCam.h"
#include "engine.h"
#include "postProcess.h"

//#define ANTIALIAS

int main()
{
	int width=640,height=480;
#ifndef ANTIALIAS
	PixelBuff image(width,height);
	Camera* cam=new ConicCam(width,height);
#else
	PixelBuff image(2*width,2*height);
	Camera* cam=new ConicCam(2*width,2*height);
#endif
	//int width=1024,height=768;
	
	/*PixelBuff image(2*width,2*height);
	Camera* cam=new ConicCam(2*width,2*height);*/

	Scene scene(Vector3(10,10,10));
	scene.AddObject(new Simple(new Sphere(Vector3(0,0,16),3),new Phong(Color(1,0,0),Color(0.6,0.6,0.6),0.9,0.1,100)));
	scene.AddObject(new Simple(new Sphere(Vector3(8,0,12),3),new Phong(Color(0,1,0),Color(0.6,0.6,0.6),0.9,0.1,100)));
	scene.AddObject(new Simple(new Sphere(Vector3(-8,0,14),3),new Phong(Color(0,0,1),Color(0.6,0.6,0.6),0.9,0.1,100)));
	scene.AddObject(new Simple(new Sphere(Vector3(0,4,23),5),new Phong(Color(1,1,0),Color(0.6,0.6,0.6),0.8,0.2,100)));
	scene.AddObject(new Simple(new Plane(Vector3(0,0,-1),28),new Phong(Color(1,0,0),Color(0,0,0),0.9,0,100)));
	scene.AddObject(new Simple(new Plane(Vector3(-1,0,0),15),new Phong(Color(0,1,0),Color(0,0,0),0.9,0,100)));
	scene.AddObject(new Simple(new Plane(Vector3(1,0,0),15),new Phong(Color(0,0,1),Color(0,0,0),0.9,0,100)));
	scene.AddObject(new Simple(new Plane(Vector3(0,1,0),15),new Phong(Color(1,0,1),Color(0,0,0),0.9,0,100)));
	scene.AddObject(new Simple(new Plane(Vector3(0,-1,0),15),new Phong(Color(0,1,1),Color(0,0,0),0.9,0,100)));
	/*scene.AddLight(new GlobalLight(Color(1,1,1),Vector3(1,-0.7,1)));
	scene.AddLight(new GlobalLight(Color(1,1,1),Vector3(-1,-0.7,1)));*/
	//scene.AddLight(new PointLight(Color(1,1,1),Vector3(0,0,5)));
	//scene.AddObject(new PointLight(Color(80,80,80),Vector3(4,7,10),new Sphere(Vector3(4,7,10),0.1)));
	//scene.AddObject(new PointLight(Color(100,100,100),Vector3(-4,-8,10),new Sphere(Vector3(-4,-8,10),0.1)));
	scene.AddObject(new AreaLight(Color(100,100,100),new Sphere(Vector3(0,-7,10),0.5),32));
	//scene.AddObject(new AreaLight(Color(100,100,100),new Sphere(Vector3(4,7,10),0.5),64));
	//scene.AddObject(new AreaLight(Color(100,100,100),new Sphere(Vector3(0,7,10),0.5),64));
	//scene.AddObject(new AreaLight(Color(100,100,100),new Sphere(Vector3(-4,7,10),0.5),64));
	Engine engine(&scene);
	engine.Render(cam,image);
#ifdef ANTIALIAS
	PixelBuff screen(width,height);
	image.Downsample(screen);
	ToBMP(screen,"reddevillo");
#else
	PostProcess::DepthAntialiasing(image,image);
	ToBMP(image,"reddevillo");
#endif
	return 0;
}