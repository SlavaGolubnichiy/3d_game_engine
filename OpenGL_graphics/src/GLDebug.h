#pragma once

#ifndef __glew_h__
	#include <GL/glew.h>
#endif

#include <iostream>



void GLAPIENTRY MessageCallback
(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam
)
{
	std::cout << "OpenGL error(" << "0x" << std::hex << type << "): " << std::endl << 
		"\t source: \t"		<< std::hex << "0x" << source << std::endl <<
		"\t id: \t\t"		<< std::dec << id << std::endl <<
		"\t severity: \t"	<< std::dec << severity << std::endl <<
		"\t length: \t"		<< std::dec << length << std::endl <<
		"\t message: \t"	<< message << std::endl;

	__debugbreak();		/// to trace the error source watch "Function calls stack trace" during debug session.

}



void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function, const char *file, int line)
{
	while (GLenum error = glGetError())		// loop with assignment will run while error != 0
	{
		std::cout << "[OpenGL error] (" << error << ")" << std::endl <<
			function << std::endl <<
			file << " : " << "line " << line << std::endl << std::endl;
		return false;
	}

	return true;
}

										//	__debugbreak(); - is a macros that places a compilation breakpoint 
										//	(functions with names starting with __ (two underscores)(in Visual Studio) - are compiler's internal functions => they won't work in other compilers)
#define ASSERT(x) if (!(x)) __debugbreak();

											// #x - turns x into a string, __FILE__ - retrieves the filepath of "x" (???????????????), __LINE__ - retrieves the line of "x" (????????????????)
											// not closing		ASSERT(GLLogCall())		, because GLCall()	macros call will be ended with a semicolon ';'
											// don't forget not to place the 'space' after a backslash '\'
#define GLCall(x)	GLClearError();		\
					x;					\
					ASSERT(GLLogCall(#x, __FILE__, __LINE__))