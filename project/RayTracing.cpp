#include <bits/stdc++.h>
#include "geom.h"
#include "lights.h"
#include "BSurfaces.h"
#include "camera.h"
using namespace std;
const double EPS=0.001;
Camera cam;
double screenY=540,screenX=720;
int dim1,dim2,npts1,npts2,bgR=0,bgG=0,bgB=0;double IlumAmb=0.2;
int *buffer;
double *Dists;
vector<vector<Point> > ControlPoints;
vector<Triangle> vecTri,aux;
vector<Quadric> vecQuad;
vector<LightDirectional> vecLgt;

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
	}	
	buffer=(int*)malloc(sizeof(int)*screenX*screenY*3);
	Dists=(double*)malloc(sizeof(double)*screenX*screenY);
	cout<<screenX<<" "<<screenY<<endl;
	cout<<"#DONE!\n255\n";
}

int main(void){
	Quadric quad(1,1,1,0,0,0,0,0,0,-100,0.1,0.2,0,0,159,23,0);//vecQuad.push_back(quad);
	ReadCP();
	cam.normalize();
	memset(buffer,0,sizeof buffer);
	int VTsize=vecTri.size(),VQsize=vecQuad.size(); 		
	for(int scx=0;scx<screenX;scx++){
		cerr<<scx<<"-"<<VQsize<<endl;
		for(int scy=0;scy<screenY;scy++){
			Dists[int(scx+scy*screenX)]=100000000000000000000.0;
			buffer[int(scx+scy*screenX)*3+0]=bgR;
			buffer[int(scx+scy*screenX)*3+1]=bgG;
			buffer[int(scx+scy*screenX)*3+2]=bgB;
			for(int i=0;i<VTsize;i++){
				if(intersect(vecTri[i],cam.position,ray(cam,scx,scy,screenX,screenY))){
					Point txr=triXray(vecTri[i],cam.position,!ray(cam,scx,scy,screenX,screenY));
					if((txr-cam.position).mag()<Dists[int(scx+scy*screenX)]){
						buffer[int(scx+scy*screenX)*3+0]=IlumAmb*vecTri[i].ka*vecTri[i].clrR;
						buffer[int(scx+scy*screenX)*3+1]=IlumAmb*vecTri[i].ka*vecTri[i].clrG;
						buffer[int(scx+scy*screenX)*3+2]=IlumAmb*vecTri[i].ka*vecTri[i].clrB;
						for(int ltt=0;ltt<vecLgt.size();ltt++){						
							if(((!vecLgt[ltt].dir)*(!ray(cam,scx,scy,screenX,screenY)))<=0)continue;
							bool notBlk=true;
							for(int j=0;j<VTsize;j++){
								if(intersect(vecTri[j],txr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0)))notBlk=false;
							}
							for(int j=0;j<VQsize;j++){
								if(intersectQuad(&vecQuad[j],txr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0))>EPS)notBlk=false;	
							}
							if(notBlk){
								buffer[int(scx+scy*screenX)*3+0]=min(
max(int(getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrR+vecTri[i].ka*IlumAmb*vecTri[i].clrR),buffer[int(scx+scy*screenX)*3+0]),255);
								buffer[int(scx+scy*screenX)*3+1]=min(
max(int(getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrG+vecTri[i].ka*IlumAmb*vecTri[i].clrG),buffer[int(scx+scy*screenX)*3+1]),255);
								buffer[int(scx+scy*screenX)*3+2]=min(
max(int(getLightTriColor(vecTri[i],vecLgt[ltt])*vecTri[i].clrB+vecTri[i].ka*IlumAmb*vecTri[i].clrB),buffer[int(scx+scy*screenX)*3+2]),255);
							}	
						}
						Dists[int(scx+scy*screenX)]=(txr-cam.position).mag();
					}
				}
			}
			for(int i=0;i<VQsize;i++){
				double dstqd=intersectQuad(&vecQuad[i],cam.position,!ray(cam,scx,scy,screenX,screenY));
				if(dstqd>0&&dstqd<Dists[int(scx+scy*screenX)]){
					Dists[int(scx+scy*screenX)]=dstqd;
					Point qxr=((!ray(cam,scx,scy,screenX,screenY))*dstqd)+cam.position;
					buffer[int(scx+scy*screenX)*3+0]=IlumAmb*vecQuad[i].ka*vecQuad[i].clrR;
					buffer[int(scx+scy*screenX)*3+1]=IlumAmb*vecQuad[i].ka*vecQuad[i].clrG;
					buffer[int(scx+scy*screenX)*3+2]=IlumAmb*vecQuad[i].ka*vecQuad[i].clrB;
					for(int ltt=0;ltt<vecLgt.size();ltt++){						
						if(((!vecLgt[ltt].dir)*(!ray(cam,scx,scy,screenX,screenY)))<=0)continue;
						bool notBlk=true;
						for(int j=0;j<VTsize;j++){
							if(intersect(vecTri[j],qxr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0)))notBlk=false;
						}
						for(int j=0;j<VQsize;j++){
							if(intersectQuad(&vecQuad[j],qxr-(vecLgt[ltt].dir*EPS),vecLgt[ltt].dir*(-1.0))>EPS)notBlk=false;	
						}
						if(notBlk){
							buffer[int(scx+scy*screenX)*3+0]=min(255,max(buffer[int(scx+scy*screenX)*3+0],
int(IlumAmb*vecQuad[i].ka*vecQuad[i].clrR+getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrR)));
							buffer[int(scx+scy*screenX)*3+1]=min(255,max(buffer[int(scx+scy*screenX)*3+1],
int(IlumAmb*vecQuad[i].ka*vecQuad[i].clrG+getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrG)));
							buffer[int(scx+scy*screenX)*3+2]=min(255,max(buffer[int(scx+scy*screenX)*3+2],
int(IlumAmb*vecQuad[i].ka*vecQuad[i].clrB+getLightQuadColor(vecQuad[i],vecLgt[ltt],qxr)*vecQuad[i].clrB)));
						}	
					}
				}
			}
//glVertex2f((scx-screenX/2.0)/double(screenX/2.0),(scy-screenY/2.0)/double(screenY/2.0) );
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
