#pragma once

#include "renderer/basic/Shader.h"
#include "renderer/basic/ShaderProgram.h"
#include "renderer/basic/VertexArray.h"
#include "renderer/basic/VertexBuffer.h"
#include "renderer/basic/IndexBuffer.h"
#include "renderer/basic/Texture2D.h"
#include "OpenglObj3DData.h"

#include "util/FileReader.h"
#include "util/aux_.h"

#include <exception>
#include <math.h>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"


/*
	@description
	Object to create an opengl-context object (each obj - new pair of shaders and one shader program)
*/
class OpenglObj
{
	protected:

	OpenglObj3DData* obj_data = nullptr;
	VertexArray* va = nullptr;
	VertexBuffer* vb = nullptr;
	ShaderProgram* shader_program = nullptr;
	float rotation_angle = 0.0f;
	glm::mat4 model_mat = glm::mat4(1.0f);

	glm::vec3 current_position = glm::vec3(0.0f, 0.0f, 0.0f);
	float current_rotated_angle = 0.0f;
	glm::vec3 current_rotated_normal = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 current_scale = glm::vec3(1.0f, 1.0f, 1.0f);



	public:

	/*
	 *	@description
	 * Configurates vertices attributes all in one, using data from array pointers.
	 * All three arrays (layouts, components_sizes_in_floats and offsets) have
	 * to be the same size (because specifying attribute with some data missing is
	 * impossible).
	 *
	 * @params
	 * layouts_arr -	pointer to an array of (unsigned int) elements.
	 *					Layouts_arr[i] represents layout for an attribute number (i).
	 * floats_per_attribs_arr -	pointer to an array of (unsigned int) elements.
	 *							components_sizes_in_floats_arr[i] represents
	 *							component_size_in_floats for attrib number (i).
	 * offsets_arr -	array of ((void*) uint*sizeof(float)).
	 *					offsets_arr[i] represents offset for an attribute number (i).
	 * attribs_number - common size for arrays, specified above. layouts_arr,
	 *					floats_per_attribs_arr and offsets_arr must have the same size,
	 *					passed using this (attribs_number) function parameter.
	 *
	 **/
	void LinkVerticesAttributes(unsigned int* layouts_arr, unsigned int* floats_per_attribs_arr, unsigned int* offsets_arr, unsigned int attribs_arrays_size)
	{
		// the truth is that 
		// 1: (sum(components_sizes_in_floats)) has to be = (floats_per_vertex)
		// 1 - checking one_vertex_size_input correctness
		unsigned int input_based_floats_per_vertex = aux::Sum(floats_per_attribs_arr, attribs_arrays_size);
		if (input_based_floats_per_vertex != this->obj_data->floats_per_vertex)
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
			this->vb->LinkVertexAttrib
			(
				layouts_arr[i],
				floats_per_attribs_arr[i],
				GL_FLOAT,
				this->obj_data->floats_per_vertex * sizeof(float),
				(void*)(offsets_arr[i] * sizeof(float))
			);
		}
	}

	#pragma region getters

	unsigned int GetShaderProgramId() const
	{
		return this->shader_program->GetId();
	}

	glm::mat4 GetModelMatrix() const
	{
		return this->model_mat;
	}

	glm::vec3 GetPosition() const
	{
		return this->current_position;
	}

	float GetRotatedAngle() const
	{
		return this->current_rotated_angle;
	}

	glm::vec3 GetRotatedNormal() const
	{
		this->current_rotated_normal;
	}

	glm::vec3 GetScale() const
	{
		return this->current_scale;
	}

	#pragma endregion

	#pragma region setters
	void SetPosition(glm::vec3 new_position)
	{
		// transfer_matrix info
		/*
		this->model_mat =
		(
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			pos.x, pos.y,	pos.z,	1.0f
		);
		*/

		this->model_mat[3][0] = this->model_mat[3][0] - this->current_position.x + new_position.x;
		this->model_mat[3][1] = this->model_mat[3][1] - this->current_position.y + new_position.y;
		this->model_mat[3][2] = this->model_mat[3][2] - this->current_position.z + new_position.z;
		this->current_position = new_position;
	}

	void SetRotation(float angle, glm::vec3 rotation_normal)
	{
		// rotation matrix info		(does not work properly if uncomment)
		/*
		float new_angle = this->current_rotated_angle + angle;
		glm::mat4 rotation_mat
		(
			glm::cos(new_angle),	0.0f,	-glm::sin(new_angle),	0.0f,
			0.0f,					1.0f,	0.0f,					0.0f,
			glm::sin(new_angle),	0.0f,	glm::cos(new_angle),	0.0f,
			0.0f,					0.0f,	0.0f,					1.0f
		);
		this->model_mat = this->model_mat * rotation_mat;
		*/

		/// method 1 [not OK when rotating model, scaled with different xyz-scale ratios]
		// reset rotation
		this->model_mat = glm::rotate(this->model_mat, glm::radians(-this->current_rotated_angle), this->current_rotated_normal);
		//set new rotated angle
		this->current_rotated_angle = (this->current_rotated_angle <= 360.0f) ? (angle) : (angle - 360.0f);
		this->current_rotated_normal = rotation_normal;
		this->model_mat = glm::rotate(this->model_mat, glm::radians(angle), rotation_normal);

		/// method 2 [not OK at all]
		//this->model_mat[0][0] = this->model_mat[0][0] / glm::cos(this->current_rotated_angle);
		//this->model_mat[2][2] = this->model_mat[2][2] / glm::cos(this->current_rotated_angle);
		//this->model_mat[2][0] = this->model_mat[2][0] / -glm::sin(this->current_rotated_angle);
		//this->model_mat[0][2] = this->model_mat[0][2] / -glm::sin(this->current_rotated_angle);
		//this->current_rotated_angle = angle;
		//this->current_rotated_normal = rotation_normal;
	}

	/**
	*	@Why_Private?
	*	Because SetRotation() works incorrectly
	*	with a model, scaled non-linearly (scaled with different xyz-scale ratios).
	*
	*/
	private:
	void SetScale(glm::vec3 new_scale)
	{
		// scale matrix info
		/*
		glm::mat4 model_mat =
		(
			scale.x,	0.0f,		0.0f,		0.0f,
			0.0f,		scale.y,	0.0f,		0.0f,
			0.0f,		0.0f,		scale.z,	0.0f,
			0.0f,		0.0f,		0.0f,		1.0f
		);
		*/

		/// method 1
		this->model_mat[0][0] = this->model_mat[0][0] / this->current_scale.x * new_scale.x;
		this->model_mat[1][1] = this->model_mat[1][1] / this->current_scale.y * new_scale.y;
		this->model_mat[2][2] = this->model_mat[2][2] / this->current_scale.z * new_scale.z;
		this->current_scale = new_scale;

		/// method 2
		//glm::vec3 scale_diff = new_scale - this->current_scale + glm::vec3(1.0f, 1.0f, 1.0f);
		//this->model_mat = glm::scale(this->model_mat, scale_diff);
		//this->current_scale = new_scale;
	}

	public:
	void SetScale(float new_scale)
	{
		/// method 2
		glm::vec3 scale_diff = new_scale - this->current_scale + glm::vec3(1.0f, 1.0f, 1.0f);
		this->model_mat = glm::scale(this->model_mat, scale_diff);
		this->current_scale = glm::vec3(new_scale, new_scale, new_scale);
	}
	#pragma endregion

	#pragma region uniforms_getters_and_setters
	public:
	float GetUniform1f(std::string inshader_uniform_name) const
	{
		this->shader_program->GetUniform1f(inshader_uniform_name);
	}

	void SetUniform1f(std::string uniform_name, float uniform_value)
	{
		this->shader_program->SetUniform1f(uniform_name, uniform_value);
	}

	float GetUniform1ui(std::string inshader_uniform_name)
	{
		this->shader_program->GetUniform1f(inshader_uniform_name);
	}

	void SetUniform1ui(std::string uniform_name, unsigned int uniform_value)
	{
		this->shader_program->SetUniform1ui(uniform_name, uniform_value);
	}

	void SetUniformMatrix4f(std::string uniform_name, const GLfloat* matrix_data)
	{
		this->shader_program->SetUniformMatrix4f(uniform_name, matrix_data);
	}

	void SetUniformVec3(std::string uniform_name, const GLfloat* vector_data)
	{
		this->shader_program->SetUniformVec3(uniform_name, vector_data);
	}
	#pragma endregion

	// Use this method in a game-loop when vertices data and vertices layouts are to be modified and drawn in real-time
	/// UpdateIndicesData implementation will be close to UpdateVerticesData(),
	/// but require some additional changes of class data fields
	void UpdateVerticesData
	(
		float* new_vertices_data,
		unsigned int new_vertices_data_variable_size,
		unsigned int* attribs_layouts_arr,
		unsigned int* attribs_floats_per_attribs_arr,
		unsigned int* attribs_offsets_arr,
		unsigned int attribs_arrays_size
	)
	{
		this->vb->UpdateVerticesData(new_vertices_data, new_vertices_data_variable_size);
		IndexBuffer ib(this->obj_data->indices, this->obj_data->indices_size);

		// vertex array object
		this->va = new VertexArray;
		va->Bind();
		// vertex buffer object and index buffer object
		this->vb = new VertexBuffer(new_vertices_data, new_vertices_data_variable_size);
		ib.Bind();

		LinkVerticesAttributes
		(
			attribs_layouts_arr,
			attribs_floats_per_attribs_arr,
			attribs_offsets_arr,
			attribs_arrays_size
		);

		this->vb->Unbind();
		this->va->Unbind();
		ib.Unbind();
	}
	
	void UpdateVerticesData(OpenglObj3DData object_data)
	{
		this->UpdateVerticesData
		(
			object_data.vertices,
			object_data.vertices_size,
			object_data.attribs_layouts,
			object_data.floats_per_attribs,
			object_data.attribs_offsets,
			object_data.attribs_arrays_size
		);
	}

	void UseShaderProgram() const
	{
		this->shader_program->UseProgram();
	}

	virtual void Draw() = 0;
};

class OpenglObjColored : public OpenglObj
{
	private:
	aux::color::RGBAColor color;



	public:

	OpenglObjColored
	(
		OpenglObj3DData* obj_data,
		std::string vertex_shader_file_path,
		std::string fragment_shader_file_path,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)
	)
	{
		// allocate same amount of memory for vertices and indices as inputs
		this->obj_data = new OpenglObj3DData(*obj_data);
		// declare string for shaders' source codes
		std::string vertex_shader_source;
		std::string fragment_shader_source;
		// retrieve shaders' source codes
		vertex_shader_source = FileReader::Read(vertex_shader_file_path);
		fragment_shader_source = FileReader::Read(fragment_shader_file_path);

		// load shaders into opengl machine
		Shader vertex_shader(vertex_shader_source.c_str(), GL_VERTEX_SHADER, vertex_shader_file_path);
		Shader fragment_shader(fragment_shader_source.c_str(), GL_FRAGMENT_SHADER, fragment_shader_file_path);
		this->shader_program = new ShaderProgram(vertex_shader.GetId(), fragment_shader.GetId());

		IndexBuffer ib(this->obj_data->indices, this->obj_data->indices_size);
		// vertex array object
		this->va = new VertexArray;
		va->Bind();
		// vertex buffer object and index buffer object
		this->vb = new VertexBuffer(this->obj_data->vertices, this->obj_data->vertices_size);
		//vb.Bind();
		ib.Bind();

		LinkVerticesAttributes
		(
			obj_data->attribs_layouts,
			obj_data->floats_per_attribs,
			obj_data->attribs_offsets,
			obj_data->attribs_arrays_size
		);

		this->vb->Unbind();
		this->va->Unbind();
		ib.Unbind();
		this->SetPosition(position);
	}

	~OpenglObjColored()
	{
		if (this->obj_data) { this->obj_data->~OpenglObj3DData();	this->obj_data = nullptr; }
		if (this->va) { this->va->~VertexArray();				this->va = nullptr; }
		if (this->vb) { this->vb->~VertexBuffer();				this->vb = nullptr; }
		if (this->shader_program) { this->shader_program->~ShaderProgram();	this->shader_program = nullptr; }

	}

	glm::vec3 GetColor()
	{
		return glm::vec3(this->color.r, this->color.g, this->color.b);
	}
	void SetColor(glm::vec3 new_color)
	{
		aux::color::RGBAColor new_color_casted{ new_color.r, new_color.g, new_color.b, 1.0f };
		aux::color::TryIsColorValid(new_color_casted);
		this->color = new_color_casted;
	}

	void Draw()
	{
		this->va->Bind();
		glDrawElements(GL_TRIANGLES, this->obj_data->indices_size, GL_UNSIGNED_INT, 0);
	}

};

class OpenglObjTextured : public OpenglObj
{
	private:
	Texture2D* texture = nullptr;



	public:

	/**
	*	@params
	*	...
	*	texture_data	- (*arbitrary for specidying a texture) provide texture data to this object
	*	texture_sampler_uniform_value	- (*arbitrary for specidying a texture)
	*									name of the uniform in shader, specified for this object, of type sampler2D,
	*									used to retrieve a color from a texture
	*/
	OpenglObjTextured
	(
		OpenglObj3DData* obj_data,
		std::string vertex_shader_file_path,
		std::string fragment_shader_file_path,
		Texture2DData* texture_data,
		const char* texture_sampler_uniform_name,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)
	)
	{
		// allocate same amount of memory for vertices and indices as inputs
		this->obj_data = new OpenglObj3DData(*obj_data);
		// declare string for shaders' source codes
		std::string vertex_shader_source;
		std::string fragment_shader_source;

		// check all input data
		vertex_shader_source = FileReader::Read(vertex_shader_file_path);
		fragment_shader_source = FileReader::Read(fragment_shader_file_path);

		if (texture_data == nullptr)
		{
			throw std::exception(std::string("Texture data is not specified.").c_str());
		}

		if (texture_sampler_uniform_name == nullptr || texture_sampler_uniform_name[0] == '\0')
		{
			throw std::exception(std::string("Texture sampler uniform name is not specified or empty.").c_str());
		}

		// define shaders' source codes depending on texture value
		bool is_texture = texture_data;
		bool is_vertex_shader_file_path = !vertex_shader_file_path.empty();
		bool is_fragment_shader_file_path = !fragment_shader_file_path.empty();

		// load shaders into opengl machine
		Shader vertex_shader(vertex_shader_source.c_str(), GL_VERTEX_SHADER, vertex_shader_file_path);
		Shader fragment_shader(fragment_shader_source.c_str(), GL_FRAGMENT_SHADER, fragment_shader_file_path);

		IndexBuffer ib(this->obj_data->indices, this->obj_data->indices_size);
		this->shader_program = new ShaderProgram(vertex_shader.GetId(), fragment_shader.GetId());

		// vertex array object
		this->va = new VertexArray;
		va->Bind();
		// vertex buffer object and index buffer object
		this->vb = new VertexBuffer(this->obj_data->vertices, this->obj_data->vertices_size);
		//vb.Bind();
		ib.Bind();

		LinkVerticesAttributes
		(
			obj_data->attribs_layouts,
			obj_data->floats_per_attribs,
			obj_data->attribs_offsets,
			obj_data->attribs_arrays_size
		);

		this->vb->Unbind();
		this->va->Unbind();
		ib.Unbind();

		this->texture = new Texture2D(texture_data);
		this->shader_program->SetUniform1ui(texture_sampler_uniform_name, this->texture->GetTextureUniformValue());
		this->SetPosition(position);
	}

	~OpenglObjTextured()
	{
		if (this->texture)			{ this->texture->~Texture2D();				this->texture = nullptr;	}
		if (this->obj_data)			{ this->obj_data->~OpenglObj3DData();		this->obj_data = nullptr;	}
		if (this->va)				{ this->va->~VertexArray();					this->va = nullptr;			}
		if (this->vb)				{ this->vb->~VertexBuffer();				this->vb = nullptr;			}
		if (this->shader_program)	{ this->shader_program->~ShaderProgram();	this->shader_program = nullptr; }

	}

	void Draw()
	{
		if (this->texture != nullptr)
		{
			this->texture->Bind();
		}
		this->va->Bind();
		glDrawElements(GL_TRIANGLES, this->obj_data->indices_size, GL_UNSIGNED_INT, 0);
	}

};

class OpenglObjTexturedSpecmapped : public OpenglObj
{
	private:
	Texture2D* texture = nullptr;
	Texture2D* texture_specular_map = nullptr;



	public:

	/**
	*	@params
	*	...
	*	texture_data	- (*arbitrary for specidying a texture) provide texture data to this object
	*	texture_sampler_uniform_value	- (*arbitrary for specidying a texture)
	*									name of the uniform in shader, specified for this object, of type sampler2D,
	*									used to retrieve a color from a texture
	*/
	OpenglObjTexturedSpecmapped
	(
		OpenglObj3DData* obj_data,
		std::string vertex_shader_file_path,
		std::string fragment_shader_file_path,
		Texture2DData* texture_data,
		const char* texture_sampler_uniform_name,
		Texture2DData* texture_specular_map_data,
		const char* texture_specular_map_sampler_uniform_name,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)
	)
	{
		// allocate same amount of memory for vertices and indices as inputs
		this->obj_data = new OpenglObj3DData(*obj_data);
		// declare string for shaders' source codes
		std::string vertex_shader_source;
		std::string fragment_shader_source;

		// check all input data
		vertex_shader_source = FileReader::Read(vertex_shader_file_path);
		fragment_shader_source = FileReader::Read(fragment_shader_file_path);

		if (texture_data == nullptr)
		{
			throw std::exception(std::string("Texture data is not specified.").c_str());
		}

		if (texture_sampler_uniform_name == nullptr || texture_sampler_uniform_name[0] == '\0')
		{
			throw std::exception(std::string("Texture sampler uniform name is not specified or empty.").c_str());
		}

		if (texture_specular_map_data == nullptr)
		{
			throw std::exception(std::string("Texture specular map data is not specified.").c_str());
		}

		if (texture_specular_map_sampler_uniform_name == nullptr || texture_specular_map_sampler_uniform_name[0] == '\0')
		{
			throw std::exception(std::string("Texture specular map sampler uniform name is not specified or empty.").c_str());
		}

		// load shaders into opengl machine
		Shader vertex_shader(vertex_shader_source.c_str(), GL_VERTEX_SHADER, vertex_shader_file_path);
		Shader fragment_shader(fragment_shader_source.c_str(), GL_FRAGMENT_SHADER, fragment_shader_file_path);
		this->shader_program = new ShaderProgram(vertex_shader.GetId(), fragment_shader.GetId());

		IndexBuffer ib(this->obj_data->indices, this->obj_data->indices_size);
		// vertex array object
		this->va = new VertexArray;
		va->Bind();
		// vertex buffer object and index buffer object
		this->vb = new VertexBuffer(this->obj_data->vertices, this->obj_data->vertices_size);
		//vb.Bind();
		ib.Bind();

		LinkVerticesAttributes
		(
			obj_data->attribs_layouts,
			obj_data->floats_per_attribs,
			obj_data->attribs_offsets,
			obj_data->attribs_arrays_size
		);

		this->vb->Unbind();
		this->va->Unbind();
		ib.Unbind();

		this->texture = new Texture2D(texture_data);
		this->shader_program->SetUniform1ui(texture_sampler_uniform_name, this->texture->GetTextureUniformValue());

		this->texture_specular_map = new Texture2D(texture_specular_map_data);
		this->shader_program->SetUniform1ui(texture_specular_map_sampler_uniform_name, this->texture_specular_map->GetTextureUniformValue());
		
		this->SetPosition(position);
	}

	~OpenglObjTexturedSpecmapped()
	{
		if (this->texture)				{ this->texture->~Texture2D();				this->texture = nullptr; }
		if (this->texture_specular_map)	{ this->texture_specular_map->~Texture2D();	this->texture_specular_map = nullptr; }
		if (this->obj_data)			{ this->obj_data->~OpenglObj3DData();			this->obj_data = nullptr; }
		if (this->va)				{ this->va->~VertexArray();						this->va = nullptr; }
		if (this->vb)				{ this->vb->~VertexBuffer();					this->vb = nullptr; }
		if (this->shader_program)	{ this->shader_program->~ShaderProgram();		this->shader_program = nullptr; }

	}

	void Draw()
	{
		if (this->texture != nullptr)
		{
			this->texture->Bind();
			this->texture_specular_map->Bind();
		}
		this->va->Bind();
		glDrawElements(GL_TRIANGLES, this->obj_data->indices_size, GL_UNSIGNED_INT, 0);
	}

};