#pragma once

#ifndef __glew_h__
	#include <GL/glew.h>		// does not contain #pragma once
#endif

#include <vector>
#include <iostream>

class ShaderProgram
{
	private:
	unsigned int id;

	bool DoesUniformExist(std::string inshader_uniform_name);

	public:
	ShaderProgram(unsigned int vertex_shader_id, unsigned int fragment_shader_id);
	~ShaderProgram();

	unsigned int GetId();
	void UseProgram();
	std::string GetGLSLLinkerErrorLog();

	GLfloat	GetUniform1f(std::string inshader_uniform_name);
	GLuint  GetUniform1ui(std::string inshader_uniform_name);
	void SetUniform1f(std::string inshader_uniform_name, GLfloat value);
	void SetUniform1ui(std::string inshader_uniform_name, GLuint value);
	void SetUniformMatrix4f(std::string inshader_uniform_name, const GLfloat* matrix_data);
	void SetUniformVec3(std::string uniform_name, const GLfloat* vector_data);
};