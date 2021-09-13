#pragma once

#ifndef __glew_h__
	#include <GL/glew.h>		// does not contain #pragma once
#endif

#define NULL 0

class Shader
{
	private:
	unsigned int id;
	const char* source;

	// auxiliary, not obligatory
	std::string filename;



	public:
	Shader(const char* shader_source, GLenum GL_shader_type, std::string filename = "unknown. Use stack trace to watch.")
		:
		source(shader_source),
		filename(filename)
	{
		this->id = glCreateShader(GL_shader_type);			// initialize shader object in opengl and get reference
		glShaderSource(this->id, 1, &(this->source), NULL);	// pass a shader code to a shader object we created
		glCompileShader(this->id);							// compile the shader
	
		// get errors messages from the shader compiler
		GLint is_success = 0;
		glGetShaderiv(this->id, GL_COMPILE_STATUS, &is_success);
		if (is_success == GL_FALSE)
		{
			std::exception
			(
				std::string
				(
					"GLSL compiler error: \n"
					"filepath: \t" + this->filename + "\n"
					"errorlog: \n" + this->GetGLSLCompilerErrorLog().c_str() + "\n"
					"filename: use IDE's stack trace to"
				).c_str()
			);
		}
	}

	~Shader()
	{
		glDeleteShader(this->id);	// delete shader object from opengl machine
	}

	unsigned int GetId()
	{
		return this->id;
	}

	std::string GetGLSLCompilerErrorLog()
	{
		GLint info_log_length;
		glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &info_log_length);
		GLchar* buffer = new GLchar[info_log_length];

		GLsizei buffer_size;
		glGetShaderInfoLog(this->id, info_log_length, &buffer_size, buffer);

		std::string error_log = buffer;
		delete[] buffer;

		return error_log;
	}

};