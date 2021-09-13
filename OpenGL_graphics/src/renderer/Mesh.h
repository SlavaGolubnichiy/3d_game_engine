#pragma once

#include "renderer/basic/Shader.h"
#include "renderer/basic/ShaderProgram.h"
#include "renderer/basic/VertexArray.h"
#include "renderer/basic/VertexBuffer.h"
#include "renderer/basic/IndexBuffer.h"
#include "renderer/basic/Texture2D.h"
#include "Camera.h"
#include "OpenglObj3DData.h"

#include "util/FileReader.h"
#include "util/aux_.h"

#include <exception>
#include <math.h>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

// class AttribsInfo (original)
/*
class AttribsInfo
{
	private:
	unsigned int size = 0;
	unsigned int* attribs_layouts = nullptr;
	unsigned int* floats_per_attribs = nullptr;
	unsigned int* attribs_offsets = nullptr;

	
	public:
	template<typename Vertex_struct_type>
	AttribsLayout()
		:
		size(Vertex_struct_type.GetComponentsNumber())
		/// unable to use this class, because this method requires special class with struct-like structure
	{
		// allocate mem for layouts arrays
		this->attribs_layouts = new unsigned int[this->size];
		this->floats_per_attribs = new unsigned int[this->size];
		this->attribs_offsets = new unsigned int[this->size];

		// examine Vertex_struct_type data components sizes
		unsigned int current_offset = 0;
		for (unsigned int i = 0; i < this->size; i++)
		{
			this->attribs_layouts[i] = i;
			this->floats_per_attribs[i] = Vertex_struct_type::GetComponentSize(i);	// 2 if glm::vec2, 3 if glm::vec3 etc.
			this->attribs_offsets[i] = current_offset;

			current_offset += this->floats_per_attribs[i];
		}
	}

	AttribsLayout(const AttribsLayout& other)
		:
		size(other.GetSize())
	{
		// copy arrays data
		other.GetAttribsLayouts(this->attribs_layouts, this->size);
		other.GetFloatsPerAttribs(this->floats_per_attribs, this->size);
		other.GetAttribsOffsets(this->attribs_offsets, this->size);
	}

	~AttribsLayout()
	{
		this->size = 0;
		if (this->attribs_layouts)		{	delete[] this->attribs_layouts;		}	this->attribs_layouts = nullptr;
		if (this->floats_per_attribs)	{	delete[] this->floats_per_attribs;	}	this->floats_per_attribs = nullptr;
		if (this->attribs_offsets)		{	delete[] this->attribs_offsets;		}	this->attribs_offsets = nullptr;
	}

	#pragma region getters
	unsigned int GetSize() const
	{
		return this->size;
	}
	void GetAttribsLayouts(unsigned int* dest, const unsigned int dest_size) const
	{
		if (dest_size != this->size)
		{
			throw std::exception(std::string("This AttribsLayout's array size value must be equal to destination_size value.").c_str());
		}
		aux::CopyData<unsigned int>(this->attribs_layouts, dest, dest_size);
	}
	void GetFloatsPerAttribs(unsigned int* dest, const unsigned int dest_size) const
	{
		if (dest_size != this->size)
		{
			throw std::exception(std::string("This FloatsPerAttribs's array size value must be equal to destination_size value.").c_str());
		}
		aux::CopyData<unsigned int>(this->floats_per_attribs, dest, dest_size);
	}
	void GetAttribsOffsets(unsigned int* dest, const unsigned int dest_size) const
	{
		if (dest_size != this->size)
		{
			throw std::exception(std::string("This AttribsOffsets's array size value must be equal to destination_size value.").c_str());
		}
		aux::CopyData<unsigned int>(this->offsets, dest, dest_size);
	}
	#pragma endregion

};
*/

class AttribsInfo // class-replacement until AttribsInfo (original) can not be used
{
	public:
	unsigned int arrays_size = 0;
	unsigned int* layouts = nullptr;
	unsigned int* floats_per_attribs = nullptr;
	unsigned int* offsets = nullptr;



	public:
	AttribsInfo(unsigned int* attribs_layouts_arr, unsigned int* floats_per_attribs_arr, unsigned int* attribs_offsets_arr, unsigned int attribs_info_arrays_size)
		:
		arrays_size(attribs_info_arrays_size)
	{
		// allocate mem for layouts arrays
		aux::CopyData(attribs_layouts_arr, this->layouts, this->arrays_size);
		aux::CopyData(floats_per_attribs_arr, this->floats_per_attribs, this->arrays_size);
		aux::CopyData(attribs_offsets_arr, this->offsets, this->arrays_size);
	}

	AttribsInfo(const AttribsInfo& other)
		:
		arrays_size(other.GetSize())
	{
		// copy arrays data
		aux::CopyData(other.layouts, this->layouts, this->arrays_size);
		aux::CopyData(other.floats_per_attribs, this->floats_per_attribs, this->arrays_size);
		aux::CopyData(other.offsets, this->offsets, this->arrays_size);
	}

	~AttribsInfo()
	{
		this->arrays_size = 0;
		if (this->layouts)				{ delete[] this->layouts;			}	this->layouts = nullptr;
		if (this->floats_per_attribs)	{ delete[] this->floats_per_attribs;}	this->floats_per_attribs = nullptr;
		if (this->offsets)				{ delete[] this->offsets;			}	this->offsets = nullptr;
	}

	#pragma region getters
	unsigned int GetSize() const
	{
		return this->arrays_size;
	}
	void GetAttribsLayouts(unsigned int* dest, const unsigned int dest_size) const
	{
		if (dest_size != this->arrays_size)
		{
			throw std::exception(std::string("This AttribsLayout's array size value must be equal to destination_size value.").c_str());
		}
		aux::CopyData<unsigned int>(this->layouts, dest, dest_size);
	}
	void GetFloatsPerAttribs(unsigned int* dest, const unsigned int dest_size) const
	{
		if (dest_size != this->arrays_size)
		{
			throw std::exception(std::string("This FloatsPerAttribs's array size value must be equal to destination_size value.").c_str());
		}
		aux::CopyData<unsigned int>(this->floats_per_attribs, dest, dest_size);
	}
	void GetAttribsOffsets(unsigned int* dest, const unsigned int dest_size) const
	{
		if (dest_size != this->arrays_size)
		{
			throw std::exception(std::string("This AttribsOffsets's array size value must be equal to destination_size value.").c_str());
		}
		aux::CopyData<unsigned int>(this->offsets, dest, dest_size);
	}
	#pragma endregion


};



// 1. Implement draw()
// 2. Test
// 3. When tested successfully, put AttribsInfo into a separate .h file

// 3.
// implement calculation of 
//		attribs layouts, 
//		floats_per_attribs and 
//		attribs_offsets	
//	from std::vector<Vertex> vertices
// (implemented in AttribsLayout class)

class Mesh
{
	protected:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture2D*> textures;

	VertexArray* va;



	private:

	template<typename Vertex_struct_type>
	void LinkVerticesAttributes
	(
		VertexBuffer* vb, 
		unsigned int* layouts_arr, 
		unsigned int* floats_per_attribs_arr, 
		unsigned int* offsets_arr, 
		unsigned int attribs_arrays_size
	)
	{
		// the truth is that 
		// 1: (sum(components_sizes_in_floats)) has to be = (floats_per_vertex)
		// 1 - checking one_vertex_size_input correctness
		unsigned int input_based_floats_per_vertex = aux::Sum(floats_per_attribs_arr, attribs_arrays_size);
		if (input_based_floats_per_vertex != sizeof(Vertex_struct_type))
		{
			std::exception
			(
				"floats_per_attribs_arr data specified incorrectly. sum(floats_per_attribs_arr) has to be equal to floats_per_vertex. \n"
				"floats_per_vertex must have been specified in the OpenGLobj constructor. Look at function calls stack trace in your IDE for details."
			);
		}
		/// layouts and offsets are not checked - more complex checking !!! (check this statement twice before doing something)
		// specify all the vertex attributes with data checked
		for (unsigned int i = 0; i < attribs_arrays_size; i++)
		{
			vb->LinkVertexAttrib
			(
				layouts_arr[i],
				floats_per_attribs_arr[i],
				GL_FLOAT,
				sizeof(Vertex_struct_type) * sizeof(float),
				(void*)(offsets_arr[i] * sizeof(float))
			);
		}
	}

	public:
	/*
		@warning
		not tested
	*/
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture2DData> textures_data, AttribsInfo* attribs_info)
	{
		this->vertices = vertices;
		this->indices = indices;
		unsigned int textures_data_size = textures_data.size();
		for (unsigned int i = 0; i < textures_data_size; i++)
		{
			this->textures.push_back(new Texture2D(&textures_data[i]));
		}

		IndexBuffer ib(indices);
		// vertex array object
		VertexArray va;
		va.Bind();
		// vertex buffer object and index buffer object
		VertexBuffer vb(vertices);
		//vb.Bind();
		ib.Bind();

		// paused here
		LinkVerticesAttributes<Vertex>
		(
			&vb,
			attribs_info->layouts,
			attribs_info->floats_per_attribs,
			attribs_info->offsets,
			attribs_info->arrays_size
		);

		vb.Unbind();
		va.Unbind();
		ib.Unbind();
	}

	~Mesh()
	{
		if (this->va) { delete this->va; }	this->va = nullptr;
	}

	void Draw(ShaderProgram& shader_program, Camera& camera)
	{
		shader_program.UseProgram();
		// pass shader uniforms values
		/// this will differ regarding to different shaders compiled into shader program (make different mesh classes (MeshColored, MeshIlluminated etc.))

		this->va->Bind();
		// bind textures (similar to as shpwn in the tutorial)
		/// this will be or not be inside some classes derived from a Mesh class. (MeshTextured only).

		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	}

};