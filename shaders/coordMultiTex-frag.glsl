#version 430
in vec2 txCoord; //DEVE ser o mesmo nome da variavel de saida do vertexshader
layout (binding=0) uniform sampler2D texture0; //(binding=0) -> GL_TEXTURE0
layout (binding=1) uniform sampler2D texture1; //(binding=1) -> GL_TEXTURE1

uniform float mixFactor;

out vec4 fragColor;

void main(void)
{	
	fragColor = mix(texture(texture0, txCoord), texture(texture1, txCoord), mixFactor);
}
