#version 430
in vec2 txCoord; //DEVE ser o mesmo nome da variavel de saida do vertexshader
layout (binding=0) uniform sampler2D texture0; //(binding=0) -> GL_TEXTURE0

out vec4 fragColor;

void main(void)
{	
	//texture0 = 0;
	vec4 texColor= texture(texture0, txCoord);
	// if(texColor.a < 0.5)
	// 	discard;

	fragColor = texColor;
}
