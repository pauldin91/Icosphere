#ifndef DE_H
#define DE_H
#include <GL/glew.h>
#include <iostream>
static void GLClearError(){
	while(glGetError()!=GL_NO_ERROR);
}
static bool GLLogCall(const char * function,const char * file,int line){
	while(GLenum error=glGetError()){
		std::cout<<"[OpenGL error] ("<<error<<") : "<<function<<" "<<file<<" at line :"<<line<<"\n";
		return false;
	}return true;
}
#define ASSERT(x) if(!(x)) __builtin_trap();
#define GLCall(x) GLClearError();x;ASSERT(GLLogCall(#x,__FILE__,__LINE__));



#endif