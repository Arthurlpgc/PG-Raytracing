#ifndef __LIGHT_H_INCLUDED_
#define __LIGHT_H_INCLUDED_
#include "geom.h"
struct LightDirectional{
	Point dir;
	double Il;
	LightDirectional():dir(),Il(){};
	LightDirectional(Point p,double c):dir(p),Il(c){};
};
#endif
