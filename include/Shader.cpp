#include "Shader.h"
Shader::Shader(const std::string & programPath,bool geomShader):shader_ID(0),hasGeometryShader(geomShader){
	std::ifstream programFile(programPath);
	if (!programFile.is_open()) {
		std::cout << "Error opening the file";
	}
	std::stringstream vertex;
	std::stringstream geom;
	std::stringstream fragment;
	std::string line;
	int vert = 0;
	while (getline(programFile,line)) {
		if(line.find("#Vertex")!= std::string::npos) {
			vert = 0;
			getline(programFile,line);
		}
		else if (hasGeometryShader && line.find("#Geometry") != std::string::npos) {
			
			vert = 1;
			getline(programFile, line);
		}
		else if (line.find("#Fragment") != std::string::npos) {
			vert = 2;
			getline(programFile, line);
		}
		
		if (vert==0) {
			vertex << line<<'\n';
		}
		else if (vert == 1) {
			geom << line << '\n';
		}
		else if(vert==2){
			fragment << line << '\n';
		}
		
	}
	programFile.close();
	create(vertex.str(),geom.str(),fragment.str());
	
	
}
Shader::~Shader() {
	glDeleteProgram(shader_ID);

}

unsigned int Shader::compile(unsigned int type,const std::string & program) {
	unsigned int id = glCreateShader(type);
	const char *src = program.c_str();
	glShaderSource(id,1,&src,nullptr);
	glCompileShader(id);
	int res;
	glGetShaderiv(id,GL_COMPILE_STATUS,&res);
	if (res == GL_FALSE) {
		int len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
	    char* message = (char*)alloca(len*sizeof(char));
		glGetShaderInfoLog(id,len,&len,message);
		bool veert = (type == GL_VERTEX_SHADER);
		std::cout << "Failed to Compile : " << (veert ? "Vertex " :" Fragment") << "Shader \n"<<message<<"\n";
		glDeleteShader(id);
	}return id;

}
void Shader::create(const std::string & vert, const std::string & geom, const std::string & frag) {
	
	shader_ID = glCreateProgram();
	
	
	unsigned int vertexShader =   compile(GL_VERTEX_SHADER , vert);
	glAttachShader(shader_ID,vertexShader);
	unsigned int geometryShader;
	if(hasGeometryShader){
		geometryShader = compile(GL_GEOMETRY_SHADER, geom);
		glAttachShader(shader_ID, geometryShader);
	
	}
	unsigned int fragmentShader = compile(GL_FRAGMENT_SHADER,frag);
	glAttachShader(shader_ID,fragmentShader);
	glLinkProgram(shader_ID);
	glValidateProgram(shader_ID);
	glDeleteShader(vertexShader);
	if(hasGeometryShader){
		glDeleteShader(geometryShader);
	}
	glDeleteShader(fragmentShader);

}
void Shader::bind() {
	glUseProgram(shader_ID);
}
void Shader::unBind() {
	glUseProgram(0);
}
unsigned int Shader::getUniformLocation(const std::string & name)
{
	return glGetUniformLocation(shader_ID,name.c_str());
}
void Shader::setUniform1i(const std::string & name, int val){
	glUniform1i(getUniformLocation(name), val);
}
void Shader::setUniform1f(const std::string & name, float val){
	glUniform1f(getUniformLocation(name),val);
}
void Shader::setUniformVec2f(const std::string & name, const glm::vec2 & val){
	glUniform2fv(getUniformLocation(name),1,&val[0]);
}
void Shader::setUniformVec3f(const std::string & name, const glm::vec3 & val){
	glUniform3fv(getUniformLocation(name), 1, &val[0]);
}
void Shader::setUniformVec4f(const std::string & name, const glm::vec4 & val){
	glUniform4fv(getUniformLocation(name), 1, &val[0]);
}
void Shader::setUniformMat2f(const std::string & name, const glm::mat2 & val) {
	glUniformMatrix2fv(getUniformLocation(name), 1,GL_FALSE, &val[0][0]);
}
void Shader::setUniformMat3f(const std::string & name, const glm::mat3 & val){
	glUniformMatrix3fv(getUniformLocation(name),1, GL_FALSE,&val[0][0]);
}

void Shader::setUniformMat4f(const std::string & name, const glm::mat4 & val){
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &val[0][0]);
}

