#pragma once

#ifndef __glew_h__
	#include <GL/glew.h>		// does not contain #pragma once
#endif

#include <vector>
#include <iostream>

#include "ShaderProgram.h"

/// SAFE_SET_METHOD is used because in order to set a value to UNIFORM, located in 
/// the shader, we need to call glUseProgram(shader_program_id) first. 
// However, sometimes user can call SetUniformXX(...) for a shader_program,
// that is not used at the moment. In order to prevent the data going to the wrong
// destination or error, we need to check whether if 
// target (this) shader_program or different shader_program is in use by OpenGL now.
// if this => set_uniform(uniform_name, new_value);
// else if different => 
//			save previous shader_program;
//			use this shader_program;
//			set_uniform(uniform_name, new_value);
//			use previous shader_program;
#define SAFE_GET_SET_METHOD(x)										\
	GLint shader_program_being_used;								\
	glGetIntegerv(GL_CURRENT_PROGRAM, &shader_program_being_used);	\
	if ((unsigned int)shader_program_being_used == this->id)		\
	{																\
		x;											\
	}												\
	else											\
	{												\
		this->UseProgram();							\
		x;											\
		glUseProgram(shader_program_being_used);	\
	}												\

ShaderProgram::ShaderProgram(unsigned int vertex_shader_id, unsigned int fragment_shader_id)
{
	this->id = glCreateProgram();					// initialize a shader program object in opengl
	glAttachShader(this->id, vertex_shader_id);		// attach shaders to an existing shader program
	glAttachShader(this->id, fragment_shader_id);
	glLinkProgram(this->id);						// link (wrap-up) all the shaders together in the shader program
	// get info about was linking successful
	GLint is_success = 0;
	glGetProgramiv(this->id, GL_LINK_STATUS, &is_success);
	if (is_success == GL_FALSE)
	{
		std::exception
		(
			std::string
			(
				"OpenGL shader program linker error: " + this->GetGLSLLinkerErrorLog() + "\n"
			).c_str()
		);
	}
	// safe shaders and shader program resources release
	glDetachShader(this->id, vertex_shader_id);
	glDetachShader(this->id, fragment_shader_id);
	
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->id);
}

unsigned int ShaderProgram::GetId()
{
	return this->id;
}

bool IsThisProgramInUse()
{
	return true;
}

void ShaderProgram::UseProgram()
{
	glUseProgram(this->id);		// use shader program in the following draw() calls
}

std::string ShaderProgram::GetGLSLLinkerErrorLog()
{
	GLint info_log_length = 0;
	glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &info_log_length);
	GLchar* buffer = new GLchar[info_log_length];
	GLsizei buffer_size;
	glGetProgramInfoLog(this->id, info_log_length, &buffer_size, buffer);

	glDeleteProgram(this->id);

	std::string error_log = buffer;
	delete[] buffer;

	return error_log;
}

#pragma region uniforms_methods

bool ShaderProgram::DoesUniformExist(std::string inshader_uniform_name)
{
	GLint program_uniforms_count;
	glGetProgramiv(this->id, GL_ACTIVE_UNIFORMS, &program_uniforms_count);
	
	GLuint uniform_location = glGetUniformLocation(this->id, inshader_uniform_name.c_str());
	if (uniform_location > (unsigned int)program_uniforms_count)
	{
		return false;
	}
	else
	{
		return true;
	}
}

GLfloat ShaderProgram::GetUniform1f(std::string inshader_uniform_name)
{
	if (DoesUniformExist(inshader_uniform_name))
	{
		GLint uniform_location = glGetUniformLocation(this->id, inshader_uniform_name.c_str());
		GLfloat retrieved_uniform_value = 0.0f;
		SAFE_GET_SET_METHOD		// calls this.UseProgram(); if this program is not in usage
		(
			glGetUniformfv(this->id, uniform_location, &retrieved_uniform_value);
		);
		return retrieved_uniform_value;
	}
	else
	{
		throw std::exception
		(
			std::string("Uniform \'" + inshader_uniform_name + "\' does not exist in any shader, compiled to this shader program.").c_str()
		);
	}
}

GLuint ShaderProgram::GetUniform1ui(std::string inshader_uniform_name)
{
	if (DoesUniformExist(inshader_uniform_name))
	{
		GLint uniform_location = glGetUniformLocation(this->id, inshader_uniform_name.c_str());
		GLuint retrieved_uniform_value = 0;
		SAFE_GET_SET_METHOD		// calls this.UseProgram(); if this program is not in usage
		(
			glGetUniformuiv(this->id, uniform_location, &retrieved_uniform_value);
		);
		return retrieved_uniform_value;
	}
	else
	{
		throw std::exception
		(
			std::string("Uniform \'" + inshader_uniform_name + "\' does not exist in any shader, compiled to this shader program.").c_str()
		);
	}
}

void ShaderProgram::SetUniform1f(std::string inshader_uniform_name, GLfloat value)
{
	if (DoesUniformExist(inshader_uniform_name))
	{
		GLuint uniform_location = glGetUniformLocation(this->id, inshader_uniform_name.c_str());
		SAFE_GET_SET_METHOD		// calls this.UseProgram(); if this program is not in usage
		(
			glUniform1f(uniform_location, value)
		);
	}
	else
	{
		throw std::exception
		(
			std::string("Uniform \'" + inshader_uniform_name + "\' does not exist in any shader, compiled to this shader program.").c_str()
		);
	}
}

void ShaderProgram::SetUniform1ui(std::string inshader_uniform_name, GLuint value)
{
	if (DoesUniformExist(inshader_uniform_name))
	{
		GLuint uniform_location = glGetUniformLocation(this->id, inshader_uniform_name.c_str());
		SAFE_GET_SET_METHOD		// calls this.UseProgram(); if this program is not in usage
		(
			// pass a value to uniform, referenced by uniform's id. 
			// must always be done after ShaderProgram.UseProgram()
			glUniform1i(uniform_location, value)
		);
	}
	else
	{
		throw std::exception
		(
			std::string("Uniform \'" + inshader_uniform_name + "\' does not exist in any shader, compiled to this shader program.").c_str()
		);
	}
}

void ShaderProgram::SetUniformMatrix4f(std::string inshader_uniform_name, const GLfloat* matrix_data)
{
	if (DoesUniformExist(inshader_uniform_name))
	{
		GLuint uniform_location = glGetUniformLocation(this->id, inshader_uniform_name.c_str());
		SAFE_GET_SET_METHOD		// calls this.UseProgram(); if this program is not in usage
		(
			// pass a value to uniform, referenced by uniform's id. 
			// must always be done after ShaderProgram.UseProgram()
			glUniformMatrix4fv(uniform_location, 1, GL_FALSE, matrix_data)
		);
	}
	else
	{
		throw std::exception
		(
			std::string("Uniform \'" + inshader_uniform_name + "\' does not exist in any shader, compiled to this shader program.").c_str()
		);
	}
}

void ShaderProgram::SetUniformVec3(std::string inshader_uniform_name, const GLfloat* vector_data)
{
	if (DoesUniformExist(inshader_uniform_name))
	{
		GLuint uniform_location = glGetUniformLocation(this->id, inshader_uniform_name.c_str());
		SAFE_GET_SET_METHOD		// calls this.UseProgram(); if this program is not in usage
		(
			// pass a value to uniform, referenced by uniform's id. 
			// must always be done after ShaderProgram.UseProgram()
			glUniform3f(uniform_location, *vector_data, *(vector_data + 1), *(vector_data+2))
		);
	}
	else
	{
		throw std::exception
		(
			std::string("Uniform \'" + inshader_uniform_name + "\' does not exist in any shader, compiled to this shader program.").c_str()
		);
	}
}

#pragma endregion