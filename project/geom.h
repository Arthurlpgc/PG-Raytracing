#ifndef __Geom_H_INCLUDED__
#define __Geom_H_INCLUDED__

struct Point{	
	double x,y,z;
	Point(double x,double y,double z):x(x),y(y),z(z){};
};

struct Triangle{
	Point a,b,c;
	Triangle(Point a,Point b,Point c):a(a),b(b),c(c){};
};
#endif 
