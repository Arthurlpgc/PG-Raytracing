#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include "geom.h"
#include "BSurfaces.h"
#include "camera.h"
using namespace std;

Camera cam;
double screenY=540,screenX=720;
int dim1,dim2,npts1,npts2;
vector<vector<Point> > ControlPoints;
void ReadCP(){
	double x,y,z;
	cout<<"Reading..\n";
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
	cin>>npts1>>npts2;
	cout<<"DONE!\n";
}

int main(void){
	ReadCP();
    vector<Triangle> vecTri=GenerateBezierTriangles(ControlPoints,npts1,npts2);
    cam.normalize();
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
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
  		
  		glColor3f(1, 1, 1);
		for(int scx=0;scx<screenX;scx++){
			for(int scy=0;scy<screenY;scy++){
				bool paint=0;
				for(int i=0;i<vecTri.size();i++){
					if(intersect(vecTri[i],cam.position,ray(cam,scx,scy,screenX,screenY))){
					 	paint=1;
					}
				}
				if(paint){
					glPointSize(1);
					glBegin(GL_POINTS);
						glVertex2f((scx-screenX/2.0)/double(screenX/2.0),(scy-screenY/2.0)/double(screenY/2.0) );
					glEnd();
				}
			}
		}	
		cout<<"Rendered\n"<<flush;
	    cam.position.z+=1;
	    
	    glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
