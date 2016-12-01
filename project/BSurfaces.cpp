#ifndef __BSFRCS_CPP_INCLUDED
#define __BSFRCS_CPP_INCLUDED
//REAAAALLY SLOW, but processes once.
#include <bits/stdc++.h>
#include "geom.h"
using namespace std;
Point FindPointInBcurve(vector<Point> v,double t){
	if(v.empty())return Point(0,0,0);
	while(v.size()>1){
		for(int i=0;i<v.size()-1;i++){
			v[i]=(Point(v[i].x*t+v[i+1].x*(1-t),v[i].y*t+v[i+1].y*(1-t),v[i].z*t+v[i+1].z*(1-t)));
		}
		v.pop_back();
	}
	return v[0];
}
vector<Point> Casteljau(vector<Point> v,int numPoints){
	vector<Point> BezierCurve;
	for(int i=0;i<numPoints;i++){
		BezierCurve.push_back(FindPointInBcurve(v,double(i)/double(numPoints-1)));
	}
	return BezierCurve;
}
vector<Triangle> GenerateBezierTriangles(const vector<vector<Point> > v,int npts1,int npts2){
	vector<Triangle> ret;
	vector<vector<Point> > processedCP;
	vector<Point> aux,prev,actual;
	for(int i=0;i<v.size();i++){
		processedCP.push_back(Casteljau(v[i],npts2));
		aux.push_back(v[i][0]);
	}
	prev=Casteljau(aux,npts1);
	for(int i=1;i<npts2;i++){
		aux.clear();
		for(int j=0;j<processedCP.size();j++){
			aux.push_back(processedCP[j][i]);
		}
		actual=Casteljau(aux,npts1);
		for(int j=1;j<npts1;j++){
			ret.push_back(Triangle(actual[j-1],actual[j],prev[j]));
			ret.push_back(Triangle(actual[j-1],prev[j-1],prev[j]));
		}
	}
	return ret;
}
vector<vector<Point> > dbg1(const vector<vector<Point> > v,int npts1,int npts2){
	vector<vector<Point> > processedCP,ret;
	vector<Point> aux,prev,actual;
	for(int i=0;i<v.size();i++){
		processedCP.push_back(Casteljau(v[i],npts2));
		aux.push_back(v[i][0]);
	}
	ret.push_back(Casteljau(aux,npts1));
	for(int i=1;i<npts2;i++){
		aux.clear();
		for(int j=0;j<processedCP.size();j++){
			aux.push_back(processedCP[j][i]);
		}
		ret.push_back(Casteljau(aux,npts1));
	}
	return ret;
}
#endif
