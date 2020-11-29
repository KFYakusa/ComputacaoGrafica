#version 430
in vec3 clCoord; //DEVE ser o mesmo nome da variavel de saida do vertexshader
in vec2 txCoord; //DEVE ser o mesmo nome da variavel de saida do vertexshader
layout (binding=0) uniform sampler2D texture0;  //(binding=0) -> GL_TEXTURE0

out vec4 fragColor;

void main(void)
{	
	fragColor = texture(texture0, txCoord) * vec4(clCoord, 1.0f);
}
