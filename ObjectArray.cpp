#include "ObjectArray.h"

#include <GL/glew.h> //GLEW
#include <GLFW/glfw3.h> //GLFW

using namespace Eigen;
using namespace std;

ObjectArray::ObjectArray(){
    
    VAO = 0;
    vertVBO =0;
    nVertices =0;

}


void ObjectArray::deleteVAOVBOs(){

    if(vertVBO > 0)
        glDeleteBuffers(1, &vertVBO);
    
    if(VAO > 0)
        glDeleteVertexArrays(1, &VAO);

	VAO = 0;
    vertVBO =0;
}

void ObjectArray::create(std::vector<Eigen::Vector3f> vertices, unsigned int vtsVerticePtr){
        
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
}



void ObjectArray::render(){

	//Ativa o array de vertices e cor
	glBindVertexArray(VAO);

	//desenha os triangulos
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei) nVertices);

	//desativa o array de vertices e cor
	glBindVertexArray(0);

}



std::vector<Eigen::Vector3f> ObjectArray::getCubeVertices(){
    //vertices
    //n faces = 12 triangulos 
    //nvertices = 36  (12 x 3)
    //Cada vertices possui 3 valors (X, Y, Z) = 36*3 = 108
    int nv = 108;
    float array[nv] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
    };

    int nVertices =  nv / 3;

	vector<Vector3f> vertices(nVertices);
	int idx = 0;
	for(size_t i=0; i<nVertices; i++){		
		vertices[i]= Vector3f(array[idx], array[idx+1], array[idx+2]);
		idx +=3;
	}

    return vertices;
}

std::vector<Eigen::Vector3f>  ObjectArray::getPyramidVertices(){
    
    int nv = 54;
    float array[nv] =
	{
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f
	};


   int nVertices =  nv / 3;

	vector<Vector3f> vertices(nVertices);
	int idx = 0;
	for(size_t i=0; i<nVertices; i++){		
		vertices[i]= Vector3f(array[idx], array[idx+1], array[idx+2]);
		idx +=3;
	}

    return vertices;
}


