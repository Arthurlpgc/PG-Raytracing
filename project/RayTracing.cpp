#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include "geom.h"
#include "BSurfaces.h"
using namespace std;


int dim1,dim2,npts1,npts2;
vector<vector<Point> > ControlPoints;
void ReadCP(){
	double x,y,z;
	cout<<"Reading..\n";
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
    vector<Triangle> vec=GenerateBezierTriangles(ControlPoints,npts1,npts2);
    GLFWwindow* window;
    if (!glfwInit())return -1;
    window = glfwCreateWindow(720, 480, "Raytracing", NULL, NULL);
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



        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
