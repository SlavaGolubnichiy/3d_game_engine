#version 330 core

// inputs and outputs (linked inputs and outputs that carry data through shaders) 
// must have same names - required to link them

// inputs
in vec3 color;
in vec2 texCoord;	// takes texture coordinates, output by vertex shader as input
// outputs
out vec4 FragColor;

// uniforms (like global variables for GLSL and C++/OpenGL code (linking required for c++))
uniform sampler2D tex0;
uniform vec3 ambient_light;

// local variable of type like array of 4 floats (vector4, vec4)
vec4 texColor;

void main()
{
	// texture() colors the object triangles with color, retrieved from texture image
	texColor = texture(tex0, texCoord);
	FragColor = texColor * vec4(ambient_light, 1.0f);
}