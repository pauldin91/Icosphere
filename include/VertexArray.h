#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H
#include "de.h"
#include <iostream>
#include <vector>

class VertexArray {
	unsigned int m_vbo_ID;
	unsigned int m_vao_ID;
	unsigned int m_ibo_ID;
	unsigned int m_stride;
	unsigned int m_offset;
	std::vector<unsigned int> m_layout;

public :
	VertexArray(float * , unsigned int);
	~VertexArray();
	void addIndexBuffer(unsigned int *,unsigned int);
	void bind();
	void unBind();
	void addAttribute(unsigned int);
	void pushLayout();
};


#endif
