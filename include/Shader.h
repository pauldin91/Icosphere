#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
class Shader{
	unsigned int shader_ID;
	bool hasGeometryShader;
	unsigned int  compile(unsigned int , const std::string &);
	void create(const std::string &, const std::string &, const std::string &);
    
public:
	Shader(const std::string & programPath,bool geomShader);
	~Shader();
	void bind();
	void unBind();

	unsigned int getUniformLocation(const std::string &);
	void setUniform1i  (const std::string & ,int);
	void setUniform1f  (const std::string & ,float);
	void setUniformVec2f(const std::string & ,const glm::vec2 &);
	void setUniformVec3f(const std::string & ,const glm::vec3 &);
	void setUniformVec4f(const std::string & ,const glm::vec4 &);
	void setUniformMat2f(const std::string &, const glm::mat2 &);
	void setUniformMat3f(const std::string & ,const glm::mat3 &);
	void setUniformMat4f(const std::string & ,const glm::mat4 &);

};
#endif



