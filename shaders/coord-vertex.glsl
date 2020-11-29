#version 430

layout (location=0) in vec3 vertCoord;
uniform mat4 PVM;

void main(void)
{
	gl_Position = PVM * vec4(vertCoord, 1.0);
} 
