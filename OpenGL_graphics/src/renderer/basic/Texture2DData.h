#pragma once

#ifndef __glew_h__
	#include <GL/glew.h>		// does not contain #pragma once
#endif

#ifndef _glfw3_h_
	#include <GLFW/glfw3.h>		// does not contain #pragma once
#endif

#include "util/aux_.h"
#include "util/FileReader.h"

#ifndef STBI_INCLUDE_STB_IMAGE_H
	#include "vendor/stb_image/stb_image.h"		// does not contain #pragma once
#endif

#include <vector>
#include <string>



class Texture2DData
{
	private:
	const std::vector<std::string> images_extensions_allowed =
	{
		".png", ".jpg", ".jpeg"
	};
	// const std::unordered_map img_extension_to_GL_color_formats [1] [all=2]

	std::string image_path;
	GLenum GL_color_format;
	GLenum GL_texture_slot;
	GLenum GL_texture_min_filter;
	GLenum GL_texture_mag_filter;
	GLenum GL_texture_wrap_s;
	GLenum GL_texture_wrap_t;
	aux::color::RGBAColor texture_backgr_color {0.0f, 0.0f, 0.0f, 0.0f};

	

	public:

	#pragma region validators

	/**
		@description
		This method validates symbols in the filepath string and ckecks does file exist.
	*/
	bool TryIsImagePathValid(std::string image_path)
	{
		// check if image_path is not empty
		if (image_path.empty())
		{
			throw std::exception(std::string("image_path value of \'" + image_path + "\' is empty, so it's not valid. \n").c_str());
		}

		// check if image_path has no OS-forbidden symbols
		const unsigned int forbidden_symbols_size = 9;
		const char forbidden_symbols[forbidden_symbols_size]{ ':', '*', '?', '"', '<', '>', '|' };
		for (unsigned int i = 0; i < forbidden_symbols_size; i++)
		{
			size_t forbidden_symbol_pos = image_path.find(forbidden_symbols[i]);
			if (forbidden_symbol_pos != std::string::npos)
			{
				throw std::exception
				(
					std::string
					(
						"Image path \'" + image_path + "\' has forbidden symbol at " + 
						std::to_string((unsigned int) forbidden_symbol_pos) + "'. \n" +
						"Unallowed image path's symbols: " + forbidden_symbols + "\n"
					).c_str()
				);
				break;
			}
		}

		// check if image_path's file extension represents a texture file 
		bool is_extension_valid = false;
		std::string image_extension = FileReader::GetFileExtension(image_path);
		for (unsigned int i = 0; i < this->images_extensions_allowed.size(); i++)
		{
			if (image_extension == this->images_extensions_allowed[i])
			{
				is_extension_valid = true;
				break;
			}
		}
		if (!is_extension_valid)
		{
			std::string allowed_values = "";
			for (unsigned int i = 0; i < this->images_extensions_allowed.size(); i++)
			{
				allowed_values += this->images_extensions_allowed[i] + ", ";
			}
			allowed_values.erase(allowed_values.size() - 2);
			allowed_values += ".";

			throw std::exception
			(
				std::string
				(
					"Image extension, specified in image path \'" + image_path + "\' is not allowed. \n" +
					"Image extensions allowed: " + allowed_values + "\n"
				).c_str()
			);
		}

		// check does file exist
		if (!FileReader::DoesFileExist(image_path))
		{
			throw std::exception
			(
				std::string("Image path" + image_path + " is invalid. File was not found.").c_str()
			);
		}

		return true;
	}

	bool TryIsColorFormatValid(GLenum GL_color_format) const
	{
		std::string image_file_extension = FileReader::GetFileExtension(this->image_path);
		GLenum GL_color_format_should_be;
		if (image_file_extension == ".png")
		{
			GL_color_format_should_be = GL_RGBA;
		}
		else if (image_file_extension == ".jpg" || image_file_extension == ".jpeg")
		{
			GL_color_format_should_be = GL_RGB;
		}

		if (GL_color_format != GL_color_format_should_be)
		{
			throw
			(
				std::string
				(
					"Image file " + image_file_extension + "extension is not allowed or " /* delete this when std::unordered_map used */ 
					"(image_path.extension -> GL_color_format) relation is invalid."
				).c_str()
				// make string with std::unordered_map img_extension_to_GL_color_formats full info [2]
			);
		}

		return true;
	}

	bool TryIsTextureSlotValid(GLenum GL_texture_slot)
	{
		if (GL_TEXTURE0 <= GL_texture_slot && GL_texture_slot <= GL_TEXTURE31)
		{
			return true;
		}
		else
		{
			throw std::exception
			(
				std::string
				(
					"GL_texture_slot value of " + std::to_string(GL_texture_slot) + "is not valid. \n" +
					"GL_texture_slot value has to be in range [0; 31]."
				).c_str()
			);
		}
	}

	bool TryIsTextureMinMagFilterValid(GLenum GL_texture_min_or_mag_filter)
	{
		if (GL_texture_min_or_mag_filter == GL_NEAREST || GL_texture_min_or_mag_filter == GL_LINEAR)
		{
			return true;
		}
		else
		{
			throw std::exception
			(
				std::string
				(
					"Neither GL_texture_min_filter nor GL_texture_mag_filter can have a value of " +
					std::to_string(GL_texture_min_or_mag_filter) + ". It's not valid. "
					"List of valid values: \n"
					"\t GL_NEAREST (" + std::to_string(GL_NEAREST) + "),"
					"\t GL_LINEAR (" + std::to_string(GL_LINEAR) + ")."
				).c_str()
			);
		}
	}

	bool TryIsTextureWrapSWrapTValid(GLenum GL_texture_wrap_s_or_wrap_t)
	{
		if
			(
				GL_texture_wrap_s_or_wrap_t == GL_REPEAT ||
				GL_texture_wrap_s_or_wrap_t == GL_MIRRORED_REPEAT ||
				GL_texture_wrap_s_or_wrap_t == GL_CLAMP_TO_EDGE ||
				GL_texture_wrap_s_or_wrap_t == GL_CLAMP_TO_BORDER
				)
		{
			return true;
		}
		else
		{
			throw std::exception
			(
				std::string
				(
					"Neither GL_texture_wrap_s nor GL_texture_wrap_t can have a value of " +
					std::to_string(GL_texture_wrap_s_or_wrap_t) + ". It's not valid. "
					"List of valid values: \n"
					"\t GL_REPEAT (" + std::to_string(GL_REPEAT) + "),"
					"\t GL_MIRRORED_REPEAT (" + std::to_string(GL_MIRRORED_REPEAT) + "),"
					"\t GL_CLAMP_TO_EDGE (" + std::to_string(GL_CLAMP_TO_EDGE) + "),"
					"\t GL_CLAMP_TO_BORDER (" + std::to_string(GL_CLAMP_TO_BORDER) + ")."
				).c_str()
			);
		}
	}

	// aux::color::TryIsColorValid() is used instead of local color validator

	#pragma endregion
	
	/**
		@description

		@params
		image_path - not empty string with a valid filepath (forbidden symbols are :*?"<>| ) to an image
		GL_color_format - 
		GL_texture_slot - 
		GL_texture_min_filter - 
		GL_texture_mag filter - 
		GL_texture_wrap_s - 
		GL_texture_wrap_t - 
		texture_border_color - pointer to an array of 3 floats in range [0.0f, 1.0f] each.

	*/
	// Write description with note, that float* GL_texture_border_color has to be 
	// an array of, at least, 3 initialized floats in order to be interpreted as a valid color
		/// .png has RGBA color format, .jpg and .jpeg have RGB color format.
		// If .jpg or .jpeg texture image is passed and GL_img_color_format set to GL_RGBA =>
		// result is undefined.
		// If .png texture image is passed and GL_img_color_format set to GL_RGB =>
		// result is undefined.
	// Implement correct description for a method (that will be shown in a pop-up window 
	// when method is typed in)
	Texture2DData
	(
		std::string image_path,
		GLenum GL_color_format,
		GLenum GL_texture_slot,
		GLenum GL_texture_min_filter = GL_NEAREST,
		GLenum GL_texture_mag_filter = GL_NEAREST,
		GLenum GL_texture_wrap_s = GL_REPEAT,
		GLenum GL_texture_wrap_t = GL_REPEAT,
		aux::color::RGBAColor texture_backgr_color = {0.5f, 0.5f, 0.5f, 0.5f}
	)
		:
		image_path(image_path),
		GL_color_format(GL_color_format),
		GL_texture_slot(GL_texture_slot),
		GL_texture_min_filter(GL_texture_min_filter),
		GL_texture_mag_filter(GL_texture_mag_filter),
		GL_texture_wrap_s(GL_texture_wrap_s),
		GL_texture_wrap_t(GL_texture_wrap_t),
		texture_backgr_color(texture_backgr_color)		// when "struct a = struct b;" only data being copied (pointers values are different)
	{
		TryIsImagePathValid(image_path);
		TryIsTextureSlotValid(GL_texture_slot);
		TryIsColorFormatValid(GL_color_format);
		TryIsTextureMinMagFilterValid(GL_texture_min_filter);
		TryIsTextureMinMagFilterValid(GL_texture_mag_filter);
		TryIsTextureWrapSWrapTValid(GL_texture_wrap_s);
		TryIsTextureWrapSWrapTValid(GL_texture_wrap_t);
		aux::color::TryIsColorValid(texture_backgr_color);
	}

	Texture2DData(const Texture2DData& other)
		:
		Texture2DData
		(
			other.GetImagePath(),
			other.GetColorFormat(),
			other.GetTextureSlot(),
			other.GetTextureMinFilter(),
			other.GetTextureMagFilter(),
			other.GetTextureWrapS(),
			other.GetTextureWrapT(),
			other.GetTextureBackgroundColor()
		)
	{
	}

	#pragma region getters

	std::vector<std::string> GetImageExtensionsAllowed()
	{
		return this->images_extensions_allowed;
	}

	std::string GetImagePath() const 
	{
		return this->image_path;
	}
	GLenum GetColorFormat() const 
	{
		return this->GL_color_format;
	}
	GLenum GetTextureSlot() const
	{
		return this->GL_texture_slot;
	}
	GLenum GetTextureMinFilter() const
	{
		return this->GL_texture_min_filter;
	}
	GLenum GetTextureMagFilter() const
	{
		return this->GL_texture_mag_filter;
	}
	GLenum GetTextureWrapS() const
	{
		return this->GL_texture_wrap_s;
	}
	GLenum GetTextureWrapT() const
	{
		return this->GL_texture_wrap_t;
	}
	aux::color::RGBAColor GetTextureBackgroundColor() const
	{
		return this->texture_backgr_color;
	}
	
	#pragma endregion

};