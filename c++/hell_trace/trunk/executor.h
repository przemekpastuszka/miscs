/*
HellTrace by Przemysław Pastuszka
University of Wrocław, Poland

All you need to execute tasks is provided here.
*/

#ifndef HELLEXECUTOR
#define HELLEXECUTOR

#include "task.h"
#include "resourceManager.h"
#include <map>
#include <string>
#include "vector3.h"

typedef void (*executive)( ResourceManager&, Task*);

class Executor
{
  std::map<std::string,executive> functions;
  
  public:
    void AddFunction(std::string name,executive function);
    void ExecuteFile(std::string name);
};

std::string GetValue(Task* t,std::string name);
Vector3 ToVector3(std::string value);
Resource* GetResource(Task* t,std::string name,std::string entity,ResourceManager& res,Resource::ResourceType type);

namespace Executive
{
  void ImageLoader( ResourceManager&,Task*);
  void ImageSaver( ResourceManager&,Task*);
  void ImageCreator( ResourceManager&,Task*);
  void Lerp( ResourceManager&,Task*);
  void GaussianBlur( ResourceManager&,Task*);
  void AdvToneMap( ResourceManager&,Task*);
  void SimpleToneMap( ResourceManager&,Task*);
  void DepthAntialiasing( ResourceManager&,Task*);
  void BrightPass( ResourceManager&,Task*);
  void SphereCreator( ResourceManager&,Task*);
  void PlaneCreator( ResourceManager&,Task*);
  void PointLightCreator( ResourceManager&,Task*);
  void AreaLightCreator( ResourceManager&,Task*);
  void PhongCreator( ResourceManager&,Task*);
  void SimpleCreator( ResourceManager&,Task*);
  void SceneCreator( ResourceManager&,Task*);
  void OrthoCamCreator( ResourceManager&,Task*);
  void ConicCamCreator( ResourceManager&,Task*);
  void RayTracing( ResourceManager&,Task*);
}

#endif
