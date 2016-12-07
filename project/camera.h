#ifndef __CAMERA_H_INCLUDED
#define __CAMERA_H_INCLUDED
using namespace std;
struct Camera{
	Point position,vecV,vecN,vecU;
	double d,hx,hy;
	int resx,resy;
	Camera(){}
	Camera(Point p,Point v,Point n):position(p),vecV(v),vecN(n){};
	void normalize(){
		vecV=!vecV;
		vecN=!vecN;
		vecU=!(vecV%vecN);
		vecV=!(vecN%vecU);
	}
};
bool intersect(Triangle tri,Point p,Point d);
double intersectQuad(Quadric *q, Point p, Point d);
double getLightTriColor(Triangle tri,LightDirectional lt);
double getLightQuadColor(Quadric quad,LightDirectional lt,Point inter);
Point triXray(Triangle tri,Point p,Point d);
Point ray(Camera cam,int sx,int sy,int mx,int my);
#endif
