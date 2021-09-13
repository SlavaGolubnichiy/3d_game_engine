#pragma once

#include "util/aux_.h"





/*
	@description
	Object for storing data, required to build an opengl object.
*/
class OpenglObj3DData
{
	public:

	// vertices data
	unsigned int	number_of_vertices = 0;
	unsigned int	floats_per_vertex = 0;
	unsigned int	vertices_size = 0;
	float*			vertices;
	unsigned int	indices_size = 0;
	unsigned int*	indices;

	// vertex layouts
	unsigned int attribs_arrays_size = 0;
	unsigned int* attribs_layouts;
	unsigned int* floats_per_attribs;
	unsigned int* attribs_offsets;

	public:

	/**
	*	@params
	*	...
	*	attributes_arrays_size	- size of layouts, floats_per_attribs and offsets arrays (sizes of those arrays have to be equal)
	*							usually, this value is equal to a number of different attributes in a single vertex
	*	layouts					- pointer to an array of locations of attributes in a single vertex (also specified in the begining of vertex shader)
	*							length of layouts array has to be equal to "attribs_array_size" param value.
	*	floats_per_attribs		- pointer to an array of sizes (in floats) of each attribute in a single vertex
	*							length of layouts array has to be equal to "attribs_array_size" param value.
	*	offsets					- pointer to an array of offsets of each attribute in a single vertex (offset of an attrib is a sum of sizes of left-hand-side attributes in a single vertex)
	*/
	OpenglObj3DData
	(
		unsigned int number_of_vertices,
		unsigned int floats_per_vertex,
		float* vertices,
		unsigned int indices_size,
		unsigned int* indices,
		unsigned int attribs_arrays_size,
		unsigned int* attribs_layouts,
		unsigned int* floats_per_attribs,
		unsigned int* attribs_offsets
	)
		:
		number_of_vertices(number_of_vertices),
		floats_per_vertex(floats_per_vertex),
		vertices_size(number_of_vertices * floats_per_vertex),
		indices_size(indices_size),
		attribs_arrays_size(attribs_arrays_size)
	{
		// vertices data
		this->vertices = new float[vertices_size];
		this->indices = new unsigned int[indices_size];
		aux::CopyData<float>(vertices, this->vertices, vertices_size);
		aux::CopyData<unsigned int>(indices, this->indices, indices_size);
		// layouts data
		this->attribs_layouts = new unsigned int[attribs_arrays_size];
		this->floats_per_attribs = new unsigned int[attribs_arrays_size];
		this->attribs_offsets = new unsigned int[attribs_arrays_size];
		aux::CopyData<unsigned int>(attribs_layouts, this->attribs_layouts, attribs_arrays_size);
		aux::CopyData<unsigned int>(floats_per_attribs, this->floats_per_attribs, attribs_arrays_size);
		aux::CopyData<unsigned int>(attribs_offsets, this->attribs_offsets, attribs_arrays_size);
	}

	OpenglObj3DData(const OpenglObj3DData& other)
		:
		number_of_vertices(other.number_of_vertices),
		floats_per_vertex(other.floats_per_vertex),
		vertices_size(other.vertices_size),
		indices_size(other.indices_size),
		attribs_arrays_size(other.attribs_arrays_size)
	{
		// vertices data
		this->vertices = new float[vertices_size];
		this->indices = new unsigned int[indices_size];
		aux::CopyData<float>(other.vertices, this->vertices, vertices_size);
		aux::CopyData<unsigned int>(other.indices, this->indices, indices_size);
		// layouts data
		this->attribs_layouts = new unsigned int[attribs_arrays_size];
		this->floats_per_attribs = new unsigned int[attribs_arrays_size];
		this->attribs_offsets = new unsigned int[attribs_arrays_size];
		aux::CopyData<unsigned int>(other.attribs_layouts, this->attribs_layouts, attribs_arrays_size);
		aux::CopyData<unsigned int>(other.floats_per_attribs, this->floats_per_attribs, attribs_arrays_size);
		aux::CopyData<unsigned int>(other.attribs_offsets, this->attribs_offsets, attribs_arrays_size);
	}

	OpenglObj3DData& operator=(const OpenglObj3DData& other) = delete;

	~OpenglObj3DData()
	{
		if (this->vertices)				{ delete[] this->vertices;				this->vertices = nullptr;			}
		if (this->indices)				{ delete[] this->indices;				this->indices = nullptr;			}
		if (this->attribs_layouts)		{ delete[] this->attribs_layouts;		this->attribs_layouts = nullptr;	}
		if (this->floats_per_attribs)	{ delete[] this->floats_per_attribs;	this->floats_per_attribs = nullptr;	}
		if (this->attribs_offsets)		{ delete[] this->attribs_offsets;		this->attribs_offsets = nullptr;	}
	}

};
