/*
HellTrace by Przemysław Pastuszka
University of Wrocław, Poland

Heart of tasks execution.
*/

#include "executor.h"
#include "utilities.h"
#include "pixelBuff.h"
#include "log.h"
#include "postProcess.h"
#include "sphere.h"
#include "plane.h"
#include "pointLight.h"
#include "areaLight.h"
#include "phong.h"
#include "simple.h"
#include "orthoCam.h"
#include "conicCam.h"
#include "scene.h"
#include "engine.h"

void Executor::AddFunction(std::string name,executive function)
{
  functions[name]=function;
}

//do all the tasks from a file
void Executor::ExecuteFile(std::string name)
{
   ResourceManager resources; //init resource manager
   TaskManager tasks; //init task manager
   tasks.FromFile(name); //parsem file
   tasks.SortTasks(); //do topological sort
   Task* temp;
   while((temp=tasks.GetTask())!=NULL)
   {
   	//find and execute appropriate action
   	std::map<std::string,executive>::iterator found=functions.find(temp->type);
   	if(found==functions.end())
   		Log::AddMessage("Unsupported task: "+temp->type,Log::ERR);
   	Log::AddMessage("Task "+temp->name+" started...",Log::LOW); 
   	(*found).second(resources,temp);
   	Log::AddMessage("Task "+temp->name+" finished",Log::LOW); 
   }
}

//task consists of diverse elements. This function gets one of them by the name
std::string GetValue(Task* t,std::string name)
{
	std::map<std::string,Task::TaskElement>::iterator found = t->elements.find(name);
	if(found==t->elements.end())
		return "";
	return (*found).second.value;
}

//converts string like 'a,b,c' into Vector3(a,b,c)
Vector3 ToVector3(std::string value)
{
	std::vector<std::string> el=Split(value,",");
	if(el.size()!=3)
		Log::AddMessage("Point in 3D space should consist of 3 coordinates",Log::ERR);
	return Vector3(ToFloat(el[0]),ToFloat(el[1]),ToFloat(el[2]));
}

//Gets resource by the name described in task
//you also should provide 'entity' to have ability to report errors properly
//and ResourceType, so function can check does it match with found resource
Resource* GetResource(Task* t,std::string name,std::string entity,ResourceManager& res,Resource::ResourceType type)
{
	std::string source=GetValue(t,name);
  	if(source=="")
  		Log::AddMessage(entity+": Source needs to be specified",Log::ERR);
  	Resource* one=res.Get(source);
  	if(one==NULL)
  		Log::AddMessage(entity+": "+source+ " cannot be found",Log::ERR);
  	if(one->GetResourceType()!=type)
		Log::AddMessage(entity+": wrong type of resource",Log::ERR);
  	return one;
}

//gets element. In case it cannot be found, you can specify default value
std::string GetElement(Task* t,std::string name,std::string entity,ResourceManager& res,
	bool canBeDefault,std::string defVal)
{
	std::string source=GetValue(t,name);
  	if(source=="")
  	{
  		if(canBeDefault)
  		{
  			Log::AddMessage(entity+": "+name+" not specified. '"+defVal+"' assumed",Log::NORMAL);
  			source=defVal;
		}
		else
  			Log::AddMessage(entity+": "+name+" needs to be specified",Log::ERR);
  	}
	return source;
}

//functions responsible for tasks execution. I'm not going to comment them.
namespace Executive
{
  void ImageLoader(ResourceManager& res,Task* t)
  {    
  	//load input file name
	std::string fileName=GetElement(t,"filename","ImageLoader",res,false,"");
	//load input file format
	std::string format=GetElement(t,"format","ImageLoader",res,true,"hlt");
	
	//load file to the buffer
	if(format=="hlt")
		res.Add(t->name,FromHLT(fileName));
	if(format=="bmp")
		res.Add(t->name,FromBMP(fileName));
	if(format=="raw")
	{
		int width=(int)ToFloat(GetElement(t,"width","ImageLoader",res,false,""));
  		int height=(int)ToFloat(GetElement(t,"height","ImageLoader",res,false,""));
  		res.Add(t->name,FromRAW(fileName,width,height));
	}
	if(format!="hlt"&&format!="raw"&&format!="bmp")
		Log::AddMessage("ImageLoader: Unsupported format "+format,Log::ERR);
  }
  
  void ImageSaver( ResourceManager& res,Task* t)
  {
	//get the name of the output file
	std::string fileName=GetValue(t,"filename");
	if(fileName=="")
	{
		Log::AddMessage("ImageSaver: Name of the input file not specified. Skipping.",Log::HIGH);
		return;
	}
	//get the desired format
	std::string format=GetElement(t,"format","ImageSaver",res,true,"hlt");
	
	//find the buffer in the environment
	PixelBuff* buff=(PixelBuff*)GetResource(t,"buffer","ImageSaver",res,Resource::BUFFER);
	
	//save
	if(format=="hlt")
		ToHLT(buff,fileName);
	if(format=="bmp")
		ToBMP(buff,fileName);
	if(format=="raw")
		ToRAW(buff,fileName);
	if(format!="hlt"&&format!="bmp"&&format!="raw")
		Log::AddMessage("ImageSaver: Unsupported format "+format+". Skipping.",Log::HIGH);
  }	
  void ImageCreator( ResourceManager& res,Task* t)
  {
  	int width,height;
  	width=(int)ToFloat(GetValue(t,"width"));
  	height=(int)ToFloat(GetValue(t,"height"));
  	
  	if(width<=0||height<=0)
  		Log::AddMessage("ImageCreator: Wrong width or size attribute",Log::ERR);
  	
  	res.Add(t->name,new PixelBuff(width,height));
  }	
  void Lerp( ResourceManager& res,Task* t)
  {
  	PixelBuff* one=(PixelBuff*)GetResource(t,"buffer1","Lerp",res,Resource::BUFFER);
  	PixelBuff* two=(PixelBuff*)GetResource(t,"buffer2","Lerp",res,Resource::BUFFER);
  		
  	float value=ToFloat(GetElement(t,"value","Lerp",res,false,"0"));
  	
  	res.Add(t->name,PostProcess::Lerp(one,two,value));	
  }
  void GaussianBlur( ResourceManager& res,Task* t)
  {
  	PixelBuff* one=(PixelBuff*)GetResource(t,"buffer","GaussianBlur",res,Resource::BUFFER);
  	
  	float value=ToFloat(GetElement(t,"sigma","GaussianBlur",res,false,"0"));;
  	
  	res.Add(t->name,PostProcess::GaussianBlur(one,value));
  }
  void AdvToneMap( ResourceManager& res,Task* t)
  {
  	PixelBuff* one=(PixelBuff*)GetResource(t,"buffer","AdvToneMap",res,Resource::BUFFER);
  	
  	float grey=ToFloat(GetElement(t,"middleGrey","AdvToneMap",res,true,"0.6"));
  	float white=ToFloat(GetElement(t,"whitePoint","AdvToneMap",res,true,"16"));
  	
  	res.Add(t->name,PostProcess::AdvToneMapping(one,grey,white));
  }
  void SimpleToneMap( ResourceManager& res,Task* t)
  {
  	PixelBuff* one=(PixelBuff*)GetResource(t,"buffer","SimpleToneMap",res,Resource::BUFFER);
  	float exposure=ToFloat(GetElement(t,"exposure","SimpleToneMap",res,true,"1"));
  	
  	res.Add(t->name,PostProcess::SimpleToneMapping(one,exposure));
  }
  void DepthAntialiasing( ResourceManager& res,Task* t)
  {
  	PixelBuff* one=(PixelBuff*)GetResource(t,"buffer","DepthAntialiasing",res,Resource::BUFFER);
  	float treshold=ToFloat(GetElement(t,"treshold","DepthAntialiasing",res,true,"1.2"));
  	
  	res.Add(t->name,PostProcess::DepthAntialiasing(one,one,treshold));
  }
  void BrightPass( ResourceManager& res,Task* t)
  {
  	PixelBuff* one=(PixelBuff*)GetResource(t,"buffer","BrightPass",res,Resource::BUFFER);
  	
  	float treshold=ToFloat(GetElement(t,"treshold","BrightPass",res,true,"0.5"));
  	float offset=ToFloat(GetElement(t,"offset","BrightPass",res,true,"1"));
  	float grey=ToFloat(GetElement(t,"middleGrey","BrightPass",res,true,"0.6"));
  	float white=ToFloat(GetElement(t,"whitePoint","BrightPass",res,true,"16"));
  	
  	res.Add(t->name,PostProcess::BrightPass(one,grey,white,treshold,offset));
  }
  void SphereCreator( ResourceManager& res,Task* t)
  {
  	Vector3 pos=ToVector3(GetElement(t,"origin","Sphere",res,false,""));
  	float r=ToFloat(GetElement(t,"radius","Sphere",res,false,""));
  	
  	res.Add(t->name,new Sphere(pos,r));
  }
  void PlaneCreator( ResourceManager& res,Task* t)
  {
  	Vector3 n=ToVector3(GetElement(t,"normal","Plane",res,false,""));
  	float d=ToFloat(GetElement(t,"distance","Plane",res,false,""));
  	
  	res.Add(t->name,new Plane(n,d));
  }
  void PointLightCreator( ResourceManager& res,Task* t)
  {
  	Primitive* prim=NULL;
  	Vector3 color=ToVector3(GetElement(t,"color","PointLight",res,false,""));
  	Vector3 pos=ToVector3(GetElement(t,"position","PointLight",res,false,""));
  	
  	std::string v=GetValue(t,"primitive");
  	if(v=="")
  		Log::AddMessage("PointLight: No primitve specified for point light",Log::NORMAL);
  	else
  	{
  		Resource* resource=res.Get(v);
  		prim=dynamic_cast<Primitive*>(resource);
  		if(prim==NULL)
  			Log::AddMessage("PointLight: Object "+v+" cannot be found. Skipping...",Log::HIGH);
  	}
  	
 	res.Add(t->name,new PointLight(Color(color),pos,prim));
  }
  void AreaLightCreator( ResourceManager& res,Task* t)
  {
  	Vector3 color=ToVector3(GetElement(t,"color","AreaLight",res,false,""));
  	Primitive* prim=(Primitive*)GetResource(t,"primitive","AreaLight",res,Resource::PRIMITIVE);
  	float p=ToFloat(GetElement(t,"points","AreaLight",res,false,""));
  	
  	res.Add(t->name,new AreaLight(Color(color),prim,(int)p));
  }	
  void PhongCreator( ResourceManager& res,Task* t)
  {
  	Vector3 color=ToVector3(GetElement(t,"color","Phong",res,false,""));
  	Vector3 spec=ToVector3(GetElement(t,"specColor","Phong",res,true,"1,1,1"));
  	float diff=ToFloat(GetElement(t,"diffuse","Phong",res,false,""));
  	float ref=ToFloat(GetElement(t,"reflectance","Phong",res,false,""));
  	float specPower=ToFloat(GetElement(t,"specPower","Phong",res,true,"100"));
  	
  	res.Add(t->name,new Phong(Color(color),Color(spec),diff,ref,(int)specPower));
  }
  void SimpleCreator( ResourceManager& res,Task* t)
  {
  	Primitive* prim=(Primitive*)GetResource(t,"primitive","Simple",res,Resource::PRIMITIVE);
  	Material* m=(Material*)GetResource(t,"material","Simple",res,Resource::MATERIAL);
  	
  	res.Add(t->name,new Simple(prim,m));
  }
  void SceneCreator( ResourceManager& res,Task* t)
  {
  	Vector3 size=ToVector3(GetElement(t,"size","Scene",res,false,""));
  	
  	Scene* scene=new Scene(size);
  	std::string v;
  	int i=1;
  	while((v=GetValue(t,"object"+ToString(i++)))!="")
  		scene->AddObject((Object*)GetResource(t,"object"+ToString(i-1),"Scene",res,Resource::OBJECT));
  	res.Add(t->name,scene);
  }
  /*void OrthoCamCreator( ResourceManager& res,Task* t)
  {
  	int width=(int)ToFloat(GetElement(t,"width","OrthoCam",res,false,""));
  	int height=(int)ToFloat(GetElement(t,"height","OrthoCam",res,false,""));
  	
  	res.Add(t->name,new OrthographicCam(width,height));
  }*/
  void ConicCamCreator( ResourceManager& res,Task* t)
  {
  	int width=(int)ToFloat(GetElement(t,"width","ConicCam",res,false,""));
  	int height=(int)ToFloat(GetElement(t,"height","ConicCam",res,false,""));
  	
  	res.Add(t->name,new ConicCam(width,height));
  }
  void RayTracing( ResourceManager& res,Task* t)
  {
  	Camera* cam=(Camera*)GetResource(t,"camera","RayTracing",res,Resource::CAMERA);
  	Scene* scene=(Scene*)GetResource(t,"scene","RayTracing",res,Resource::SCENE);
  	int trDpt=(int)ToFloat(GetElement(t,"tracingDepth","RayTracing",res,true,"5"));
  	int width=(int)ToFloat(GetElement(t,"width","RayTracing",res,false,""));
  	int height=(int)ToFloat(GetElement(t,"height","RayTracing",res,false,""));
  	
  	Engine engine(scene);
  	res.Add(t->name,engine.Render(cam,width,height,trDpt));
  }
}
