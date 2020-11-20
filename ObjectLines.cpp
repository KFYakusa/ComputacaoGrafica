#include "ObjectLines.h"
#include "Math.h"
#include <GL/glew.h> //GLEW
#include <GLFW/glfw3.h> //GLFW

using namespace Eigen;
using namespace std;

ObjectLines::ObjectLines(){
    
    VAO = 0;
    vertVBO =0;
    nVertices =0;

}


void ObjectLines::deleteVAOVBOs(){

    if(vertVBO > 0)
        glDeleteBuffers(1, &vertVBO);
    
    if(VAO > 0)
        glDeleteVertexArrays(1, &VAO);

	VAO = 0;
    vertVBO =0;
}

void ObjectLines::create(unsigned int vtsVerticePtr,int gridSize){
    
	vector<Vector3f> vertices;
    
	// //primeira Linha
	// vertices.push_back(Vector3f(20.0f, 0.0f, -5.0f));
	// vertices.push_back(Vector3f(-20.0f, 0.0f, -5.0f));

	// //segunda linha
	// vertices.push_back(Vector3f(20.0f, 0.0f, -10.0f));
	// vertices.push_back(Vector3f(-20.0f, 0.0f, -10.0f));
		
	for(int i = -gridSize;i<gridSize;i++){
		vertices.push_back(Vector3f((float) gridSize, -2.0f, 0.0f -(float) i));
		vertices.push_back(Vector3f(-(float) gridSize, -2.0f, 0.0f -(float) i));
	}
	for(int i = -gridSize ;i<gridSize;i++){
		vertices.push_back(Vector3f(0.0f -(float) i, -2.0f,(float) gridSize ));
		vertices.push_back(Vector3f(0.0f -(float) i, -2.0f,-(float) gridSize));
	}


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


	//unbind all !!
	glBindVertexArray(0); 
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	vertices.clear();

}


void ObjectLines::render(){

	//Ativa o array de vertices e cor
	glBindVertexArray(VAO);

	//desenha as linhas
	glDrawArrays(GL_LINES, 0, (GLsizei) nVertices);

	//desativa o array de vertices e cor
	glBindVertexArray(0);

}

