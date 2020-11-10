#version 430

layout (location=0) in vec3 position;
uniform mat4 PVM;


void main(void)
{
	gl_Position = PVM * vec4(position, 1.0);
} 
