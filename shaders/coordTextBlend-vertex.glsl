#version 430

layout (location=0) in vec3 vertCoord;
layout (location=1) in vec2 texCoord;
uniform mat4 PVM;

out vec2 txCoord;

void main(void)
{
	gl_Position = PVM * vec4(vertCoord, 1.0);
	txCoord = texCoord;
} 