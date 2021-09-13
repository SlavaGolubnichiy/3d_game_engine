#pragma once

#ifndef __glew_h__
	#include <GL/glew.h>		// does not contain #pragma once
#endif

#include "vendor/glm/glm.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VertexBuffer
{
	private:
	unsigned int id;



	public:
	VertexBuffer(float* vertices, unsigned int vertices_size)
	{
		// generate buffer for 1 single object, get buffer's reference into (this->id).
		glGenBuffers(1, &(this->id));
		this->Bind();
		glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(float), vertices, GL_DYNAMIC_DRAW);		// passing a vertices data into vertex buffer object.
	}

	VertexBuffer(std::vector<Vertex>& vertices)
	{
		glGenBuffers(1, &(this->id));
		this->Bind();
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
	}

	~VertexBuffer()
	{
		glDeleteBuffers(1, &(this->id));
	}

	void Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->id);	// ----------------------------------------------- bind object -[means]- make object current for opengl, specify that we bind buffer of type GL_ARRAY_BUFFER
	}

	void LinkVertexAttrib
	(
		unsigned int layout,
		unsigned int component_size_in_floats,
		GLenum type,
		GLsizeiptr stride,
		void* offset
	)
	{
		this->Bind();
		glVertexAttribPointer(layout, component_size_in_floats, type, GL_FALSE, stride, offset);	// configure vertex attribute pointer (pos, dimensions, vertex_component_type, do_use_int_coords (T/F), vertex_size, nullptr)
																												// configure Vertex Attribute so that OpenGL knows how to read the VBO. 
		glEnableVertexAttribArray(layout);	// ------------------------------------------------------------------- enable vertex attribute pointer (vertex_attrib_ptr_position). So that OpenGL knows to use it.
		this->Unbind();
	}

	void Unbind()
	{
		// unbinding by binding to 0
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void UpdateVerticesData(float* new_vertices_data, unsigned int new_vertices_data_size)
	{
		this->Bind();
		glBufferData(GL_ARRAY_BUFFER, new_vertices_data_size * sizeof(float), new_vertices_data, GL_STATIC_DRAW);		// passing a vertices data into vertex buffer object.
		this->Unbind();
	}

};