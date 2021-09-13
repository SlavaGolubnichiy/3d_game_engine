#pragma once

#ifndef __glew_h__
	#include <GL/glew.h>		// does not contain #pragma once
#endif

class IndexBuffer
{
	private:
	unsigned int id;

	public:
	IndexBuffer(unsigned int* indices, unsigned int indices_size)
	{
		glGenBuffers(1, &(this->id));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
	}

	IndexBuffer(std::vector<unsigned int>& indices)
	{
		glGenBuffers(1, &(this->id));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
	}

	~IndexBuffer()
	{
		glDeleteBuffers(1, &(this->id));
	}

	void Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	}

	void Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};
