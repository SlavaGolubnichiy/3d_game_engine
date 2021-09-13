#pragma once

#include <iostream>

#include "Test.h"
#include "renderer/basic/Texture2DData.h"

class Texture2DData_Test
{
	static void Texture2DData_TryIsImagePathValid_empty()
	{


		// rewrite this stuff in several test functions

		std::string dir = "some_dir/textures_dir/";
		std::vector<std::string> filenames =
		{
			"file.png", "texture.png", "pho?o.jpeg", "photo_cropped.jpg"
		};
		for (unsigned int i = 0; i < filenames.size(); i++)
		{
			std::string test = dir + filenames[i];
			std::cout << "path: " << test << std::endl;
			std::size_t extension_dot_pos = test.find_last_of('.');
			std::size_t extension_substr_len = test.size() - extension_dot_pos;
			std::string image_extension = test.substr(extension_dot_pos, extension_substr_len);
			std::cout << "extension: " << image_extension << std::endl;
			std::cout << "Test" << i << ": " <<
				//	(Texture2DData::TryIsImagePathValid(test) ? "passed" : "failed") << 
				std::endl;
		}
	}

	void RunAllTests()
	{
		Texture2DData_TryIsImagePathValid_empty();
	}
};
