#version 430
in vec3 clCoord; //DEVE ser o mesmo nome da variavel de saida do vertexshader

out vec4 fragColor;

void main(void)
{	
	fragColor = vec4(clCoord, 1.0f);
}
