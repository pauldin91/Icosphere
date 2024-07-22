// #include "include/VertexArray.h"
#include "include/Shader.h"
#include "include/Texture.h"
// #include "include/IcoSphere.h"
#include "include/Scene.h"
#include <iostream>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "include/de.h"

#define pie    3.14159265
#define width  		 1080
#define height  	  960
#define from   	  		1
#define to     	  		8


using namespace glm;

mat4 qmodel(1.0f);
double transform(double,double,double,double,double);
static void movement(GLFWwindow * win,double, double);
bool is_number(const std::string& s);

int main(int argc, char ** argv)
{
    unsigned int param=3;
	float radius = 1.0f;
    if(argc>=2 && is_number(argv[1]) && atoi(argv[1])>0){
	  param=atoi(argv[1]);
	  if(param>to){
		  param=to;
		  std::cerr<<"Max Subdivision Order = "<<to<<'\n';
	  }
    }
	GLFWwindow* window;
	if (!glfwInit()){	
		return -1;
	}
	window = glfwCreateWindow(width, height, "Geodesic Grid", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	GLCall(Shader geometrySubdivision("resources/geometrySubdivision.glsl",true));
	GLCall(Shader simpleSubdivision("resources/simpleSubdivision.glsl",true));
	
	glfwSetCursorPosCallback(window,movement);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	std::vector<unsigned int> layout{3};
	Scene icoscene(radius,param,layout);

	Texture  texture("textures/bricks-mce.png",  GL_MIRRORED_REPEAT,GL_NEAREST,GL_RGBA);
	texture.bind(0);

	mat4 proj (1.0f);
	mat4 model(1.0f);
	mat4 view (1.0f);
	glClearColor(0.0f,0.0f,0.0f,1.0f);	

/*________ImGuiBegin________*/
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    const char* glsl_version = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
	ImVec4 clearColor(0.1f, 0.1f, 0.1f, 1.0f);
	bool triangles=false;
	bool textureOn=0; 
	bool animate=0;
	bool geomSub=0;
	bool setPerspectiveInGeometryShader=false;
	ImVec4 internal(0.0f,1.0f,0.0f,1.0f);
	ImVec4 grid(1.0f,1.0f,0.0f,1.0f);
/*________ImGuiEnd________*/
	Shader * toUse;
	unsigned int current=param;
	
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		
		glfwPollEvents();
		glClearColor(clearColor.x,clearColor.y,clearColor.z,clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
		if(geomSub==0){
			toUse=&simpleSubdivision;
		}else{
			toUse=&geometrySubdivision;
		}
		icoscene.renderScene(radius,param,current,layout);
		
		//________________Scene________________
		texture.bind(0);
		proj = perspective(radians(60.0f),1.0f,0.8f,10.0f);//frustum(-3.0f,3.0f,-3.0f,3.0f,-3.0f,3.0f);//
	    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		toUse->bind();
		toUse->setUniform1f("time", glfwGetTime());
		toUse->setUniform1i("text1", 0);
		toUse->setUniform1i("textureOn",(int)textureOn);
		toUse->setUniform1i("animate",  (int)animate);
		toUse->setUniform1i("geomSub",  (int)geomSub);
		toUse->setUniformVec4f("grid",vec4(grid.x,grid.y,grid.z,grid.w));
		toUse->setUniformVec4f("internal",vec4(internal.x,grid.y,internal.z,internal.w));
		model = qmodel;

		mat4 modelview = proj * view * model;

		if(setPerspectiveInGeometryShader){
			toUse->setUniformMat4f("modelviewV",mat4(1.0));
			toUse->setUniformMat4f("modelviewG",modelview);
		}else{
			toUse->setUniformMat4f("modelviewG",mat4(1.0));
			toUse->setUniformMat4f("modelviewV",modelview);	
		}
		//________________Scene________________

		if(triangles){
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		}else{
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);   
		}
		glDrawElements(GL_TRIANGLES,icoscene.getTriangles(),GL_UNSIGNED_INT,nullptr);
		
		/*_________ImGuiBegin_________*/
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		ImGui::Begin("Drawing Mode");
		ImGui::ColorEdit3("Clear",(float *)&clearColor);
		ImGui::Checkbox("Filled ",&triangles);
		ImGui::Checkbox("Texture ",&textureOn);
		ImGui::Checkbox("Animation ",&animate);
		ImGui::Checkbox("Geom Sub ",&geomSub);
		ImGui::Checkbox("Change Perspective ",&setPerspectiveInGeometryShader);
		
		ImGui::SliderInt("Subdivision Order",(int*)&current,from,to,"%d");
		ImGui::SliderFloat("Radius",&radius,0.5f,2.0f,"%.2f");
		if(!textureOn){
			ImGui::ColorEdit3("Internal",(float *)&internal);
			ImGui::ColorEdit3("Main",(float *)&grid);
		}
		ImGui::End();
		ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /*__________ImGuiEnd__________*/
		glfwSwapBuffers(window); 
	}
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

double transform(double t,double a, double b, double c, double d) {
	return c + (d - c) * (t - a )  / (b - a) ;
}
 
static void movement(GLFWwindow *win ,double x,double y) {
	if(glfwGetMouseButton(win,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS){
		double xpos = transform(x  - width/2.0 ,0,width,  -pie,   pie);
		double ypos = transform(height/2.0 - y ,0,height, -pie/2, pie/2);
		vec3 dir= vec3(cos(xpos)*sin(ypos), cos(ypos), sin(xpos)*sin(ypos));
		qmodel = mat4_cast(quat(0.0, dir));
	}

}
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

