#pragma once

#include <iostream>

#include "util/aux_.h"
#include "OpenglObj3DData.h"

#include "vendor/glm/glm.hpp"

class OpenglObj3DDataGenerator
{
	private:
	
	/// Vertex format description
	//	|_______positions_______|_________colors________|__texture coords_|
	//	|___x___|___y___|___z___|___r___|___g___|___b___|__t(x)__|__s(y)__|
	//	 0	   4 5     7 8    11 12	  15 16   19 20   23 24    27 28    31
	//	 float1  float2
	//	 
	//	0 positions offset = 0
	//	|<--------------------->| colors offset = 3*sizeof(float)
	//	|<--------------------------------------------->| texture_coords offset = (3+3)*sizeof(float)
	//	|<--------------------------------------------------------------->| stride = vertex_size
	// position				- vertex attribute 0
	// color				- vertex attribute 1
	// texture_coordinate	- vertex attribute 2
	
	#pragma region square_data
	
	const static unsigned int square_number_of_vertices = 4;
	const static unsigned int square_floats_per_vertex = 8;
	const static unsigned int square_vertices_size = square_number_of_vertices * square_floats_per_vertex;
	float* square_vertices = new float[square_vertices_size]
	{	// positions			// colors				// texture coords
		-0.5f, 0.0f, 0.0f,		0.8f, 0.30f, 0.2f,		0.0f, 0.0f,		// lower left
		 0.5f, 0.0f, 0.0f,		0.8f, 0.30f, 0.2f,		1.0f, 0.0f,		// lower right
		 0.5f, 1.0f, 0.0f,		1.0f, 0.60f, 0.32f,		1.0f, 1.0f,		// upper right
		-0.5f, 1.0f, 0.0f,		0.9f, 0.45f, 0.17f,		0.0f, 1.0f		// upper left
	};

	const static unsigned int square_indices_size = 6;
	unsigned int* square_indices = new unsigned int[square_indices_size]
	{
		0, 1, 2,	// lower triangle
		0, 2, 3		// upper triangle
	};
	
	#pragma endregion

	#pragma region cube_data
	
	const static unsigned int cube_number_of_vertices = 8;
	const static unsigned int cube_floats_per_vertex = 8;
	const static unsigned int cube_vertices_size = cube_number_of_vertices * cube_floats_per_vertex;
	float* cube_vertices = new float[cube_vertices_size]
	{	// positions			// colors				// texture coords
		0.0f,  0.0f,  0.0f,		1.0f, 0.68f, 0.50f,		0.0f, 0.0f,		// far top right (zyx, when looking at 00z+)
		0.0f, -1.0f,  0.0f,		1.0f, 0.25f, 0.25f,		0.0f, 0.0f,		// far bottom right
		1.0f, -1.0f,  0.0f,		1.0f, 0.68f, 0.50f,		0.0f, 0.0f,		// far bottom left
		1.0f,  0.0f,  0.0f,		1.0f, 0.93f, 0.75f,		0.0f, 0.0f,		// far top left
		0.0f,  0.0f, -1.0f,		1.0f, 0.93f, 0.75f,		0.0f, 0.0f,		// near top right
		0.0f, -1.0f, -1.0f,		1.0f, 0.68f, 0.50f,		0.0f, 0.0f,		// near bottom right
		1.0f, -1.0f, -1.0f,		1.0f, 0.93f, 0.75f,		0.0f, 0.0f,		// near bottom left
		1.0f,  0.0f, -1.0f,		1.0f, 1.00f, 1.00f,		0.0f, 0.0f		// near top left
	};

	const static unsigned int cube_indices_size = 36;
	unsigned int* cube_indices = new unsigned int[cube_indices_size]
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 7,
		0, 7, 4,
		7, 3, 2,
		7, 2, 6,
		4, 7, 6,
		4, 6, 5,
		5, 6, 1,
		1, 6, 2,
		0, 4, 1,
		1, 4, 5
	};
	
	#pragma endregion

	#pragma region triangle_data
	unsigned int triangle_number_of_vertices = 4;
	unsigned int triangle_floats_per_vertex = 8;
	unsigned int triangle_vertices_size = triangle_number_of_vertices * triangle_floats_per_vertex;
	float* triangle_vertices = new float[triangle_vertices_size]
	{	// positions							// colors						// texture coords
		-0.5f,	-0.5f*float(sqrt(3)) / 3,		0.0f,	0.8f, 0.3f, 0.2f,		0.0f, 0.0f,		// lower left
		 0.5f,	-0.5f*float(sqrt(3)) / 3,		0.0f,	0.8f, 0.3f, 0.2f,		1.0f, 0.0f,		// lower right
		 0.0f,	 0.5f*float(sqrt(3)) * 2 / 3,	0.0f,	1.0f, 0.6f, 0.32f,		0.5f, 1.0f,		// upper right
		 0.0f,   0.5f*float(sqrt(3)) * 2 / 3,	0.0f,	0.9f, 0.45f, 0.17f,		1.5f, 1.0f		// upper left
	};
	unsigned int triangle_indices_size = 3;
	unsigned int* triangle_indices = new unsigned int[triangle_indices_size]
	{
		0, 1, 2		// triangle
	};
	#pragma endregion

	#pragma region pyramid_data
	/// pyramid
	const unsigned int pyramid_number_of_vertices = 5;		// rows (number_of_vertices)
	const unsigned int pyramid_floats_per_vertex = 8;		// columns (=sum(floats_per_pos_attrib, floats_per_color_attrib, floats_per_texCoord_attrib))
	unsigned int pyramid_vertices_size = pyramid_number_of_vertices * pyramid_floats_per_vertex;
	float* pyramid_vertices = new float[pyramid_vertices_size]
	{	// positions			// colors				// texture coords
		-0.5f, 0.0f,  0.5f,		0.83f, 0.70f, 0.44f,	 2.0f, -2.0f,		// x-y0z+ bottom vertex
		-0.5f, 0.0f, -0.5f,		0.83f, 0.70f, 0.44f,	-2.0f, -2.0f,		// x-y0z- bottom vertex
		 0.5f, 0.0f, -0.5f,		0.83f, 0.70f, 0.44f,	 2.0f, -2.0f,		// x+y0z- bottom vertex
		 0.5f, 0.0f,  0.5f,		0.83f, 0.70f, 0.44f,	 6.0f, -2.0f,		// x+y0z+ bottom vertex
		 0.0f, 0.8f,  0.0f,		0.92f, 0.86f, 0.76f,	 2.0f,  2.0f		// x0y+z0 top vertex
	};
	const unsigned int pyramid_indices_size = 18;
	unsigned int* pyramid_indices = new unsigned int[pyramid_indices_size]
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};
	#pragma endregion





	/// vertex attributes layouts
	#pragma region vertex_with_Pos_Color_Tex
	const static unsigned int PosColorTex_attribs_arrays_size = 3;
							// vertex attribs ¹															//0 //1 //2 ----------
	unsigned int* PosColorTex_attribs_layouts = new unsigned int[PosColorTex_attribs_arrays_size]		{ 0,  1,  2 };
	unsigned int* PosColorTex_floats_per_attribs = new unsigned int[PosColorTex_attribs_arrays_size]	{ 3,  3,  2 };
	unsigned int* PosColorTex_attribs_offsets = new unsigned int[PosColorTex_attribs_arrays_size]		{ 0,  3,  6 };
	// 0 - vertex position
	// 1 - vertex color
	// 2 - texture coordinate
	#pragma endregion

	#pragma region vertex_with_Pos_Color_Tex_Normal
	const static unsigned int PosColorTexNormal_attribs_arrays_size = 4;
							// vertex attribs ¹																		//0 //1 //2 //4----------
	unsigned int* PosColorTexNormal_attribs_layouts = new unsigned int[PosColorTexNormal_attribs_arrays_size]		{ 0,  1,  2,  3 };
	unsigned int* PosColorTexNormal_floats_per_attribs = new unsigned int[PosColorTexNormal_attribs_arrays_size]	{ 3,  3,  2,  3 };
	unsigned int* PosColorTexNormal_attribs_offsets = new unsigned int[PosColorTexNormal_attribs_arrays_size]		{ 0,  3,  6,  8 };
	// 0 - vertex position
	// 1 - vertex color
	// 2 - texture coordinate
	// 3 - normal vector
	#pragma endregion





	public:
	
	~OpenglObj3DDataGenerator()
	{
		if (square_vertices)	{ delete[] this->square_vertices;	this->square_vertices = nullptr;	}
		if (square_indices)		{ delete[] this->square_indices;	this->square_indices = nullptr;		}
		if (cube_vertices)		{ delete[] this->cube_vertices;		this->cube_vertices = nullptr;		}
		if (cube_indices)		{ delete[] this->cube_indices;		this->cube_indices = nullptr;		}
		if (triangle_vertices)	{ delete[] this->triangle_vertices;	this->triangle_vertices = nullptr;	}
		if (triangle_indices)	{ delete[] this->triangle_indices;	this->triangle_indices = nullptr;	}
		if (pyramid_vertices)	{ delete[] this->pyramid_vertices;	this->pyramid_vertices = nullptr;	}
		if (pyramid_indices)	{ delete[] this->pyramid_indices;	this->pyramid_indices = nullptr;	}
	}

	/**
	*	@params
	*	top_left_pos - position of a vertex, closest to the x-y- direction
	*
	*/
	OpenglObj3DData GetRect3d(glm::vec2 sizes) const
	{
		if (sizes.x <= 0.0f || sizes.y <= 0.0f)
		{
			throw std::exception
			(
				std::string("Object's size.x and size.y must be a positive float values, in range (0.0f; float_max_value].").c_str()
			);
		}

		const unsigned int number_of_vertices = square_number_of_vertices;
		const unsigned int floats_per_vertex = square_floats_per_vertex;
		unsigned int vertices_size = number_of_vertices * floats_per_vertex;
		// object-related coordinates (considering (0.0f; 0.0f; 0.0f) - object's origin)
		float* vertices = new float[vertices_size]
		{	// positions				// colors				// texture coords
			0.0f,	 0.0f,		0.0f,	0.8f, 0.30f, 0.2f,		0.0f, 0.0f,		// upper left
			sizes.x, 0.0f,		0.0f,	0.8f, 0.30f, 0.2f,		1.0f, 0.0f,		// upper right
			sizes.x, sizes.y,	0.0f,	1.0f, 0.60f, 0.32f,		1.0f, 1.0f,		// lower right
			0.0f,	 sizes.y,	0.0f,	0.9f, 0.45f, 0.17f,		0.0f, 1.0f		// lower left
		};
		
		return OpenglObj3DData
		(
			number_of_vertices,
			floats_per_vertex,
			vertices,
			square_indices_size,
			square_indices,
			PosColorTex_attribs_arrays_size,
			PosColorTex_attribs_layouts,
			PosColorTex_floats_per_attribs,
			PosColorTex_attribs_offsets
		);
	}

	/**
	*	@params
	*	origin_closest_pos - position of a vertex, closest to the x-y-z- direction
	*	sizes - vector, strores sizes of x-alongside (size.x), y-alongside and z-alongside edges of an object 
	*/
	OpenglObj3DData GetCuboid3d(glm::vec3 sizes) const
	{
		if (sizes.x <= 0.0f || sizes.y <= 0.0f || sizes.z <= 0.0f)
		{
			throw std::exception
			(
				std::string("Object's size.x and size.y must be a positive float values, in range (0.0f; float_max_value].").c_str()
			);
		}

		const unsigned int number_of_vertices = cube_number_of_vertices;
		const unsigned int floats_per_vertex = cube_floats_per_vertex;
		unsigned int vertices_size = number_of_vertices * floats_per_vertex;
		float* vertices = new float[vertices_size]
		{	// positions					// colors				// texture coords
			0.0f,	 0.0f,		0.0f,		1.0f, 0.68f, 0.50f,		0.0f, 0.0f,		// far top right (zyx, when looking at 00z+)
			0.0f,	 sizes.y,	0.0f,		1.0f, 0.25f, 0.25f,		0.0f, 0.0f,		// far bottom right
			sizes.x, sizes.y,	0.0f,		1.0f, 0.68f, 0.50f,		0.0f, 0.0f,		// far bottom left
			sizes.x, 0.0f,		0.0f,		1.0f, 0.93f, 0.75f,		0.0f, 0.0f,		// far top left
			0.0f,	 0.0f,		sizes.z,	1.0f, 0.93f, 0.75f,		0.0f, 0.0f,		// near top right
			0.0f,	 sizes.y,	sizes.z,	1.0f, 0.68f, 0.50f,		0.0f, 0.0f,		// near bottom right
			sizes.x, sizes.y,	sizes.z,	1.0f, 0.93f, 0.75f,		0.0f, 0.0f,		// near bottom left
			sizes.x, 0.0f,		sizes.z,	1.0f, 1.00f, 1.00f,		0.0f, 0.0f		// near top left
		};

		return OpenglObj3DData
		(
			number_of_vertices,
			floats_per_vertex,
			vertices,
			cube_indices_size,
			cube_indices,
			PosColorTex_attribs_arrays_size,
			PosColorTex_attribs_layouts,
			PosColorTex_floats_per_attribs,
			PosColorTex_attribs_offsets
		);
	}

	OpenglObj3DData GetCuboid3d(glm::vec3 sizes, glm::vec3 solid_color) const
	{
		if (sizes.x <= 0.0f || sizes.y <= 0.0f || sizes.z <= 0.0f)
		{
			throw std::exception
			(
				std::string("Object's size.x and size.y must be a positive float values, in range (0.0f; float_max_value].").c_str()
			);
		}

		const unsigned int number_of_vertices = cube_number_of_vertices;
		const unsigned int floats_per_vertex = cube_floats_per_vertex;
		unsigned int vertices_size = number_of_vertices * floats_per_vertex;
		float* vertices = new float[vertices_size]
		{	// positions					// colors										// texture coords
			0.0f,	 0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		// far top right (zyx, when looking at 00z+)
			0.0f,	 sizes.y,	0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		// far bottom right
			sizes.x, sizes.y,	0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		// far bottom left
			sizes.x, 0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		// far top left
			0.0f,	 0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		// near top right
			0.0f,	 sizes.y,	sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		// near bottom right
			sizes.x, sizes.y,	sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		// near bottom left
			sizes.x, 0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f		// near top left
		};

		return OpenglObj3DData
		(
			number_of_vertices,
			floats_per_vertex,
			vertices,
			cube_indices_size,
			cube_indices,
			PosColorTex_attribs_arrays_size,
			PosColorTex_attribs_layouts,
			PosColorTex_floats_per_attribs,
			PosColorTex_attribs_offsets
		);
	}

	OpenglObj3DData GetCuboid3dWithNormals(glm::vec3 sizes, glm::vec3 solid_color) const
	{
		if (sizes.x <= 0.0f || sizes.y <= 0.0f || sizes.z <= 0.0f)
		{
			throw std::exception
			(
				std::string("Object's size.x and size.y must be a positive float values, in range (0.0f; float_max_value].").c_str()
			);
		}
		
		/// Cube vertices format image
		//
		//
		//										         3
		//		y								  	    /
		//								   [5]    5	   v	   [6]
		//		^							*_____|__________*
		//		|			  z  ^		   /.	  |			/|
		//		|				/		  /	.	  v		   / |
		//		|			   /	 [1] /	.		  [2] /  |
		//		|			  /			*_______________*/	 |		<- 4
		//		|			 /			|	.			|	 |
		//		|		    /	2 ->	| 	. [4]		|	 | [7]
		//		|		   /		 	|	* . . . . . | . .*
		//		|		  /				|  .			|   /
		//		|		 /				| .				|  /
		//		|	    /				|.	   	^		| /
		//		|	   /				|______/________|/
		//		|	  /					*	  /			 *
		//		| 	 /				 [0]	 1		  [3]
		//		|   /							^
		//		|  /							|
		//		| /							  side 6
		//		|/___________________________________________________________________>	x
		//	  O *
		//
		//
		//	***
		//	each side is specified in "indices" array starting from side-relative 
		//	left-bottom vertex position in a clock-wise order
		//	***
		//
		//	however, further should start from model's bottom side! (side 6 has to be 1st)
		//
		const unsigned int number_of_vertices = 24;
		const unsigned int floats_per_vertex = 11;
		unsigned int vertices_size = number_of_vertices * floats_per_vertex;
		float* vertices = new float[vertices_size]
		{	// vertex-i // positions					// colors										// tex coords	// normals			/// @ description					// image-i
						// side 1 = (0,1,2,3)
			/* 0 */		0.0f,	 0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// XOY-parallel side looking at Z-	[0] - base
			/* 1 */		0.0f,	 sizes.y,	0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// XOY-parallel side looking at Z-	[1]	- base
			/* 2 */		sizes.x, sizes.y,	0.0f,		solid_color.r, solid_color.g, solid_color.b,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// XOY-parallel side looking at Z-	[2]	- base
			/* 3 */		sizes.x, 0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// XOY-parallel side looking at Z-	[3]	- base
			
						// side 2 = (4,5,0,1)
			/* 4 */		0.0f,	 0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// YOZ-parallel side looking at X-	[4]
			/* 5 */		0.0f,	 sizes.y,	sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// YOZ-parallel side looking at X-	[5]
			/* 6 */		0.0f,	 sizes.y,	0.0f,		solid_color.r, solid_color.g, solid_color.b,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// YOZ-parallel side looking at X-	[1]
			/* 7 */		0.0f,	 0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// YOZ-parallel side looking at X-	[0]
			
						// side 3 = (7,6,5,4)
			/* 8 */		sizes.x, 0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// XOY-parallel side looking at Z+	[7] - base 
			/* 9 */		sizes.x, sizes.y,	sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// XOY-parallel side looking at Z+	[6]	- base
			/* 10 */	0.0f,	 sizes.y,	sizes.z,	solid_color.r, solid_color.g, solid_color.b,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// XOY-parallel side looking at Z+	[5]	- base
			/* 11 */	0.0f,	 0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// XOY-parallel side looking at Z+	[4] - base

						// side 4 = (3,2,6,7)
			/* 12 */	sizes.x, 0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// YOZ-parallel side looking at X+	[3]
			/* 13 */	sizes.x, sizes.y,	0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// YOZ-parallel side looking at X+	[2]
			/* 14 */	sizes.x, sizes.y,	sizes.z,	solid_color.r, solid_color.g, solid_color.b,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// YOZ-parallel side looking at X+	[6]
			/* 15 */	sizes.x, 0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// YOZ-parallel side looking at X+	[7]

						// side 5 = (1,5,6,2)
			/* 16 */	0.0f,	 sizes.y,	0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y+	[1]
			/* 17 */	0.0f,	 sizes.y,	sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y+	[5]
			/* 18 */	sizes.x, sizes.y,	sizes.z,	solid_color.r, solid_color.g, solid_color.b,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y+	[6]
			/* 19 */	sizes.x, sizes.y,	0.0f,		solid_color.r, solid_color.g, solid_color.b,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y+	[2]
			
						// side 6 = (3,7,4,0)
			/* 20 */	sizes.x, 0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y-	[3]
			/* 21 */	sizes.x, 0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y-	[7]
			/* 22 */	0.0f,	 0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y-	[4]
			/* 23 */	0.0f,	 0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y-	[0]
		};
		const static unsigned int indices_size = 36;
		unsigned int* indices = new unsigned int[indices_size]	// uses vertex-i (indexes)
		{
			// tri1			// tri2
			 0,  1,  2,		 0,  2,  3,		// side1
			 4,  5,  6,		 4,  6,  7,		// side2
			 8,  9, 10,		 8, 10, 11,		// side3
			12, 13, 14,		12, 14, 15,		// side4
			16, 17, 18,		16, 18, 19,		// side5
			20, 21, 22,		20, 22, 23		// side6
		};

		/// Normal vectors calculation and writing them back into vertices
		// BE CAREFUL ! next calculation of normals depends on each vertex position location in vertices array
		// we use vertices[i] that are marked as "base" (they are unique for our cube)
		glm::vec3 vertex0_pos = glm::vec3(vertices[0], vertices[1], vertices[2]);
		glm::vec3 vertex1_pos = glm::vec3(vertices[floats_per_vertex* 1+0], vertices[floats_per_vertex* 1+1], vertices[floats_per_vertex* 1+2]);
		glm::vec3 vertex2_pos = glm::vec3(vertices[floats_per_vertex* 2+0], vertices[floats_per_vertex* 2+1], vertices[floats_per_vertex* 2+2]);
		glm::vec3 vertex3_pos = glm::vec3(vertices[floats_per_vertex* 3+0], vertices[floats_per_vertex* 3+1], vertices[floats_per_vertex* 3+2]);
		glm::vec3 vertex4_pos = glm::vec3(vertices[floats_per_vertex*11+0], vertices[floats_per_vertex*11+1], vertices[floats_per_vertex*11+2]);
		glm::vec3 vertex5_pos = glm::vec3(vertices[floats_per_vertex*10+0], vertices[floats_per_vertex*10+1], vertices[floats_per_vertex*10+2]);
		glm::vec3 vertex6_pos = glm::vec3(vertices[floats_per_vertex* 9+0], vertices[floats_per_vertex* 9+1], vertices[floats_per_vertex* 9+2]);
		glm::vec3 vertex7_pos = glm::vec3(vertices[floats_per_vertex* 8+0], vertices[floats_per_vertex* 8+1], vertices[floats_per_vertex* 8+2]);
		
		glm::vec3 vector01 = aux::GetVector(vertex0_pos, vertex1_pos);
		glm::vec3 vector03 = aux::GetVector(vertex0_pos, vertex3_pos);
		glm::vec3 vector45 = aux::GetVector(vertex4_pos, vertex5_pos);
		glm::vec3 vector40 = aux::GetVector(vertex4_pos, vertex0_pos);
		glm::vec3 vector76 = aux::GetVector(vertex7_pos, vertex6_pos);
		glm::vec3 vector74 = aux::GetVector(vertex7_pos, vertex4_pos);
		glm::vec3 vector32 = aux::GetVector(vertex3_pos, vertex2_pos);
		glm::vec3 vector37 = aux::GetVector(vertex3_pos, vertex7_pos);
		glm::vec3 vector15 = aux::GetVector(vertex1_pos, vertex5_pos);
		glm::vec3 vector12 = aux::GetVector(vertex1_pos, vertex2_pos);
		// glm::vec3 vector37 = aux::GetVector(vertex3_pos, vertex7_pos);	// duplicate
		glm::vec3 vector30 = aux::GetVector(vertex3_pos, vertex0_pos);

		glm::vec3 normal_0123 = glm::normalize(glm::cross(vector01, vector03));		// normal of side1
		glm::vec3 normal_4510 = glm::normalize(glm::cross(vector45, vector40));		// normal of side2
		glm::vec3 normal_7654 = glm::normalize(glm::cross(vector76, vector74));		// normal of side3
		glm::vec3 normal_3267 = glm::normalize(glm::cross(vector32, vector37));		// normal of side4
		glm::vec3 normal_1562 = glm::normalize(glm::cross(vector15, vector12));		// normal of side5
		glm::vec3 normal_3740 = glm::normalize(glm::cross(vector37, vector30));		// normal of side6

		unsigned int k = floats_per_vertex;
		// side 1 - push normals to vertices
		vertices[8]			= normal_0123.x;	vertices[9]			= normal_0123.y;	vertices[10]		 = normal_0123.z;
		vertices[k * 1 + 8]	= normal_0123.x;	vertices[k * 1 + 9]	= normal_0123.y;	vertices[k * 1 + 10] = normal_0123.z;
		vertices[k * 2 + 8]	= normal_0123.x;	vertices[k * 2 + 9]	= normal_0123.y;	vertices[k * 2 + 10] = normal_0123.z;
		vertices[k * 3 + 8]	= normal_0123.x;	vertices[k * 3 + 9]	= normal_0123.y;	vertices[k * 3 + 10] = normal_0123.z;
		// side 2 - push normals to vertices
		vertices[k * 4 + 8] = normal_4510.x;	vertices[k * 4 + 9] = normal_4510.y;	vertices[k * 4 + 10] = normal_4510.z;
		vertices[k * 5 + 8] = normal_4510.x;	vertices[k * 5 + 9] = normal_4510.y;	vertices[k * 5 + 10] = normal_4510.z;
		vertices[k * 6 + 8] = normal_4510.x;	vertices[k * 6 + 9] = normal_4510.y;	vertices[k * 6 + 10] = normal_4510.z;
		vertices[k * 7 + 8] = normal_4510.x;	vertices[k * 7 + 9] = normal_4510.y;	vertices[k * 7 + 10] = normal_4510.z;
		// side 3 - push normals to vertices
		vertices[k * 8 + 8] = normal_7654.x;	vertices[k * 8 + 9] = normal_7654.y;	vertices[k * 8 + 10] = normal_7654.z;
		vertices[k * 9 + 8] = normal_7654.x;	vertices[k * 9 + 9] = normal_7654.y;	vertices[k * 9 + 10] = normal_7654.z;
		vertices[k * 10 + 8] = normal_7654.x;	vertices[k * 10 + 9] = normal_7654.y;	vertices[k * 10 + 10] = normal_7654.z;
		vertices[k * 11 + 8] = normal_7654.x;	vertices[k * 11 + 9] = normal_7654.y;	vertices[k * 11 + 10] = normal_7654.z;
		// side 4 - push normals to vertices
		vertices[k * 12 + 8] = normal_3267.x;	vertices[k * 12 + 9] = normal_3267.y;	vertices[k * 12 + 10] = normal_3267.z;
		vertices[k * 13 + 8] = normal_3267.x;	vertices[k * 13 + 9] = normal_3267.y;	vertices[k * 13 + 10] = normal_3267.z;
		vertices[k * 14 + 8] = normal_3267.x;	vertices[k * 14 + 9] = normal_3267.y;	vertices[k * 14 + 10] = normal_3267.z;
		vertices[k * 15 + 8] = normal_3267.x;	vertices[k * 15 + 9] = normal_3267.y;	vertices[k * 15 + 10] = normal_3267.z;
		// side 5 - push normals to vertices
		vertices[k * 16 + 8] = normal_1562.x;	vertices[k * 16 + 9] = normal_1562.y;	vertices[k * 16 + 10] = normal_1562.z;
		vertices[k * 17 + 8] = normal_1562.x;	vertices[k * 17 + 9] = normal_1562.y;	vertices[k * 17 + 10] = normal_1562.z;
		vertices[k * 18 + 8] = normal_1562.x;	vertices[k * 18 + 9] = normal_1562.y;	vertices[k * 18 + 10] = normal_1562.z;
		vertices[k * 19 + 8] = normal_1562.x;	vertices[k * 19 + 9] = normal_1562.y;	vertices[k * 19 + 10] = normal_1562.z;
		// side 6 - push normals to vertices
		vertices[k * 20 + 8] = normal_3740.x;	vertices[k * 20 + 9] = normal_3740.y;	vertices[k * 20 + 10] = normal_3740.z;
		vertices[k * 21 + 8] = normal_3740.x;	vertices[k * 21 + 9] = normal_3740.y;	vertices[k * 21 + 10] = normal_3740.z;
		vertices[k * 22 + 8] = normal_3740.x;	vertices[k * 22 + 9] = normal_3740.y;	vertices[k * 22 + 10] = normal_3740.z;
		vertices[k * 23 + 8] = normal_3740.x;	vertices[k * 23 + 9] = normal_3740.y;	vertices[k * 23 + 10] = normal_3740.z;

		//aux::PrintArray(vertices, vertices_size, floats_per_vertex);
		
		return OpenglObj3DData
		(
			number_of_vertices,
			floats_per_vertex,
			vertices,
			indices_size,
			indices,
			PosColorTexNormal_attribs_arrays_size,
			PosColorTexNormal_attribs_layouts,
			PosColorTexNormal_floats_per_attribs,
			PosColorTexNormal_attribs_offsets
		);
	}

	OpenglObj3DData GetTriangle3d() const
	{
		return OpenglObj3DData
		(
			triangle_number_of_vertices,
			triangle_floats_per_vertex,
			triangle_vertices,
			triangle_indices_size,
			triangle_indices,
			PosColorTex_attribs_arrays_size,
			PosColorTex_attribs_layouts,
			PosColorTex_floats_per_attribs,
			PosColorTex_attribs_offsets
		);
	}

	OpenglObj3DData GetPyramid3d() const
	{
		return OpenglObj3DData
		(
			pyramid_number_of_vertices,
			pyramid_floats_per_vertex,
			pyramid_vertices,
			pyramid_indices_size,
			pyramid_indices,
			PosColorTex_attribs_arrays_size,
			PosColorTex_attribs_layouts,
			PosColorTex_floats_per_attribs,
			PosColorTex_attribs_offsets
		);
	}

	OpenglObj3DData GetPyramid3dWithNormals(glm::vec2 bottom_rect_sizes, float pyramid_height, glm::vec3 solid_color) const
	{
		glm::vec3 sizes(bottom_rect_sizes[0], pyramid_height, bottom_rect_sizes[1]);

		if (sizes.x <= 0.0f || sizes.y <= 0.0f || sizes.z <= 0.0f)
		{
			throw std::exception
			(
				std::string("Object's size.x, size.y and size.z must be positive float values, in range (0.0f; float_max_value].").c_str()
			);
		}
		
		/// Pyramid vertices format image
		/*

			y

			^
			|
			|														  4
			|													  __/
			|													 /
			|								  [4]				v
			|								     _/\___
			|					 			   _/.	\  \___
			|				    			 _/ .	 \     \___
			|				   			   _/  .	  \		   \___				<-- 5
			|				  		     _/	  .		   \		   \___
			|		3 -->	 		   _/	 .			\			   \___
			|						 _/		.			 \				   \___
			|			   		   _/	   .  [3]		  \					   \___	 [2]
			|			  		 _/		  . . . . . . . . .\. . . . . . . . . . . .\__
			|			 	   _/	   .					\				  .	   __/
			|		    	 _/	   .		   				 \      .			__/
			|		   	   _/   .					    .	  \				 __/
			|		  	 _/  . 				   .		 	   \		  __/
			|		   _/ . 		  .						 	\	   __/
			|		 _/ .	.									 \	__/
			|	   	/.____________________________________________\/
			|	[0]													[1]
			|
			|				^				^
			|			 __/				|
			|			/
			|		  2					  side 1
			|
			|
			|
			|		z  ^
			|		__/
			|    __/
			| __/
			|/_______________________________________________________________________>	x
			  O *
		
			***
			Each side is specified in "indices" array starting from side-relative 
			left-bottom vertex position in a clock-wise order.

			Only first (bottom) side is meant to be specified starting from side-relative RIGHT-bottom vertex position.
			***

		*/
		const unsigned int number_of_vertices = 16;
		const unsigned int floats_per_vertex = 11;
		unsigned int vertices_size = number_of_vertices * floats_per_vertex;
		float* vertices = new float[vertices_size]
		{	// vertex-i // positions						// colors										// tex coords	// normals			/// @ description					// image-i
						// side 1 = (0,1,2,3)
			/* 0 */		0.0f,		0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y-	[0] - base
			/* 1 */		sizes.x,	0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 2.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y-	[1]	- base
			/* 2 */		sizes.x,	0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	2.0f, 2.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y-	[2]	- base
			/* 3 */		0.0f,		0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	2.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// XOZ-parallel side looking at Y-	[3]	- base

						// side 2 = (0,4,1)
			/* 4 */		0.0f,		0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	2.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// [0]
			/* 5 */		sizes.x/2,	sizes.y,	sizes.z/2,	solid_color.r, solid_color.g, solid_color.b,	1.0f, 2.0f,		0.0f, 0.0f, 0.0f,	// [4] - base (top vertex pos)
			/* 6 */		sizes.x,	0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// [1]
			
						// side 3 = (3,4,0)
			/* 7 */		0.0f,		0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	2.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// [3]
			/* 8 */		sizes.x/2,	sizes.y,	sizes.z/2,	solid_color.r, solid_color.g, solid_color.b,	1.0f, 2.0f,		0.0f, 0.0f, 0.0f,	// [4]
			/* 9 */		0.0f,		0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// [0]

						// side 4 = (2,4,3)
			/* 10 */	sizes.x,	0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	2.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// [2]
			/* 11 */	sizes.x/2,	sizes.y,	sizes.z/2,	solid_color.r, solid_color.g, solid_color.b,	1.0f, 2.0f,		0.0f, 0.0f, 0.0f,	// [4]
			/* 12 */	0.0f,		0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// [3]

						// side 5 = (1,4,2)
			/* 13 */	sizes.x,	0.0f,		0.0f,		solid_color.r, solid_color.g, solid_color.b,	2.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// [1]
			/* 14 */	sizes.x/2,	sizes.y,	sizes.z/2,	solid_color.r, solid_color.g, solid_color.b,	1.0f, 2.0f,		0.0f, 0.0f, 0.0f,	// [4]
			/* 15 */	sizes.x,	0.0f,		sizes.z,	solid_color.r, solid_color.g, solid_color.b,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	// [2]
		};
		const static unsigned int indices_size = 18;
		unsigned int* indices = new unsigned int[indices_size]	// uses vertex-i (indexes)
		{
			// tri1			// tri2
			 0,  1,  2,		 0, 2, 3,	// side 1
			 4,  5,  6,					// side 2
			 7,  8,  9,					// side 3
			10, 11, 12,					// side 4
			13, 14, 15,					// side 5
		};

		/// Normal vectors calculation and writing them back into vertices
		// BE CAREFUL ! next calculation of normals depends on each vertex position location in vertices array
		// we use vertices[i] that are marked as "base" (they are unique for our cube)
		glm::vec3 vertex0_pos = glm::vec3(vertices[0], vertices[1], vertices[2]);
		glm::vec3 vertex1_pos = glm::vec3(vertices[floats_per_vertex*1+0], vertices[floats_per_vertex*1+1], vertices[floats_per_vertex*1+2]);
		glm::vec3 vertex2_pos = glm::vec3(vertices[floats_per_vertex*2+0], vertices[floats_per_vertex*2+1], vertices[floats_per_vertex*2+2]);
		glm::vec3 vertex3_pos = glm::vec3(vertices[floats_per_vertex*3+0], vertices[floats_per_vertex*3+1], vertices[floats_per_vertex*3+2]);
		glm::vec3 vertex4_pos = glm::vec3(vertices[floats_per_vertex*5+0], vertices[floats_per_vertex*5+1], vertices[floats_per_vertex*5+2]);
		
		glm::vec3 vector10 = aux::GetVector(vertex1_pos, vertex0_pos);
		glm::vec3 vector12 = aux::GetVector(vertex1_pos, vertex2_pos);
		glm::vec3 vector04 = aux::GetVector(vertex0_pos, vertex4_pos);
		glm::vec3 vector01 = aux::GetVector(vertex0_pos, vertex1_pos);
		glm::vec3 vector34 = aux::GetVector(vertex3_pos, vertex4_pos);
		glm::vec3 vector30 = aux::GetVector(vertex3_pos, vertex0_pos);
		glm::vec3 vector24 = aux::GetVector(vertex2_pos, vertex4_pos);
		glm::vec3 vector23 = aux::GetVector(vertex2_pos, vertex3_pos);
		glm::vec3 vector14 = aux::GetVector(vertex1_pos, vertex4_pos);
		//glm::vec3 vector12 = aux::GetVector(vertex1_pos, vertex2_pos);	// duplicate

		glm::vec3 normal_0123 = glm::normalize(glm::cross(vector12, vector10));		// normal of side1
		glm::vec3 normal_041 = glm::normalize(glm::cross(vector04, vector01));		// normal of side2
		glm::vec3 normal_340 = glm::normalize(glm::cross(vector34, vector30));		// normal of side3
		glm::vec3 normal_243 = glm::normalize(glm::cross(vector24, vector23));		// normal of side4
		glm::vec3 normal_142 = glm::normalize(glm::cross(vector14, vector12));		// normal of side5

		unsigned int k = floats_per_vertex;
		// side 1 - push normals to vertices
		vertices[8]			= normal_0123.x;	vertices[9]			= normal_0123.y;	vertices[10]		 = normal_0123.z;
		vertices[k * 1 + 8]	= normal_0123.x;	vertices[k * 1 + 9]	= normal_0123.y;	vertices[k * 1 + 10] = normal_0123.z;
		vertices[k * 2 + 8]	= normal_0123.x;	vertices[k * 2 + 9]	= normal_0123.y;	vertices[k * 2 + 10] = normal_0123.z;
		vertices[k * 3 + 8]	= normal_0123.x;	vertices[k * 3 + 9]	= normal_0123.y;	vertices[k * 3 + 10] = normal_0123.z;
		// side 2 - push normals to vertices
		vertices[k * 4 + 8] = normal_041.x;		vertices[k * 4 + 9] = normal_041.y;		vertices[k * 4 + 10] = normal_041.z;
		vertices[k * 5 + 8] = normal_041.x;		vertices[k * 5 + 9] = normal_041.y;		vertices[k * 5 + 10] = normal_041.z;
		vertices[k * 6 + 8] = normal_041.x;		vertices[k * 6 + 9] = normal_041.y;		vertices[k * 6 + 10] = normal_041.z;
		// side 3 - push normals to vertices
		vertices[k * 7 + 8] = normal_340.x;		vertices[k * 7 + 9] = normal_340.y;		vertices[k * 7 + 10] = normal_340.z;
		vertices[k * 8 + 8] = normal_340.x;		vertices[k * 8 + 9] = normal_340.y;		vertices[k * 8 + 10] = normal_340.z;
		vertices[k * 9 + 8] = normal_340.x;		vertices[k * 9 + 9] = normal_340.y;		vertices[k * 9 + 10] = normal_340.z;
		// side 4 - push normals to vertices
		vertices[k * 10 + 8] = normal_243.x;	vertices[k * 10 + 9] = normal_243.y;	vertices[k * 10 + 10] = normal_243.z;
		vertices[k * 11 + 8] = normal_243.x;	vertices[k * 11 + 9] = normal_243.y;	vertices[k * 11 + 10] = normal_243.z;
		vertices[k * 12 + 8] = normal_243.x;	vertices[k * 12 + 9] = normal_243.y;	vertices[k * 12 + 10] = normal_243.z;
		// side 5 - push normals to vertices
		vertices[k * 13 + 8] = normal_142.x;	vertices[k * 13 + 9] = normal_142.y;	vertices[k * 13 + 10] = normal_142.z;
		vertices[k * 14 + 8] = normal_142.x;	vertices[k * 14 + 9] = normal_142.y;	vertices[k * 14 + 10] = normal_142.z;
		vertices[k * 15 + 8] = normal_142.x;	vertices[k * 15 + 9] = normal_142.y;	vertices[k * 15 + 10] = normal_142.z;

		//aux::PrintArray(vertices, vertices_size, floats_per_vertex);

		return OpenglObj3DData
		(
			number_of_vertices,
			floats_per_vertex,
			vertices,
			indices_size,
			indices,
			PosColorTexNormal_attribs_arrays_size,
			PosColorTexNormal_attribs_layouts,
			PosColorTexNormal_floats_per_attribs,
			PosColorTexNormal_attribs_offsets
		);
	}

};