// vertex shader
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec3 object_color;		// outputs the color for the fragment shader

// diffused light
out vec3 normal;
out vec3 frag_pos;

// texture
out vec2 texCoord;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

void main()
{
    gl_Position = proj_mat * view_mat * model_mat * vec4(aPos, 1.0);
	object_color = aColor;		// assigns the colors from Vertex Data to "color"
	
	// in order to calc. light, we need to provide a position of a fragment (to calc the diff between light pos and frag pos)
	// light calc will be performed relative to world coorinates, so
	// transforming frag position coordinates from model-related to world-related coordinate system
	frag_pos = vec3(model_mat * vec4(aPos, 1.0f));

	texCoord = aTex;

	// normalize() to correct the light when scaling model
	// mat3(...) to translate normal vector to a world-relative space
	normal = mat3(transpose(inverse(model_mat))) * aNormal;
}
