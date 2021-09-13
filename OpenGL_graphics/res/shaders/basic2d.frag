#version 330 core
out vec4 FragColor;

// inputs and uotputs must have same names - required to link them
in vec3 color;

uniform float scale;

void main()
{
	FragColor = vec4(color, 1.0f);		// inversed color = 1 - color
}