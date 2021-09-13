#pragma once

#ifndef __glew_h__
	#include <GL/glew.h>		// does not contain #pragma once
#endif

#ifndef _glfw3_h_
	#include <GLFW/glfw3.h>		// does not contain #pragma once
#endif

#include "util/aux_.h"
#include "Texture2DData.h"

#ifndef STBI_INCLUDE_STB_IMAGE_H
	#include "vendor/stb_image/stb_image.h"		// does not contain #pragma once
#endif

#include <string>



class Texture2D
{
	private:
	int width = 0;
	int height = 0;
	int color_channels_num = 0;
	unsigned char* img_bytes = nullptr;
	std::string img_file_extension;
	GLenum GL_color_format;
	GLenum GL_texture_slot;
	GLenum GL_texture_min_filter;
	GLenum GL_texture_mag_filter;
	GLenum GL_texture_wrap_s;
	GLenum GL_texture_wrap_t;
	float* texture_border_color = new float[4];
	GLuint id = 0;

	void GenerateTexture(GLenum GL_texture_slot)
	{
		// make a reservation of memory for the texture
		glGenTextures(1, &this->id);
		// texture slot is already validated by texture data class
		glActiveTexture(this->GL_texture_slot);
		// make texture current to the following OpenGL commands
		glBindTexture(GL_TEXTURE_2D, this->id);
		// specify texture minification and magnification filters
		/*
			GL_NEAREST = pixels values will have a value of the nearest pixel. image will remain pixelated after scaling
			GL_LINEAR = pixels values will have an average value of nearest pixels values. image will be blurry after scaling
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->GL_texture_min_filter);	// filter used when image is scaled down
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->GL_texture_mag_filter);	// filter used when image is scaled up
		// specify the texture wrap method
		/*
			GL_REPEAT = texture will be repeated in all direction until reaching the object borders
			GL_MIRRORED_REPEAT = texture will be repeated with mirroring in all directions on the object
			GL_CLAMP_TO_EDGE = pixels on edge of the texture will be repeated till reaching the object's border frame
			GL_CLAMP_TO_BORDER = pixels out of the texture (when texture is inside the object's borders) will be filled with color specified by GL_TEXTURE_BORDER_COLOR param
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->GL_texture_wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->GL_texture_wrap_t);
		if (this->GL_texture_wrap_s == GL_CLAMP_TO_BORDER || this->GL_texture_wrap_t == GL_CLAMP_TO_BORDER)
		{
			// specifying the GL_TEXTURE_BORDER_COLOR parameter value
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, this->texture_border_color);
		}
		// finally, pass the DATA of the texture image, memory was reserved for
		glTexImage2D(GL_TEXTURE_2D, 0, this->GL_color_format, this->width, this->height, 0, this->GL_color_format, GL_UNSIGNED_BYTE, this->img_bytes);
		glGenerateMipmap(GL_TEXTURE_2D);
		// make texture not current texture in OpenGL state machine context
		glBindTexture(GL_TEXTURE_2D, 0);
	}



	public:

	static unsigned int GetOpenGLCurrentActiveTextureSlot()
	{
		GLint active_texture = 0;
		glGetIntegerv(GL_ACTIVE_TEXTURE, &active_texture);
		return active_texture;
	}

	private:
	/**
		@description
		copy description from Texture2DData::constructor(... many args ...) if this constructor is being used
	
	*/
	// Write description with note, that float* GL_texture_border_color has to be 
	// an array of, at least, 3 initialized floats in order to be interpreted as a valid color
		/// .png has RGBA color format, .jpg and .jpeg have RGB color format.
		// If .jpg or .jpeg texture image is passed and GL_img_color_format set to GL_RGBA =>
		// result is undefined.
		// If .png texture image is passed and GL_img_color_format set to GL_RGB =>
		// result is undefined.
	Texture2D
	(
		std::string img_path,
		GLenum GL_color_format,
		GLenum GL_texture_slot,
		GLenum GL_texture_min_filter,
		GLenum GL_texture_mag_filter,
		GLenum GL_texture_wrap_s,
		GLenum GL_texture_wrap_t,
		aux::color::RGBAColor texture_backgr_color
	)
		:
		img_file_extension(FileReader::GetFileExtension(img_path)),
		GL_color_format(GL_color_format),
		GL_texture_slot(GL_texture_slot),
		GL_texture_min_filter(GL_texture_min_filter),
		GL_texture_mag_filter(GL_texture_mag_filter),
		GL_texture_wrap_s(GL_texture_wrap_s),
		GL_texture_wrap_t(GL_texture_wrap_t),
		texture_border_color(new float[4] {texture_backgr_color.r, texture_backgr_color.g, texture_backgr_color.b, texture_backgr_color.a})
	{
		/// validate input params
		// also, separate this constructor from Constructor(Texture2DData*)

		// flip image when loaded, because stb_image reads images from top left, while OpenGl - from bottom left corner
		stbi_set_flip_vertically_on_load(true);
		this->img_bytes = stbi_load(std::string(img_path).c_str(), &this->width, &this->height, &this->color_channels_num, 0);
		this->GenerateTexture(this->GL_texture_slot);
	}

	public:
	// valid data provided by Texture2DData class.
	Texture2D(const Texture2DData* texture_data)
		// this is a syntax for calling a constructor from the constructor
		// constructor used there is called "delegating constructor"
		// this feature is available since C++11. 
		:
		Texture2D
		(
			texture_data->GetImagePath(),
			texture_data->GetColorFormat(),
			texture_data->GetTextureSlot(),
			texture_data->GetTextureMinFilter(),
			texture_data->GetTextureMagFilter(),
			texture_data->GetTextureWrapS(),
			texture_data->GetTextureWrapT(),
			texture_data->GetTextureBackgroundColor()
		)
	{
	}

	// non-assignable yet
	void operator=(const Texture2D& other)
	{
		
	}

	~Texture2D()
	{
		// free the memory
		stbi_image_free(img_bytes);
		this->img_bytes = nullptr;
		// delete the texture data from OpenGL machine (and, also, VRAM)
		glDeleteTextures(1, &this->id);
		// free border color memory
		delete[] this->texture_border_color;
		this->texture_border_color = nullptr;
	}

	void Bind()
	{
		glActiveTexture(this->GL_texture_slot);
		// make texture current for OpenGL
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	void Unbind()
	{
		// make texture not current for OpenGL
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLuint GetId()
	{
		return this->id;
	}

	GLuint GetTextureUniformValue()
	{
		// when slot = GL_TEXTURE0 [0x84C0], uniform_value = 0
		// when slot = GL_TEXTURE1 [0x84C1], uniform_value = 1
		// when slot = GL_TEXTURE2 [0x84C2], uniform_value = 2
		// etc.
		return this->GL_texture_slot - GL_TEXTURE0;
	}


};