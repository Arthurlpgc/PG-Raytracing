#ifndef __CAMERA_CPP_INCLUDED
#define __CAMERA_CPP_INCLUDED
#include <bits/stdc++.h>
#include "geom.h"
#include "lights.h"
#include "camera.h"
using namespace std;
const double EPS=0.0001;
bool intersect(Triangle tri,Point p,Point d){
	Point d1=tri.b-tri.a,d2=tri.c-tri.a;
	Point h=d%d2;
	double a=d1*h;
	if(a > -EPS && a < EPS){return 0;}
	double f=1/a;
	Point s=p-tri.a;
	double u=f*(s*h);
	if(u<0.0||u>1.0){return 0;}
	Point q=s%d1;
	double v=f*(d*q);
	if(v<0.0||u+v>1.0){return 0;}
	double t = f*(d2*q);
	if(t>EPS)return 1;
	else return 0;
}
Point triXray(Triangle tri,Point p,Point d){
	Point d1=tri.b-tri.a,d2=tri.c-tri.a;
	Point h=d%d2;
	double a=d1*h;
	double f=1/a;
	Point s=p-tri.a;
	double u=f*(s*h);
	Point q=s%d1;
	double v=f*(d*q);
	double t = f*(d2*q);
	return (tri.c*v)+(tri.b*u)+(tri.a*(1.0-u-v));
}
int getLightTriColor(Triangle tri,LightDirectional lt){
	Point N=!((tri.c-tri.a)%(tri.b-tri.a));
	double mult=((N*lt.dir)*lt.Il*tri.kd);
	return abs(mult*255);
}
Point ray(const Camera cam,int sx,int sy,int mx,int my){
	Point X=cam.vecN*cam.d;
	double x=((sx-mx/2.0)/double(mx/2.0))*cam.hx;
	double y=((sy-my/2.0)/double(my/2.0))*cam.hy;
	return X+(cam.vecV*y)+(cam.vecU*x);
}
#endif
