#version 330 core

// inputs and outputs (linked inputs and outputs that carry data through shaders) 
// must have same names - required to link them

// inputs
in vec3 color;

// outputs
out vec4 FragColor;

void main()
{
	FragColor = vec4(color, 1.0f);
}
