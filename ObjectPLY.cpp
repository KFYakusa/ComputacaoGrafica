#include "ObjectPLY.h"

#include <GL/glew.h> //GLEW
#include <GLFW/glfw3.h> //GLFW

using namespace Eigen;
using namespace std;

ObjectPLY::ObjectPLY(){
    
    VAO = 0;
    vertVBO =0;
	corVBO=0;
    nVertices =0;

}


void ObjectPLY::deleteVAOVBOs(){

    if(vertVBO > 0)
        glDeleteBuffers(1, &vertVBO);
    
    if(VAO > 0)
        glDeleteVertexArrays(1, &VAO);

	VAO = 0;
    vertVBO =0;
}

void ObjectPLY::create(std::vector<Eigen::Vector3f> vertices, unsigned int vtsVerticePtr,unsigned int vtsCorptr, vector<Vector3f> cor){
        
	nVertices = vertices.size();

    //Cria o Vertex Array Object (VAO). Ele vai armazenar as informações do buffer e como este buffer será acessado no vertex shader
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //bind

	//cria o buffer para armazenar os vértices
	glGenBuffers(1, &vertVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3f), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(vtsVerticePtr);  	//define como o buffer será acessado no vertex shader!
	glVertexAttribPointer(vtsVerticePtr, 3,	GL_FLOAT, GL_FALSE,	0, (void*)0);

	//cria buffer pra armazenar as cores
	glGenBuffers(1, &corVBO);
	glBindBuffer(GL_ARRAY_BUFFER, corVBO);
	glBufferData(GL_ARRAY_BUFFER, cor.size() * sizeof(Vector3f), &cor[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(vtsCorptr);  	
	glVertexAttribPointer(vtsCorptr, 3,	GL_FLOAT, GL_FALSE,	0, (void*)0);


	//unbind all !!
	glBindVertexArray(0); 
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void ObjectPLY::render(){

	//Ativa o array de vertices e cor
	glBindVertexArray(VAO);

	//desenha os triangulos
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei) nVertices);

	//desativa o array de vertices e cor
	glBindVertexArray(0);

}




