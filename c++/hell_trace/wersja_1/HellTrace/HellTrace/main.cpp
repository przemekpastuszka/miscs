#include <stdio.h>
#include "utilities.h"
#include "simple.h"
#include "sphere.h"
#include "plane.h"
#include "solid.h"
#include "scene.h"
#include "globalLight.h"
#include "pointLight.h"
#include "orthoCam.h"
#include "conicCam.h"
#include "engine.h"

int main()
{
	int width=640,height=480;
	//int width=1024,height=768;
	PixelBuff image(width,height);
	Camera* cam=new ConicCam(width,height);
	/*PixelBuff image(2*width,2*height);
	Camera* cam=new ConicCam(2*width,2*height);*/

	Scene scene(Vector3(10,10,10),0.1);
	scene.AddObject(new Simple(new Sphere(Vector3(0,0,16),3),new Solid(Color(0,0,1),0.9,0.8,100),false));
	scene.AddObject(new Simple(new Sphere(Vector3(8,0,12),3),new Solid(Color(0,1,0),0.8,0.2,100),false));
	scene.AddObject(new Simple(new Sphere(Vector3(-8,0,14),3),new Solid(Color(1,0,0),0.7,0.4,100),false));
	scene.AddObject(new Simple(new Sphere(Vector3(0,4,23),5),new Solid(Color(0.75,0.75,0.75),1,0,0),false));
	scene.AddObject(new Simple(new Plane(Vector3(0,0,-1),28),new Solid(Color(0,0,1),1,0,0),false));
	scene.AddObject(new Simple(new Plane(Vector3(-1,0,0),15),new Solid(Color(1,0,0),1,0,0),false));
	scene.AddObject(new Simple(new Plane(Vector3(1,0,0),15),new Solid(Color(1,0,0),1,0,0),false));
	scene.AddObject(new Simple(new Plane(Vector3(0,1,0),15),new Solid(Color(0,1,0),1,0,0),false));
	scene.AddObject(new Simple(new Plane(Vector3(0,-1,0),15),new Solid(Color(1,1,1),1,0,0),false));
	/*scene.AddLight(new GlobalLight(Color(1,1,1),Vector3(1,-0.7,1)));
	scene.AddLight(new GlobalLight(Color(1,1,1),Vector3(-1,-0.7,1)));*/
	//scene.AddLight(new PointLight(Color(1,1,1),Vector3(0,0,5)));
	scene.AddLight(new PointLight(Color(80,80,80),Vector3(4,7,10)));
	scene.AddLight(new PointLight(Color(100,100,100),Vector3(-4,-8,10)));

	Engine engine(&scene);
	engine.Render(cam,image);

	/*PixelBuff screen(width,height);
	image.Downsample(screen);
	ToBMP(screen,"reddevillo");*/
	ToBMP(image,"reddevillo");
	return 0;
}