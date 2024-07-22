#include "VertexArray.h"

VertexArray::VertexArray(float * verts,unsigned int size):
	m_stride(0),m_vbo_ID(0),m_ibo_ID(0),m_vao_ID(0){
	glGenVertexArrays(1, &m_vao_ID);
	glGenBuffers(1, &m_vbo_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_ID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), verts, GL_STATIC_DRAW);
	
}

VertexArray::~VertexArray(){

	glDeleteVertexArrays(1, &m_vao_ID);
	glDeleteBuffers(1, &m_vbo_ID);
	glDeleteBuffers(1, &m_ibo_ID);
}

void VertexArray::addIndexBuffer(unsigned int * indices, unsigned int size){
	glGenBuffers(1, &m_ibo_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

void VertexArray::bind(){
	glBindVertexArray(m_vao_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo_ID);
}

void VertexArray::unBind(){
	glBindVertexArray(0);
}

void VertexArray::addAttribute(unsigned int attSize){
	m_layout.push_back(attSize);
	m_stride += attSize;	
}

void VertexArray::pushLayout(){
	GLCall(glBindVertexArray(m_vao_ID);)
	unsigned int offset = 0 ;
	for (unsigned int i = 0; i < m_layout.size(); ++i ) {
		glVertexAttribPointer(i,m_layout[i], GL_FLOAT, GL_FALSE ,m_stride * sizeof(float),(const void *)( offset * sizeof(float)));
		glEnableVertexAttribArray(i);
		offset += m_layout[i];
	}
}