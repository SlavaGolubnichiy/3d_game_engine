#pragma once

#include <iomanip>

#include "vendor/glm/glm.hpp"

namespace aux
{
	template <typename T>
	static void CopyData(const T* src, T* dest, unsigned int elements_to_copy_number)
	{
		for (unsigned int i = 0; i < elements_to_copy_number; i++)
		{
			dest[i] = src[i];
		}
	}

	void PrintArray(float* ar, unsigned int ar_size, unsigned int floats_in_a_row = 10)
	{
		std::string ar_data_text = "";
		for (unsigned int i = 0; i < ar_size; i++)
		{
			ar_data_text += std::to_string(ar[i]) + "f, ";
			if ((i + 1) % floats_in_a_row == 0)
			{
				ar_data_text += "\n";
			}
		}
		std::cout << "Array data:" << std::endl;
		std::cout << ar_data_text.c_str() << std::endl;
	}

	static unsigned int Sum(unsigned int* src_array, unsigned int src_array_size)
	{
		unsigned int sum = 0;
		for (unsigned int i = 0; i < src_array_size; i++)
		{
			sum += src_array[i];
		}
		return sum;
	}

	template <typename T>
	static unsigned int TryGetDataSize(T* arr)
	{
		T buf;
		unsigned long int i = 0;
		while (i < 0xFFFFFFFFFFFFFFFF)
		{
			try
			{
				buf = arr[i];
			}
			catch(...)
			{
				return i;
			}
			i++;
		}
	}

	namespace color
	{
		struct RGBAColor
		{
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			float a = 0.0f;
		};

		bool TryIsColorValid(RGBAColor color)
		{
			bool is_R_valid = (0.0f <= color.r) && (color.r <= 1.0f);
			bool is_G_valid = (0.0f <= color.g) && (color.g <= 1.0f);
			bool is_B_valid = (0.0f <= color.b) && (color.b <= 1.0f);
			bool is_A_valid = (0.0f <= color.a) && (color.a <= 1.0f);
			if (is_R_valid && is_G_valid && is_B_valid && is_A_valid)
			{
				return true;
			}
			else
			{
				throw std::exception
				(
					std::string
					(
						"One or more color components store a float value out of valid range [0.0f; 1.0f]."
					).c_str()
				);
			}
		}
	};


	template<typename T>
	class Array
	{
		private:
		T* data_ptr = nullptr;
		unsigned int data_size = 0;



		public:

		bool TryIsSizeValid(unsigned int size_value)
		{
			if (size_value > 0)
			{
				this->data_size = size_value;
				return true;
			}
			else
			{
				throw std::exception(std::string("Size has to be in range [1; uint_max_value]."));
			}
		}

		Array()
			:
			data_ptr(nullptr),
			data_size(0)
		{
		}

		Array(T* data_ptr, unsigned int data_size)
			:
			data_ptr(new T[data_size]),
			data_size(data_size)
		{
			if (TryIsSizeValid(data_size))
			{
				aux::CopyData<T>(data_ptr, this->data_ptr, data_size);
			}
		}

		// copy constructor
		Array(const Array<T>&& other)
		{
			this->data_ptr = nullptr;
			this->data_ptr = new T[other.GetSize()];
			aux::CopyData<T>(other.GetDataPtr(), this->data_ptr, other.GetSize());
		}

		~Array()
		{
			this->data_ptr = nullptr;
		}

		void SetData(T* data_ptr, unsigned int data_size)
		{
			this->data_ptr = nullptr;
			this->data_ptr = new T[data_size];
			this->data_ptr = aux::CopyData<T>(data_ptr, this->data_ptr, data_size);
			this->data_size = data_size;
		}

		T* GetDataPtr() const
		{
			return this->data_ptr;
		}

		unsigned int GetSize() const
		{
			return this->data_size;
		}
	};

	unsigned int max(unsigned int value1, unsigned int value2)
	{
		// * = return if
		// + = or (logic)
		return value1 * (value1 > value2) + value2 * (value2 > value1);
	}

	glm::vec3 GetVector(glm::vec3 start_point, glm::vec3 end_point)
	{
		return glm::vec3(end_point.x - start_point.x, end_point.y - start_point.y, end_point.z - start_point.z);
	}
};

namespace Default
{
	const char* vertex_shader_source =
		"#version 330 core						\n"
		"layout(location = 0) in vec3 aPos;		\n"
		"layout(location = 1) in vec3 aColor;	\n"

		"// outputs the color for the fragment shader	\n"
		"out vec3 color;								\n"

		"///uniform float scale;	// never declare uniforms that won't be used, else => errors \n"

		"void main()		\n"
		"{					\n"
		"gl_Position =		\n"
		"	vec4			\n"
		"	(				\n"
		"		aPos.x,		\n"
		"		aPos.y,		\n"
		"		aPos.z,		\n"
		"		1.0			\n"
		"	);				\n"

		"// assigns the colors from Vertex Data to \'color\'. \n"
		"color = aColor; \n"
		"} \n"
		"";

	const char* fragment_shader_source =
		"#version 330 core					\n"
		"out vec4 FragColor;				\n"

		"// inputs and outputs must have same names - required to link them \n"
		"in vec3 color;						\n"

		"uniform float scale;				\n"

		"void main()						\n"
		"{									\n"
		"	FragColor = vec4(color, 1.0f);	\n"
		"}									\n"
		"";
};


