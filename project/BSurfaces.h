#ifndef __BSFRCS_H_INCLUDED
#define __BSFRCS_H_INCLUDED
#include <bits/stdc++.h>
#include "geom.h"
using namespace std;
Point FindPointInBcurve(vector<Point> v,double t);
vector<Point> Casteljau(vector<Point> v,int numPoints);
vector<Triangle> GenerateBezierTriangles(const vector<vector<Point> > v,int npts1,int npts2,int R,int G,int B,double ka,double kd,double ks,double pot,double KS,double KT,double ir);
#endif
