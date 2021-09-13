#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec3 color;			// outputs the color for the fragment shader
out vec2 texCoord;		// outputs the texture coordinates for fragment shader

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;



void main()
{

	gl_Position = proj_mat * view_mat * model_mat * vec4(aPos, 1.0f);
	
	// assigns the colors from Vertex Data to "color"
	color = aColor;

	texCoord = aTex;
}
