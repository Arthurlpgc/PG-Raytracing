#include <bits/stdc++.h>
#include "geom.h"
#include "lights.h"
#include "BSurfaces.h"
#include "camera.h"
using namespace std;
const double EPS=0.000001;
Camera cam;
double screenY=540,screenX=720;
int dim1,dim2,npts1,npts2,bgR=0,bgG=0,bgB=0,VTsize,VQsize,supersample=0,depth=0;double IlumAmb=0.2;
int *buffer;
double *Dists;
vector<vector<Point> > ControlPoints;
vector<Triangle> vecTri,aux;
vector<Quadric> vecQuad;
vector<LightDirectional> vecLgt;
struct color3f{
	int R,G,B;
	color3f operator*(double x){color3f ret;
	ret.R=max(0,min(int(R*x),255));
	ret.G=max(0,min(int(G*x),255));
	ret.B=max(0,min(int(B*x),255));
	return ret;
	}
	color3f operator+(color3f c){
	color3f ret;
	ret.R=max(0,min(int(R+c.R),255));
	ret.G=max(0,min(int(G+c.G),255));
	ret.B=max(0,min(int(B+c.B),255));
	return ret;
	}
};
color3f getRayColor(Point initpos,Point raydir,int depth=0){
	color3f cor;
	Point intersec,refRay;
	double Dists=100000000000000000000.0,KS=0;
	cor.R=bgR;
	cor.G=bgG;
	cor.B=bgB;
	for(int i=0;i<VTsize;i++){
		if(intersect(vecTri[i],initpos,raydir)){
			Point txr=intersec=triXray(vecTri[i],initpos,!raydir);
			if((txr-initpos).mag()<Dists){
				cor.R=IlumAmb*vecTri[i].ka*vecTri[i].clrR;
				cor.G=IlumAmb*vecTri[i].ka*vecTri[i].clrG;
				cor.B=IlumAmb*vecTri[i].ka*vecTri[i].clrB;
				Point N=!((vecTri[i].a-vecTri[i].b)%(vecTri[i].a-vecTri[i].c));
				refRay=!(N^raydir);KS=vecTri[i].KS;
				for(int ltt=0;ltt<vecLgt.size();ltt++){						
					if(((!vecLgt[ltt].dir)*(!raydir))<=EPS)continue;
					bool notBlk=true;
					for(int j=0;j<VTsize;j++){
						if(intersect(vecTri[j],txr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0)))notBlk=false;
					}
					for(int j=0;j<VQsize;j++){
						if(intersectQuad(&vecQuad[j],txr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0))>EPS)notBlk=false;	
					}
					if(notBlk){
						cor.R=min(
	max(int(getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrR+vecTri[i].ka*IlumAmb*vecTri[i].clrR+
	fabs(vecLgt[ltt].Il*vecTri[i].ks*pow(fabs((!(N^vecLgt[ltt].dir))*(!raydir)),vecTri[i].pot))*255)
	,cor.R),255);
						cor.G=min(
	max(int(getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrG+vecTri[i].ka*IlumAmb*vecTri[i].clrG+
	fabs(vecLgt[ltt].Il*vecTri[i].ks*pow(fabs((!(N^vecLgt[ltt].dir))*(!raydir)),vecTri[i].pot))*255)
	,cor.G),255);
						cor.B=min(
	max(int(getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrB+vecTri[i].ka*IlumAmb*vecTri[i].clrB+
	fabs(vecLgt[ltt].Il*vecTri[i].ks*pow(fabs((!(N^vecLgt[ltt].dir))*(!raydir)),vecTri[i].pot))*255)
	,cor.B),255);
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
			Point qxr=intersec=((!raydir)*dstqd)+initpos;
			cor.R=IlumAmb*vecQuad[i].ka*vecQuad[i].clrR;
			cor.G=IlumAmb*vecQuad[i].ka*vecQuad[i].clrG;
			cor.B=IlumAmb*vecQuad[i].ka*vecQuad[i].clrB;
			Quadric quad=vecQuad[i];Point inter=qxr;KS=vecQuad[i].KS;
			Point N(2*quad.a*inter.x+quad.d*inter.y+quad.e*inter.z+quad.g,2*quad.b*inter.y+quad.d*inter.x+quad.f*inter.z+quad.h,2*quad.c*inter.z+quad.e*inter.x+quad.e*inter.y+quad.j);
			refRay=!(N^raydir);
			for(int ltt=0;ltt<vecLgt.size();ltt++){
				if((!vecLgt[ltt].dir)*(!N)>EPS)continue;						
				if(((!vecLgt[ltt].dir)*(!raydir))<=0)continue;
				bool notBlk=true;
				for(int j=0;j<VTsize;j++){
					if(intersect(vecTri[j],qxr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0)))notBlk=false;
				}
				for(int j=0;j<VQsize;j++){
					if(intersectQuad(&vecQuad[j],qxr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0))>EPS)notBlk=false;	
				}
				if(notBlk){
					cor.R=min(255,max(cor.R,
	int(IlumAmb*vecQuad[i].ka*vecQuad[i].clrR+getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrR+
	fabs(vecLgt[ltt].Il*vecQuad[i].ks*pow(fabs((!(N^vecLgt[ltt].dir))*(!raydir)),vecQuad[i].pot))*255
	)));
					cor.G=min(255,max(cor.G,
	int(IlumAmb*vecQuad[i].ka*vecQuad[i].clrG+getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrG+
	fabs(vecLgt[ltt].Il*vecQuad[i].ks*pow(fabs((!(N^vecLgt[ltt].dir))*(!raydir)),vecQuad[i].pot))*255
	)));
					cor.B=min(255,max(cor.B,
	int(IlumAmb*vecQuad[i].ka*vecQuad[i].clrB+getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrB+
	fabs(vecLgt[ltt].Il*vecQuad[i].ks*pow(fabs((!(N^vecLgt[ltt].dir))*(!raydir)),vecQuad[i].pot))*255
	)));
				}	
			}
		}
	}
	if(depth&&Dists<10000000000000000000.0){
		color3f ref=getRayColor(intersec+refRay*EPS,refRay,depth-1);
		
		return cor*(1.0-KS)+ref*KS;
	}else return cor;
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
			int R,G,B;double ka,kd,ks,pot,KS,KT,ir;
			cin>>R>>G>>B>>ka>>kd>>ks>>pot>>KS>>KT>>ir;
			aux=GenerateBezierTriangles(ControlPoints,npts1,npts2,R,G,B,ka,kd,ks,pot,KS,KT,ir);
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
			vecQuad.push_back(quad);
		}else if(input=="supersampling")cin>>supersample;
		else if(input=="depth")cin>>depth;
	}
	cout<<screenX<<" "<<screenY<<endl;
	screenX*=(1+supersample);
	screenY*=(1+supersample);	
	buffer=(int*)malloc(sizeof(int)*screenX*screenY*3);
	//Dists=(double*)malloc(sizeof(double)*screenX*screenY);
	cout<<"#DONE!\n255\n";
}

int main(void){
	ReadCP();
	cam.normalize();
	VTsize=vecTri.size(),VQsize=vecQuad.size();
	/*Point raydir1=!ray(cam,490,270,960,540);
	cerr<<raydir1.x<<" "<<raydir1.y<<" "<<raydir1.z<<endl;
	getRayColor(cam.position,!raydir1,1);
	return 0;*/
	memset(buffer,0,sizeof buffer); 		
	for(int scx=0;scx<screenX;scx++){
		cerr<<scx<<"-"<<endl;
		for(int scy=0;scy<screenY;scy++){
			Point raydir=ray(cam,scx,scy,screenX,screenY);
			color3f cor=getRayColor(cam.position,raydir,depth);
			buffer[int(scx+scy*screenX)*3+0]=cor.R;
			buffer[int(scx+scy*screenX)*3+1]=cor.G;
			buffer[int(scx+scy*screenX)*3+2]=cor.B;
		}
	}	
	for(int scy=0;scy<screenY/(1+supersample);scy++){
		for(int scx=0;scx<screenX/(1+supersample);scx++){
			int r=0,g=0,b=0;
			for(int scxp=0;scxp<(1+supersample);scxp++){
				for(int scyp=0;scyp<(1+supersample);scyp++){
					r+=buffer[int(scx*(supersample+1)+scxp+(scy*(supersample+1)+scyp)*screenX)*3+0];
					g+=buffer[int(scx*(supersample+1)+scxp+(scy*(supersample+1)+scyp)*screenX)*3+1];
					b+=buffer[int(scx*(supersample+1)+scxp+(scy*(supersample+1)+scyp)*screenX)*3+2];
				}
			}
			cout<<r/(supersample+1)/(supersample+1)<<endl;
			cout<<g/(supersample+1)/(supersample+1)<<endl;
			cout<<b/(supersample+1)/(supersample+1)<<endl;
		}
	}
	cout<<"#Rendered\n"<<flush;
	return 0;
}
