#ifndef __Geom_H_INCLUDED__
#define __Geom_H_INCLUDED__
#include <math.h>
struct Point{	
	double x,y,z;
	Point(){}
	Point(double x,double y,double z):x(x),y(y),z(z){};
	double mag(){
		return sqrt(x*x+y*y+z*z);
	}
	double operator*(Point p){
		return x*p.x+y*p.y+z*p.z;
	}
	Point operator*(double d){
		return Point(d*x,d*y,d*z);
	}
	Point operator+(Point p){//just use when the "Point" really means a vector
		return Point(x+p.x,y+p.y,z+p.z);
	}
	Point operator-(Point p){
		return Point(x-p.x,y-p.y,z-p.z);
	}
	Point operator%(Point p){
		return Point(y*p.z-z*p.y,z*p.x-x*p.z,x*p.y-y*p.x);
	}
	Point operator!(){
		return Point(x/mag(),y/mag(),z/mag());
	}
};

struct Triangle{
	Point a,b,c;
	Triangle(Point a,Point b,Point c):a(a),b(b),c(c){};
};
#endif 
