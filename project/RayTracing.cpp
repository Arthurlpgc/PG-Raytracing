#ifdef FORCE_GL_VIEW
#include <GLFW/glfw3.h>
#endif
#include <bits/stdc++.h>
#include "geom.h"
#include "lights.h"
#include "BSurfaces.h"
#include "camera.h"
using namespace std;

Camera cam;
const double screenY=540,screenX=720;
int dim1,dim2,npts1,npts2,bgR=0,bgG=0,bgB=0;
int *buffer;
double *Dists;
vector<vector<Point> > ControlPoints;
void ReadCP(){
	double x,y,z;
	cout<<"P3\n#Reading..\n";
	cin>>cam.position.x>>cam.position.y>>cam.position.z>>cam.vecV.x>>cam.vecV.y>>cam.vecV.z>>cam.vecN.x>>cam.vecN.y>>cam.vecN.z;
	cin>>cam.d>>cam.hx>>cam.hy;cam.resx=screenX;cam.resy=screenY;
	cin>>dim1>>dim2;
	for(int i=0;i<dim1;i++){
		vector<Point> auxV;auxV.clear();
		for(int j=0;j<dim2;j++){
			cin>>x>>y>>z;
			auxV.push_back(Point(x,y,z));
		}
		ControlPoints.push_back(auxV);
	}
	buffer=(int*)malloc(sizeof(int)*screenX*screenY*3);
	Dists=(double*)malloc(sizeof(double)*screenX*screenY);
	cin>>npts1>>npts2;
	cout<<screenX<<" "<<screenY<<endl;
	cout<<"#DONE!\n255\n";
}

int main(void){
	ReadCP();LightDirectional lt=LightDirectional(Point(0,0.6,-0.8),1);
	vector<Triangle> vecTri=GenerateBezierTriangles(ControlPoints,npts1,npts2);
	cam.normalize();
#ifdef FORCE_GL_VIEW	
	GLFWwindow* window;
	if (!glfwInit())return -1;
	window = glfwCreateWindow(screenX, screenY, "Raytracing", NULL, NULL);
	if (!window){
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
   // glfwSetMouseButtonCallback(window, mbpressed);
   // glfwSetCursorPosCallback(window, mmove);
   	int rendering=0;
	while (!glfwWindowShouldClose(window)){
		if(rendering==0){
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSwapBuffers(window);
		}
		if(rendering==1){
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
#endif
		memset(buffer,0,sizeof buffer);
 		int VTsize=vecTri.size(); 		
		for(int scx=0;scx<screenX;scx++){
			for(int scy=0;scy<screenY;scy++){
				Dists[int(scx+scy*screenX)]=100000000000000000000.0;
				buffer[int(scx+scy*screenX)*3+0]=bgR;
				buffer[int(scx+scy*screenX)*3+1]=bgG;
				buffer[int(scx+scy*screenX)*3+2]=bgB;
				for(int i=0;i<VTsize;i++){
					if(intersect(vecTri[i],cam.position,ray(cam,scx,scy,screenX,screenY))){
						buffer[int(scx+scy*screenX)*3+0]=getLightTriColor(vecTri[i],lt);
						buffer[int(scx+scy*screenX)*3+1]=getLightTriColor(vecTri[i],lt);
						buffer[int(scx+scy*screenX)*3+2]=getLightTriColor(vecTri[i],lt);
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
#ifdef FORCE_GL_VIEW
		glPointSize(1);
		for(int scx=0;scx<screenX;scx++){
			for(int scy=0;scy<screenY;scy++){
			}
		}
		glfwSwapBuffers(window);
		}
		rendering++;
		glfwPollEvents();
	}
	glfwTerminate();
#endif
	return 0;
}
