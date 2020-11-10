#version 430

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;

uniform mat4 PVM;
//uniform float Tx;
out vec4 corVertice;

void main(void)
{
	gl_Position = PVM * vec4(position, 1.0);
	corVertice = vec4(color,1.0);
} 
