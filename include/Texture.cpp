#include "Texture.h"

Texture::Texture(const std::string & filePath,unsigned int tB,unsigned int tF,unsigned int alphachannel):
	textureBehavior(tB),textureFiltering(tF){
	glGenTextures(1, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureBehavior);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureBehavior);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFiltering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFiltering);

	int m_width, m_height, m_channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channels, 0);
	

	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, alphachannel, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	
	}else{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Texture::bind(GLuint slot){
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D,texture_ID);
}

void Texture::unBind(){
	glBindTexture(GL_TEXTURE_2D,0);
}

Texture::~Texture(){
	glDeleteTextures(1,&texture_ID);
}
