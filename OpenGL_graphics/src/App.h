#pragma once

#ifndef __glew_h__
	#include <GL/glew.h>		// does not contain #pragma once
#endif

#ifndef _glfw3_h_
	#include <GLFW/glfw3.h>		// does not contain #pragma once
#endif

#include <iostream>
#include <string>

#include "GLFWManagerSingleton.h"
#include "renderer/basic/Texture2D.h"
#include "renderer/OpenglObj3DData.h"
#include "renderer/OpenglObj3DDataGenerator.h"
#include "renderer/OpenglObj.h"
#include "Camera.h"
#include "renderer/Mesh.h"

#include "GLDebug.h"
#include "util/aux_.h"
#include "util/FileReader.h"

#include "MouseScrollCallbackInput.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#ifndef STBI_INCLUDE_STB_IMAGE_H
	#include "src/vendor/stb_image/stb_image.h"		// does not contain #pragma once
#endif

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"



std::string to_string(GLenum);


namespace ThisApp
{
	unsigned int main_window_width = 1600;
	unsigned int main_window_height = 900;
	std::string main_window_name = "MyOpenGLWindowName";

	static inline void PrintOpenGLInfo()
	{
		std::cout <<
			"GL version: \t" << glGetString(GL_VERSION) << std::endl <<
			"GL vendor: \t" << glGetString(GL_VENDOR) << std::endl <<
			"GL renderer: \t" << glGetString(GL_RENDERER) << std::endl <<
			"GLSL version: \t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl << std::endl;
	}
}



void PrintUniformsInfo(unsigned int shader_program_id)
{
	GLuint info_len = 256;
	GLchar* info = new GLchar[info_len];
	GLsizei actual_len = 0;
	GLenum uniform_type = 0;
	GLint uniform_size = 0;

	// get active uniforms count
	GLint active_uniforms_count = 0;
	glGetProgramiv(shader_program_id, GL_ACTIVE_UNIFORMS, &active_uniforms_count);

	std::cout << "Shader program [id=" << shader_program_id << "] uniforms info: \n";

	// print each uniform info
	for (int i = 0; i < active_uniforms_count; i++)
	{
		glGetActiveUniform(shader_program_id, i, info_len, &actual_len, &uniform_size, &uniform_type, info);
		//info[actual_len] = '\0';
		std::cout <<
			"[" << i << "] uniform info: \n"
			"\t text_info = " << info << "\n"
			"\t uniform_type = " << to_string(uniform_type) << "\n"
			"\t uniform size = " << uniform_size << "\n";
	}
	std::cout << "End of shader program [id=" << shader_program_id << "] uniforms info.";
}


std::string to_string(GLenum gl_uniform_type_enum)
{
	switch (gl_uniform_type_enum)
	{
		case	GL_FLOAT									:	return	"float					_";
		case	GL_FLOAT_VEC2								:	return	"vec2					_";
		case	GL_FLOAT_VEC3								:	return	"vec3					_";
		case	GL_FLOAT_VEC4								:	return	"vec4					_";
		case	GL_DOUBLE									:	return	"double					_";
		case	GL_DOUBLE_VEC2								:	return	"dvec2					_";
		case	GL_DOUBLE_VEC3								:	return	"dvec3					_";
		case	GL_DOUBLE_VEC4								:	return	"dvec4					_";
		case	GL_INT										:	return	"int					_";
		case	GL_INT_VEC2									:	return	"ivec2					_";
		case	GL_INT_VEC3									:	return	"ivec3					_";
		case	GL_INT_VEC4									:	return	"ivec4					_";
		case	GL_UNSIGNED_INT								:	return	"unsigned int			_";
		case	GL_UNSIGNED_INT_VEC2						:	return	"uvec2					_";
		case	GL_UNSIGNED_INT_VEC3						:	return	"uvec3					_";
		case	GL_UNSIGNED_INT_VEC4						:	return	"uvec4					_";
		case	GL_BOOL										:	return	"bool					_";
		case	GL_BOOL_VEC2								:	return	"bvec2					_";
		case	GL_BOOL_VEC3								:	return	"bvec3					_";
		case	GL_BOOL_VEC4								:	return	"bvec4					_";
		case	GL_FLOAT_MAT2								:	return	"mat2					_";
		case	GL_FLOAT_MAT3								:	return	"mat3					_";
		case	GL_FLOAT_MAT4								:	return	"mat4					_";
		case	GL_FLOAT_MAT2x3								:	return	"mat2x3					_";
		case	GL_FLOAT_MAT2x4								:	return	"mat2x4					_";
		case	GL_FLOAT_MAT3x2								:	return	"mat3x2					_";
		case	GL_FLOAT_MAT3x4								:	return	"mat3x4					_";
		case	GL_FLOAT_MAT4x2								:	return	"mat4x2					_";
		case	GL_FLOAT_MAT4x3								:	return	"mat4x3					_";
		case	GL_DOUBLE_MAT2								:	return	"dmat2					_";
		case	GL_DOUBLE_MAT3								:	return	"dmat3					_";
		case	GL_DOUBLE_MAT4								:	return	"dmat4					_";
		case	GL_DOUBLE_MAT2x3							:	return	"dmat2x3				_";
		case	GL_DOUBLE_MAT2x4							:	return	"dmat2x4				_";
		case	GL_DOUBLE_MAT3x2							:	return	"dmat3x2				_";
		case	GL_DOUBLE_MAT3x4							:	return	"dmat3x4				_";
		case	GL_DOUBLE_MAT4x2							:	return	"dmat4x2				_";
		case	GL_DOUBLE_MAT4x3							:	return	"dmat4x3				_";
		case	GL_SAMPLER_1D								:	return	"sampler1D				_";
		case	GL_SAMPLER_2D								:	return	"sampler2D				_";
		case	GL_SAMPLER_3D								:	return	"sampler3D				_";
		case	GL_SAMPLER_CUBE								:	return	"samplerCube			_";
		case	GL_SAMPLER_1D_SHADOW						:	return	"sampler1DShadow		_";
		case	GL_SAMPLER_2D_SHADOW						:	return	"sampler2DShadow		_";
		case	GL_SAMPLER_1D_ARRAY							:	return	"sampler1DArray			_";
		case	GL_SAMPLER_2D_ARRAY							:	return	"sampler2DArray			_";
		case	GL_SAMPLER_1D_ARRAY_SHADOW					:	return	"sampler1DArrayShadow	_";
		case	GL_SAMPLER_2D_ARRAY_SHADOW					:	return	"sampler2DArrayShadow	_";
		case	GL_SAMPLER_2D_MULTISAMPLE					:	return	"sampler2DMS			_";
		case	GL_SAMPLER_2D_MULTISAMPLE_ARRAY				:	return	"sampler2DMSArray		_";
		case	GL_SAMPLER_CUBE_SHADOW						:	return	"samplerCubeShadow		_";
		case	GL_SAMPLER_BUFFER							:	return	"samplerBuffer			_";
		case	GL_SAMPLER_2D_RECT							:	return	"sampler2DRect			_";
		case	GL_SAMPLER_2D_RECT_SHADOW					:	return	"sampler2DRectShadow	_";
		case	GL_INT_SAMPLER_1D							:	return	"isampler1D				_";
		case	GL_INT_SAMPLER_2D							:	return	"isampler2D				_";
		case	GL_INT_SAMPLER_3D							:	return	"isampler3D				_";
		case	GL_INT_SAMPLER_CUBE							:	return	"isamplerCube			_";
		case	GL_INT_SAMPLER_1D_ARRAY						:	return	"isampler1DArray		_";
		case	GL_INT_SAMPLER_2D_ARRAY						:	return	"isampler2DArray		_";
		case	GL_INT_SAMPLER_2D_MULTISAMPLE				:	return	"isampler2DMS			_";
		case	GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY			:	return	"isampler2DMSArray		_";
		case	GL_INT_SAMPLER_BUFFER						:	return	"isamplerBuffer			_";
		case	GL_INT_SAMPLER_2D_RECT						:	return	"isampler2DRect			_";
		case	GL_UNSIGNED_INT_SAMPLER_1D					:	return	"usampler1D				_";
		case	GL_UNSIGNED_INT_SAMPLER_2D					:	return	"usampler2D				_";
		case	GL_UNSIGNED_INT_SAMPLER_3D					:	return	"usampler3D				_";
		case	GL_UNSIGNED_INT_SAMPLER_CUBE				:	return	"usamplerCube			_";
		case	GL_UNSIGNED_INT_SAMPLER_1D_ARRAY			:	return	"usampler2DArray		_";
		case	GL_UNSIGNED_INT_SAMPLER_2D_ARRAY			:	return	"usampler2DArray		_";
		case	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE		:	return	"usampler2DMS			_";
		case	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:	return	"usampler2DMSArray		_";
		case	GL_UNSIGNED_INT_SAMPLER_BUFFER				:	return	"usamplerBuffer			_";
		case	GL_UNSIGNED_INT_SAMPLER_2D_RECT				:	return	"usampler2DRect			_";
		case	GL_IMAGE_1D									:	return	"image1D				_";
		case	GL_IMAGE_2D									:	return	"image2D				_";
		case	GL_IMAGE_3D									:	return	"image3D				_";
		case	GL_IMAGE_2D_RECT							:	return	"image2DRect			_";
		case	GL_IMAGE_CUBE								:	return	"imageCube				_";
		case	GL_IMAGE_BUFFER								:	return	"imageBuffer			_";
		case	GL_IMAGE_1D_ARRAY							:	return	"image1DArray			_";
		case	GL_IMAGE_2D_ARRAY							:	return	"image2DArray			_";
		case	GL_IMAGE_2D_MULTISAMPLE						:	return	"image2DMS				_";
		case	GL_IMAGE_2D_MULTISAMPLE_ARRAY				:	return	"image2DMSArray			_";
		case	GL_INT_IMAGE_1D								:	return	"iimage1D				_";
		case	GL_INT_IMAGE_2D								:	return	"iimage2D				_";
		case	GL_INT_IMAGE_3D								:	return	"iimage3D				_";
		case	GL_INT_IMAGE_2D_RECT						:	return	"iimage2DRect			_";
		case	GL_INT_IMAGE_CUBE							:	return	"iimageCube				_";
		case	GL_INT_IMAGE_BUFFER							:	return	"iimageBuffer			_";
		case	GL_INT_IMAGE_1D_ARRAY						:	return	"iimage1DArray			_";
		case	GL_INT_IMAGE_2D_ARRAY						:	return	"iimage2DArray			_";
		case	GL_INT_IMAGE_2D_MULTISAMPLE					:	return	"iimage2DMS				_";
		case	GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY			:	return	"iimage2DMSArray		_";
		case	GL_UNSIGNED_INT_IMAGE_1D					:	return	"uimage1D				_";
		case	GL_UNSIGNED_INT_IMAGE_2D					:	return	"uimage2D				_";
		case	GL_UNSIGNED_INT_IMAGE_3D					:	return	"uimage3D				_";
		case	GL_UNSIGNED_INT_IMAGE_2D_RECT				:	return	"uimage2DRect			_";
		case	GL_UNSIGNED_INT_IMAGE_CUBE					:	return	"uimageCube				_";
		case	GL_UNSIGNED_INT_IMAGE_BUFFER				:	return	"uimageBuffer			_";
		case	GL_UNSIGNED_INT_IMAGE_1D_ARRAY				:	return	"uimage1DArray			_";
		case	GL_UNSIGNED_INT_IMAGE_2D_ARRAY				:	return	"uimage2DArray			_";
		case	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE		:	return	"uimage2DMS				_";
		case	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY	:	return	"uimage2DMSArray		_";
		case	GL_UNSIGNED_INT_ATOMIC_COUNTER				:	return	"atomic_uint			_";
		default:	return "unknown or type doesnt exist";
	}
}

