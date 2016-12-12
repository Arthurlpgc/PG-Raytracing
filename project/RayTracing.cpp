#include <bits/stdc++.h>
#include "geom.h"
#include "lights.h"
#include "BSurfaces.h"
#include "camera.h"
using namespace std;
const double EPS=0.001;
Camera cam;
double screenY=540,screenX=720;
int dim1,dim2,npts1,npts2,bgR=0,bgG=0,bgB=0,VTsize,VQsize;double IlumAmb=0.2;
int *buffer;
double *Dists;
vector<vector<Point> > ControlPoints;
vector<Triangle> vecTri,aux;
vector<Quadric> vecQuad;
vector<LightDirectional> vecLgt;
struct color3f{
int R,G,B;
};
color3f getRayColor(Point initpos,Point raydir){
	color3f cor;
	double Dists=100000000000000000000.0;
	cor.R=bgR;
	cor.G=bgG;
	cor.B=bgB;
	for(int i=0;i<VTsize;i++){
		if(intersect(vecTri[i],initpos,raydir)){
			Point txr=triXray(vecTri[i],initpos,!raydir);
			if((txr-initpos).mag()<Dists){
				cor.R=IlumAmb*vecTri[i].ka*vecTri[i].clrR;
				cor.G=IlumAmb*vecTri[i].ka*vecTri[i].clrG;
				cor.B=IlumAmb*vecTri[i].ka*vecTri[i].clrB;
				for(int ltt=0;ltt<vecLgt.size();ltt++){						
					if(((!vecLgt[ltt].dir)*(!raydir))<=0)continue;
					bool notBlk=true;
					for(int j=0;j<VTsize;j++){
						if(intersect(vecTri[j],txr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0)))notBlk=false;
					}
					for(int j=0;j<VQsize;j++){
						if(intersectQuad(&vecQuad[j],txr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0))>EPS)notBlk=false;	
					}
					if(notBlk){
						cor.R=min(
	max(int(getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrR+vecTri[i].ka*IlumAmb*vecTri[i].clrR),cor.R),255);
						cor.G=min(
	max(int(getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrG+vecTri[i].ka*IlumAmb*vecTri[i].clrG),cor.G),255);
						cor.B=min(
	max(int(getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrB+vecTri[i].ka*IlumAmb*vecTri[i].clrB),cor.B),255);
					}	
				}
				Dists=(txr-initpos).mag();
			}
		}
	}
	for(int i=0;i<VQsize;i++){
		double dstqd=intersectQuad(&vecQuad[i],initpos,!raydir);
		if(dstqd>0&&dstqd<Dists){
			Dists=dstqd;
			Point qxr=((!raydir)*dstqd)+initpos;
			cor.R=IlumAmb*vecQuad[i].ka*vecQuad[i].clrR;
			cor.G=IlumAmb*vecQuad[i].ka*vecQuad[i].clrG;
			cor.B=IlumAmb*vecQuad[i].ka*vecQuad[i].clrB;
			for(int ltt=0;ltt<vecLgt.size();ltt++){						
				if(((!vecLgt[ltt].dir)*(!raydir))<=0)continue;
				bool notBlk=true;
				for(int j=0;j<VTsize;j++){
					if(intersect(vecTri[j],qxr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0)))notBlk=false;
				}
				for(int j=0;j<VQsize;j++){
					if(intersectQuad(&vecQuad[j],qxr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0))>EPS)notBlk=false;	
				}
				if(notBlk){
					Quadric quad=vecQuad[i];Point inter=qxr;
					Point N(2*quad.a*inter.x+quad.d*inter.y+quad.e*inter.z+quad.g,2*quad.b*inter.y+quad.d*inter.x+quad.f*inter.z+quad.h,2*quad.c*inter.z+quad.e*inter.x+quad.e*inter.y+quad.j);
					cor.R=min(255,max(cor.R,
	int(IlumAmb*vecQuad[i].ka*vecQuad[i].clrR+getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrR+
	fabs(vecLgt[ltt].Il*vecQuad[i].ks*pow((vecLgt[ltt].dir^N)*(!raydir),vecQuad[i].pot))
	)));
					cor.G=min(255,max(cor.G,
	int(IlumAmb*vecQuad[i].ka*vecQuad[i].clrG+getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrG+
	fabs(vecLgt[ltt].Il*vecQuad[i].ks*pow((vecLgt[ltt].dir^N)*(!raydir),vecQuad[i].pot))
	)));
					cor.B=min(255,max(cor.B,
	int(IlumAmb*vecQuad[i].ka*vecQuad[i].clrB+getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrB+
	fabs(vecLgt[ltt].Il*vecQuad[i].ks*pow((!(vecLgt[ltt].dir^N))*(!raydir),vecQuad[i].pot))
	)));
				}	
			}
		}
	}
	return cor;
}

void ReadCP(){
	double x,y,z;
	cout<<"P3\n#Reading..\n";
	string input;
	while(cin>>input){
		if(input[0]=='#')getline(cin,input);
		else if(input=="ambient")cin>>IlumAmb;
		else if(input=="eye"){cin>>cam.position.x>>cam.position.y>>cam.position.z;}
		else if(input=="camera"){cin>>cam.vecV.x>>cam.vecV.y>>cam.vecV.z>>cam.vecN.x>>cam.vecN.y>>cam.vecN.z;
					cin>>cam.d>>cam.hx>>cam.hy;cam.resx=screenX;cam.resy=screenY;
		}else if(input=="bezier"){
			cin>>dim1>>dim2;
			ControlPoints.clear();
			for(int i=0;i<dim1;i++){
				vector<Point> auxV;auxV.clear();
				for(int j=0;j<dim2;j++){
					cin>>x>>y>>z;
					auxV.push_back(Point(x,y,z));
				}
				ControlPoints.push_back(auxV);
			}
			cin>>npts1>>npts2;
			int R,G,B;double kd;
			cin>>R>>G>>B>>kd;
			aux=GenerateBezierTriangles(ControlPoints,npts1,npts2,R,G,B,kd);
			vecTri.insert(vecTri.end(),make_move_iterator(aux.begin()),make_move_iterator(aux.end()));
		}else if(input=="size")cin>>screenX>>screenY;
		else if(input=="light"){
			double x,y,z,i;
			cin>>x>>y>>z>>i;
			vecLgt.push_back(LightDirectional(!Point(x,y,z),i));
		}else if(input=="background")cin>>bgR>>bgG>>bgB;
		else if(input=="object"){
			Quadric quad;
			cin>>quad.a>>quad.b>>quad.c>>quad.d>>quad.e>>quad.f>>quad.g>>quad.h>>quad.j>>quad.k>>quad.clrR>>quad.clrG>>quad.clrB;
			cin>>quad.ka>>quad.kd>>quad.ks>>quad.pot>>quad.KS>>quad.KT>>quad.ir;
		}
	}	
	buffer=(int*)malloc(sizeof(int)*screenX*screenY*3);
	//Dists=(double*)malloc(sizeof(double)*screenX*screenY);
	cout<<screenX<<" "<<screenY<<endl;
	cout<<"#DONE!\n255\n";
}

int main(void){
	Quadric quad(1,1,1,0,0,0,0,0,0,-100,0.2,0.1,0.1,0,159,23,2.2);vecQuad.push_back(quad);
	ReadCP();
	cam.normalize();
	memset(buffer,0,sizeof buffer);
	VTsize=vecTri.size(),VQsize=vecQuad.size(); 		
	for(int scx=0;scx<screenX;scx++){
		cerr<<scx<<"-"<<endl;
		for(int scy=0;scy<screenY;scy++){
			Point raydir=ray(cam,scx,scy,screenX,screenY);
			color3f cor=getRayColor(cam.position,raydir);
			buffer[int(scx+scy*screenX)*3+0]=cor.R;
			buffer[int(scx+scy*screenX)*3+1]=cor.G;
			buffer[int(scx+scy*screenX)*3+2]=cor.B;
		}
	}	
	for(int scy=0;scy<screenY;scy++){
		for(int scx=0;scx<screenX;scx++){
			cout<<buffer[int(scx+scy*screenX)*3+0]<<endl;
			cout<<buffer[int(scx+scy*screenX)*3+1]<<endl;
			cout<<buffer[int(scx+scy*screenX)*3+2]<<endl;
		}
	}
	cout<<"#Rendered\n"<<flush;
	return 0;
}
