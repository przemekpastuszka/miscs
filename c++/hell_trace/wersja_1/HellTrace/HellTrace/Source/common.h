/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Defines some macros
*/

#ifndef COMMONHELL
#define COMMONHELL

/*
this should be explained a bit more
imagine: you cast a ray A and it hits some surface B in point C which has non-zero value of
reflectivity. So you should produce new ray A' which has an origin at point C and appropriate direction.
Now you're tracing A' and... oops! It hits surface B in point C! What happened?
Computations are wrong because of limited precision of float.
How to fix it? I introduce the EPSILON as the "safety buffer" - the intersection occurs only if the distance between
origin of the ray and computed point is higher than its value
*/
#define EPSILON 0.1

//to avoid infite recursion no more than 5 rays (reflected,refracted) can be traced
#define TRACINGDPT 5

#define ALIASTRESHOLD 1.2

#endif