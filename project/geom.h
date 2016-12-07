#ifndef __Geom_H_INCLUDED__
#define __Geom_H_INCLUDED__
#include <math.h>
struct Point{	
	double x,y,z;
	Point(){}
	Point(double x,double y,double z):x(x),y(y),z(z){};
	double mag()const{
		return sqrt(x*x+y*y+z*z);
	}
	double operator*(const Point& p)const{
		return x*p.x+y*p.y+z*p.z;
	}
	Point operator*(double d)const{
		return Point(d*x,d*y,d*z);
	}
	Point operator+(const Point& p)const{//just use when the "Point" really means a vector
		return Point(x+p.x,y+p.y,z+p.z);
	}
	Point operator-(const Point& p)const{
		return Point(x-p.x,y-p.y,z-p.z);
	}
	Point operator%(const Point& p)const{
		return Point(y*p.z-z*p.y,z*p.x-x*p.z,x*p.y-y*p.x);
	}
	Point operator!()const{
		return Point(x/mag(),y/mag(),z/mag());
	}
};

struct Triangle{
	Point a,b,c;
	double kd=0.8;int clrR,clrG,clrB;
	Triangle(Point a,Point b,Point c):a(a),b(b),c(c){};
	Triangle(Point a,Point b,Point c,int R,int G,int B,double kd):a(a),b(b),c(c),kd(kd),clrR(R),clrG(G),clrB(B){};
};

struct Quadric{
	double a,b,c,d,e,f,g,h,j,k; //coefficients of quadric surface
	double kd, ka, ks;
	int clrR, clrG, clrB;
	int pot;
	Quadric(double a, double b, double c, double d, double e, double f, double g, double h, double j, double k, double kd, double ka, double ks, int R, int G, int B, int pot):a(a),b(b),c(c),d(d),e(e),f(f),g(g),h(h),j(j),k(k),kd(kd),ka(ka),ks(ks),clrR(R),clrG(G),clrB(B),pot(pot){};
};
#endif 
