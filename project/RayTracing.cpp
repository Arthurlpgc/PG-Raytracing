#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include "geom.h"
#include "BSurfaces.h"
using namespace std;

//delete
double screenY=520,screenX=720;
double clcy(double y){
	return (screenY/2-y)/double(screenY/2);
}
double clcx(double x){
	return (x-screenX/2)/double(screenX/2);
}
void makeLines(vector<Point> v){
	for(int i=1;i<v.size();i++){
    	glBegin(GL_LINES);
			glVertex2f(clcx(v[i].x),clcy(v[i].y));
			glVertex2f(clcx(v[i-1].x),clcy(v[i-1].y));
		glEnd();
    }
}

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
	//delete    
    vector<vector<Point> > aux=dbg1(ControlPoints,npts1,npts2),aux2;
    for(int i=0;i<aux[0].size();i++){
    	aux2.push_back(vector<Point>());
    	for(int j=1;j<aux.size();j++){
    		aux2[i].push_back(aux[j][i]);
    		cout<<aux[j][i].x<<","<<aux[j][i].y<<endl;
    	}
    	cout<<"TTT\n"<<flush;
    }
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
 		//delete       
        glLineWidth(0.5);
		glColor3f(1, 1, 1);
		for(auto vvv:aux)makeLines(vvv);
		for(auto vvv:aux2){glColor3f((rand()%101)/101.0,(rand()%101)/101.0,(rand()%101)/101.0);makeLines(vvv);}
    

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
