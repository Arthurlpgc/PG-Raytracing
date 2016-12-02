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
		cout<<vecN.mag()<<" "<<vecV.mag()<<" "<<vecU.mag()<<endl;
	}
};
bool intersect(Triangle tri,Point p,Point d);
Point ray(Camera cam,int sx,int sy,int mx,int my);
#endif