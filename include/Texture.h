#ifndef TEXTURE_H
#define TEXTURE_H
#include <GL/glew.h>
#include <string>
#include <iostream>
#include "stb_image.h"

class Texture {
	unsigned int texture_ID;
	unsigned int textureBehavior;
	unsigned int textureFiltering;
public:
	Texture(const std::string &,unsigned int,unsigned int,unsigned int);
	void bind(GLuint);
	void unBind();
	~Texture();



};


#endif
