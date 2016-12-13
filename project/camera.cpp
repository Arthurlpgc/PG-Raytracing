#ifndef __CAMERA_CPP_INCLUDED
#define __CAMERA_CPP_INCLUDED
#include <bits/stdc++.h>
#include "geom.h"
#include "lights.h"
#include "camera.h"
using namespace std;
const double EPS=0.0001;
double intersectQuad(Quadric *q, Point p, Point dir){
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
	double acoef, bcoef, ccoef;
	
	double vx = dir.x;
	double vy = dir.y;
	double vz = dir.z;
	acoef = 2 * f * vy * vz + 2 * e * vx * vz + c * vz * vz + b * vy * vy + a * vx * vx + 2 * d * vx * vy;
	bcoef = 2 * b * p.y * vy + 2 * a * p.x * vx + 2 * c * p.z * vz + 2 * h * vy + 2 * g * vx + 2 * j * vz + 2 * d * p.x * vy +
        2 * e * p.y * vz + 2 * e * p.z * vy + 2 * d * p.y * vx + 2 * f * p.x * vz + 2 * f * p.z * vx;
	ccoef = a * p.x * p.x + 2 * g * p.x + 2 * f * p.x * p.z + b * p.y * p.y + 2 * e * p.y * p.z + 2 * d * p.x * p.y + c * p.z * p.z + 2 * h * p.y +
        2 * j * p.z + k;

	if(1.0 + acoef == 1.0){
		if(1.0 + bcoef == 1.0){
			return -1.0;
		}		
		
		t = (-ccoef)/(bcoef);
	} else {
		disc = bcoef*bcoef - 4*acoef*ccoef;
		if(1.0 + disc < 1.0){
			return -1.0;
		}

		root = sqrt(disc);
		t = (-bcoef -root)/(acoef + acoef);
		if(t < 0.0){
			t = (-bcoef + root)/ (acoef + acoef);		
		}

	}	

	if (t < 0.001) return -1.0;
	
	return t;	
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
double getLightTriColor(Triangle tri,LightDirectional lt){
	Point N=!((tri.c-tri.a)%(tri.b-tri.a));
	double mult=((N*lt.dir)*lt.Il*tri.kd);
	return fabs(mult);
}
double getLightQuadColor(Quadric quad,LightDirectional lt,Point inter){
	Point N(quad.a*inter.x+quad.d*inter.y+quad.e*inter.z+quad.g,quad.b*inter.y+quad.d*inter.x+quad.f*inter.z+quad.h,quad.c*inter.z+quad.e*inter.x+quad.f*inter.y+quad.j);
	double mult=((N*lt.dir)*lt.Il*quad.kd);
	return fabs(mult);
}
Point ray(const Camera cam,int sx,int sy,int mx,int my){
	Point X=cam.vecN*cam.d;
	double y=((sy-my/2.0)/double(my/2.0))*cam.hy;
	double x=((sx-mx/2.0)/double(mx/2.0))*cam.hx;
	return X+(cam.vecV*y)+(cam.vecU*x);
}
#endif
