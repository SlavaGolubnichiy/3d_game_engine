#pragma once

#ifndef __glew_h__
	#include <GL/glew.h>		// does not contain #pragma once
#endif

class VertexArray
{
	private:
	unsigned int id;



	public:
	VertexArray()
	{
		// generate buffer for 1 single object, get buffer's reference into (this->id). 
		// VAO sources pointers to one or more VBOs.
		glGenVertexArrays(1, &(this->id));
	}

	~VertexArray()
	{
		glDeleteVertexArrays(1, &(this->id));
	}

	void Bind()
	{
		// make VAO current vertex array object in opengl by binding it. This has to be done before binding VBO.
		glBindVertexArray(this->id);
	}

	void Unbind()
	{
		// unbinding by binding to 0
		glBindVertexArray(0);
	}

};