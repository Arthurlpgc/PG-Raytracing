#include <bits/stdc++.h>
#include "geom.h"
#include "lights.h"
#include "BSurfaces.h"
#include "camera.h"
using namespace std;
const double EPS=0.0001;
Camera cam;
double screenY=540,screenX=720;double bgR,bgG,bgB;
int dim1,dim2,npts1,npts2,VTsize,VQsize,supersample=0,depth=0;double IlumAmb=0.2;
double *buffer;
double *Dists;
vector<vector<Point> > ControlPoints;
vector<Triangle> vecTri,aux;
vector<Quadric> vecQuad;
vector<LightDirectional> vecLgt;
struct color3f{
	double R,G,B;
	color3f operator*(double x){color3f ret;
	ret.R=max(0.0,min(R*x,255.0));
	ret.G=max(0.0,min(G*x,255.0));
	ret.B=max(0.0,min(B*x,255.0));
	return ret;
	}
	color3f operator+(color3f c){
	color3f ret;
	ret.R=max(0.0,min(R+c.R,255.0));
	ret.G=max(0.0,min(G+c.G,255.0));
	ret.B=max(0.0,min(B+c.B,255.0));
	return ret;
	}
};

color3f getDifusaTri(int i,int ltt){
	color3f ret;
	ret.R= getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrR;
	ret.G= getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrG;
	ret.B= getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrB;
	return ret;
}
color3f getEspecularTri(int i,int ltt,Point N,Point raydir){
	color3f ret;
	ret.R=ret.G=ret.B=fabs(vecLgt[ltt].Il*vecTri[i].ks*pow(fabs((!(vecLgt[ltt].dir*(-1)^N))*(!raydir*(-1))),vecTri[i].pot));
	return ret;
}	
color3f getDifusaQuad(int i,int ltt,Point qxr){
	color3f ret;
	ret.R= getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrR;
	ret.G= getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrG;
	ret.B= getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrB;
	return ret;
}
color3f getEspecularQuad(int i,int ltt,Point N,Point raydir){
	color3f ret;
	double csn=(!(vecLgt[ltt].dir*(-1)^N))*(!raydir*(-1));
	if(csn<0)csn=0;
	ret.R=ret.G=ret.B=fabs(vecLgt[ltt].Il*vecQuad[i].ks*pow(csn,vecQuad[i].pot));
	return ret;
}
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
				if(N*raydir>0)N=N*(-1.0);
				refRay=!(raydir*(-1)^!N);KS=vecTri[i].KS;
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
						cor=cor+getDifusaTri(i,ltt)+getEspecularTri(i,ltt,N,raydir);
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
			Point N(quad.a*inter.x+quad.d*inter.y+quad.e*inter.z+quad.g,quad.b*inter.y+quad.d*inter.x+quad.f*inter.z+quad.h,quad.c*inter.z+quad.e*inter.x+quad.f*inter.y+quad.j);
			if(N*raydir>0)N=N*(-1.0);
			refRay=!(raydir*(-1)^!N);
			for(int ltt=0;ltt<vecLgt.size();ltt++){
				bool notBlk=true;
				for(int j=0;j<VTsize;j++){
					if(intersect(vecTri[j],qxr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0)))notBlk=false;
				}
				for(int j=0;j<VQsize;j++){
					if(intersectQuad(&vecQuad[j],qxr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0))>0)notBlk=false;	
				}
				if(notBlk){
					cor=cor+getDifusaQuad(i,ltt,qxr)+getEspecularQuad(i,ltt,N,raydir);
				}	
			}
		}
	}
	cor.R=min(cor.R,255.0);
	cor.G=min(cor.G,255.0);
	cor.B=min(cor.B,255.0);
	if(depth&&Dists<10000000000000000000.0&&KS){
		color3f ref=getRayColor(intersec+refRay*EPS,!refRay,depth-1);
		//cerr<<intersec.x<<" "<<intersec.y<<" "<<intersec.z<<" *-* "<<refRay.x<<" "<<refRay.y<<" "<<refRay.z<<" = "<<ref.R<<" "<<ref.G<<" "<<ref.B<<" "<<endl;
		//swap(ref.B,ref.R);
		return cor*(1.0-KS)+ref*KS;
	}else return cor;
}

void ReadCP(){
	bool rcf=0;
	double x,y,z;
	cout<<"P3\n#Reading..\n";
	string input;
	while(cin>>input){
		if(input[0]=='#')getline(cin,input);
		else if(input=="raycife")rcf=1;
		else if(input=="ambient")cin>>IlumAmb;
		else if(input=="eye"){cin>>cam.position.x>>cam.position.y>>cam.position.z;}
		else if(input=="camera"){cin>>cam.vecV.x>>cam.vecV.y>>cam.vecV.z>>cam.vecN.x>>cam.vecN.y>>cam.vecN.z;
					cin>>cam.d>>cam.hx>>cam.hy;
		}else if(input=="ortho"){
			
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
			double R,G,B;double ka,kd,ks,pot,KS,KT,ir;
			cin>>R>>G>>B>>ka>>kd>>ks>>pot>>KS>>KT>>ir;
			aux=GenerateBezierTriangles(ControlPoints,npts1,npts2,R,G,B,ka,kd,ks,pot,KS,KT,ir);
			vecTri.insert(vecTri.end(),make_move_iterator(aux.begin()),make_move_iterator(aux.end()));
		}else if(input=="size")cin>>screenX>>screenY;
		else if(input=="light"){
			double x,y,z,i;
			cin>>x>>y>>z>>i;
			if(rcf){x=-x;y=-y;z=-z;}
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
	cam.resx=screenX;cam.resy=screenY;	
	buffer=(double*)malloc(sizeof(double)*screenX*screenY*3);
	//Dists=(double*)malloc(sizeof(double)*screenX*screenY);
	cout<<"#DONE!\n255\n";
}

int main(void){
	ReadCP();
	cam.normalize();
	VTsize=vecTri.size(),VQsize=vecQuad.size();
/*	Point raydir1=!ray(cam,470,270,960,540);
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
			double r=0,g=0,b=0;
			for(int scxp=0;scxp<(1+supersample);scxp++){
				for(int scyp=0;scyp<(1+supersample);scyp++){
					r+=buffer[int(scx*(supersample+1)+scxp+(scy*(supersample+1)+scyp)*screenX)*3+0];
					g+=buffer[int(scx*(supersample+1)+scxp+(scy*(supersample+1)+scyp)*screenX)*3+1];
					b+=buffer[int(scx*(supersample+1)+scxp+(scy*(supersample+1)+scyp)*screenX)*3+2];
				}
			}
			cout<<int(255.0*min(1.0,r/(supersample+1)/(supersample+1)))<<endl;
			cout<<int(255.0*min(1.0,g/(supersample+1)/(supersample+1)))<<endl;
			cout<<int(255.0*min(1.0,b/(supersample+1)/(supersample+1)))<<endl;
		}
	}
	cout<<"#Rendered\n"<<flush;
	return 0;
}
