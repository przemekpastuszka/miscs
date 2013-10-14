/*
HellTrace by Przemysław Pastuszka
University of Wrocław, Poland

I have decided to implement programmable pipeline, so now user can simply create primitives,
scenes, cameras, materials and so on. To simplify dealing with all this stuff, I made them derivated
from Resource class.
*/

#ifndef HELLRESOURCE
#define HELLRESOURCE

class Resource
{
public:
	enum ResourceType {MATERIAL,OBJECT,PRIMITIVE,CAMERA,BUFFER,SCENE};
	virtual ResourceType GetResourceType() const=0;
};
#endif
