#version 430
// in vec4 corVertice; //DEVE ser o mesmo nome da variavel de saida do vertexshader

uniform vec3 cor;

void main(void)
{	
	gl_FragColor = vec4(cor, 1.0f);
}
