#include <bits/stdc++.h>
#include "geom.h"
using namespace std;
Point FindPointInBcurve(vector<Point> v,double t);
vector<Point> Casteljau(vector<Point> v,int numPoints);
vector<Triangle> GenerateBezierTriangles(const vector<vector<Point> > v,int npts1,int npts2);
vector<vector<Point> > dbg1(const vector<vector<Point> > v,int npts1,int npts2);
