#ifndef __CAMERA_CPP_INCLUDED
#define __CAMERA_CPP_INCLUDED
#include <bits/stdc++.h>
#include "geom.h"
#include "lights.h"
#include "camera.h"
using namespace std;
const double EPS=0.0001;
double intersect(Quadric *q, Point p, Point d){
	double a, b, c, d, e;	
	double f, g, h, j, k;
	double disc;
	double root;
	double t;	
	a = q->a;
	b = q->b;
	c = q->c;
	d = q->d;
	e = q->e;
	f = q->f;
	g = q->g;
	h = q->h;
	j = q->j;
	k = q->k;
	double acoef, bcoef, coef;
	
	double vx = d.x - p.x;
	double vy = d.y - p.y;
	double vz = d.z - p.z;
	acoef = 2 * f * vx * vz + 2 * e * vy * vz + c * vz * vz + b * vy * vy + a * vx * vx + 2 * d * vx * vy;
	bcoef = 2 * b * y0 * vy + 2 * a * x0 * vx + 2 * c * z0 * vz + 2 * h * vy + 2 * g * vx + 2 * j * vz + 2 * d * x0 * vy +
        2 * e * y0 * vz + 2 * e * z0 * vy + 2 * d * y0 * vx + 2 * f * x0 * vz + 2 * f * z0 * vx;
	ccoef = a * x0 * x0 + 2 * g * x0 + 2 * f * x0 * z0 + b * y0 * y0 + 2 * e * y0 * z0 + 2 * d * x0 * y0 + c * z0 * z0 + 2 * h * y0 +
        2 * j * z0 + k;

	if(fabs(acoef) < EPS){
		if(fabs(ccoef) < EPS){
			return -1.0
		}		
		
		t = (-ccoef)/(bcoef);
	} else {
		disc = bcoef*bcoef - 4*acoef*ccoef;
		if(disc < -EPS){
			return -1.0
		}

		root = sqrt(disc);
		t = (-bcoef -root)/(acoef + acoef);
		if(t < -EPS){
			t = (-bcoef + root)/ (acoef + acoef);		
		}

		if (t < EPS) return -1.0;
		
		return t;	
	}	
}
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
