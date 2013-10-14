/*
HellTrace by Przemysław Pastuszka
University of Wrocław, Poland

Nothing to comment, really.
*/

#include <stdio.h>
#include "log.h"
#include "executor.h"


int main(int argc, const char* argv[])
{
	try
	{
		//set up the executor
		Executor ex;
		ex.AddFunction("ImageLoader",Executive::ImageLoader);
		ex.AddFunction("ImageSaver",Executive::ImageSaver);
		ex.AddFunction("ImageCreator",Executive::ImageCreator);
		ex.AddFunction("Lerp",Executive::Lerp);
		ex.AddFunction("GaussianBlur",Executive::GaussianBlur);
		ex.AddFunction("AdvToneMap",Executive::AdvToneMap);
		ex.AddFunction("SimpleToneMap",Executive::SimpleToneMap);
		ex.AddFunction("DepthAntialiasing",Executive::DepthAntialiasing);
		ex.AddFunction("BrightPass",Executive::BrightPass);
		ex.AddFunction("Sphere",Executive::SphereCreator);
		ex.AddFunction("Plane",Executive::PlaneCreator);
		ex.AddFunction("PointLight",Executive::PointLightCreator);
		ex.AddFunction("AreaLight",Executive::AreaLightCreator);
		ex.AddFunction("Phong",Executive::PhongCreator);
		ex.AddFunction("Simple",Executive::SimpleCreator);
		//ex.AddFunction("OrthoCam",Executive::OrthoCamCreator);
		ex.AddFunction("ConicCam",Executive::ConicCamCreator);
		ex.AddFunction("RayTracing",Executive::RayTracing);
		ex.AddFunction("Scene",Executive::SceneCreator);
		
		//execute!
		if(argc>1) //from cmd line
			ex.ExecuteFile(argv[1]);
		else
			Log::AddMessage("Please specify the input file",Log::HIGH);
	}
	catch(...)
	{
		Log::ToFile(Log::LOW,"log.html");
	}
	Log::ToFile(Log::LOW,"log.html");

	return 0;
}
