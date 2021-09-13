#pragma once

#include <string>
#include <fstream>

class FileReader
{
	private:
	//static std::exception* a = new std::exception("message");


	public:
	FileReader()
	{
	}

	static bool DoesFileExist(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (file)
			return true;
		else
			return false;
	}

	static std::string Read(const std::string& filepath)
	{
		if (filepath.empty())
		{
			throw std::exception
			(
				std::string
				(
					"File reading failure. File path is empty."
				).c_str()
			);
		}

		std::ifstream file(filepath);
		std::string line;
		std::string file_text = "";
		
		if (file)
		{
			while (getline(file, line))			// reading shader source code line by line from a file
			{
				file_text += line + "\n";
			}
			return file_text;
		}
		else
		{
			throw std::exception
			(
				std::string
				(
					"File reading failure. File does not exist or filepath \'" + 
					filepath + "\' is incorrect."
				).c_str()
			);
		}
	}

	static std::string GetFileExtension(const std::string& filepath)
	{
		std::size_t extension_dot_pos = filepath.find_last_of('.');
		std::size_t extension_substr_len = filepath.size() - extension_dot_pos;
		std::string file_extension = filepath.substr(extension_dot_pos, extension_substr_len);
		return file_extension;
	}
};