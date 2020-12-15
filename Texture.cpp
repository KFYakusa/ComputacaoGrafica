#include "Texture.h"

#include <iostream>
#include <SOIL2/soil2.h>

#include <GL/glew.h> //GLEW

using namespace std;


Texture::Texture(){

}


bool Texture::loadOpenGLTexture(std::string path, unsigned int &texturePtr,bool *alpha, bool verticalFlip, bool mipmap, bool anisotropicFilter){
    int width; 
    int height;
    int channels;
    
    
    
    //le os valores dos pixels da imagem, forcando para o o formato RGB
    unsigned char* imgData = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
	if (!imgData){
        cout << "Erro Texture: Erro ao ler o arquivo " << path << endl;
        return false;
    }
    
    //inverter a imagem verticalmente!
    if(verticalFlip){
        int i, j;
		for( j = 0; j*2 < height; ++j )
		{
			int index1 = j * width * channels;
			int index2 = (height - 1 - j) * width * channels;
			for( i = width * channels; i > 0; --i )
			{
				unsigned char temp = imgData[index1];
				imgData[index1] = imgData[index2];
				imgData[index2] = temp;
				++index1;
				++index2;
			}
		}
    }

    GLenum format;
    if (channels == 1)
        format = GL_RED;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4){
        format = GL_RGBA;
        *alpha =true;
    }
        
    std::cout<< "( inside loadOpenGLTexture )has alpha? "<< (*alpha)<<endl;
    std::cout<<" nome textura: "<<path<<endl;
    //gera um ponteiro para a texura e carrega a imagem na GPU
    glGenTextures(1, &texturePtr);
    glBindTexture(GL_TEXTURE_2D, texturePtr);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imgData);
    // if(*alpha)
    // {
    //     glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    //     glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    // }
    // else{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // }

    //Ativar mipmap?
    if(mipmap){
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //default
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//default
    }
    else{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
    }

    //ativar filtro?
	if (anisotropicFilter && glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		GLfloat anisoset = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoset);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoset);

        //cout << "anisoset: " << anisoset << endl;
	}

    //libera memoria
	glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data( imgData );
	return true;
}












