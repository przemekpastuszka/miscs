#ifndef WLLCPROJ
#define WLLCPROJ

typedef struct wllc
{
	int x;
	wllc* child;
	wllc* next;
}WLLC;


WLLC merge(WLLC l1,WLLC l2);
WLLC flat(WLLC li);
WLLC copy(WLLC li);

#endif