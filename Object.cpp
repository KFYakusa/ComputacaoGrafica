#include "Object.h"
#include "Math.h"
#include <GL/glew.h> //GLEW
#include <GLFW/glfw3.h> //GLFW

using namespace Eigen;


Object::Object(){
    nVertices =0;
    rotation = Matrix4f::Identity();
    position = Vector3f(0,0,0);
    fatorEscale = Vector3f::Ones();
    color = Vector3f(1.0f,0.0f,0.0f);
}

void Object::setColor(Eigen::Vector3f c){
    color = c;
}
void Object::setPosition(Eigen::Vector3f p){
    position = p;
}
void Object::setRotation(Eigen::Matrix4f r){
    rotation = r;
}
void Object::setEscale(Eigen::Vector3f e){
    fatorEscale = e;
}

Eigen::Vector3f Object::getColor(){
    return color;
}
Eigen::Vector3f Object::getPosition(){
    return position;
}
Eigen::Matrix4f Object::getRotation(){
    return rotation;
}
Eigen::Matrix4f Object::getTranslation(){
    return Math::translationMat(position);
}
Eigen::Matrix4f Object::getEscale(){
    return Math::scaleMat(fatorEscale);
}
