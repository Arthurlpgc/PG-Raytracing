#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
using namespace std;
struct Point{
	double x,y,z;
	Point(double x,double y,double z):x(x),y(y),z(z){};
	
};

int main(void){	
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
