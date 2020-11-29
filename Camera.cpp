
#include "Camera.h"
#include "Math.h"
#include <limits>

using namespace Eigen;
Camera::Camera(){
    projectionMatrix = Eigen::Matrix4f::Identity();
    position = Eigen::Vector3f::Zero();
}

void Camera::create(Eigen::Matrix4f proj, Eigen::Vector3f pos, Eigen::Vector3f target){
    projectionMatrix = proj;
    position = pos;

    dirZ = position - target;
    dirZ.normalize();

    Eigen::Vector3f auxY(0.0f, 1.0f, 0.0f);
    dirX = auxY.cross(dirZ);
    dirX.normalize();

    dirY = dirZ.cross(dirX);
    dirY.normalize();


    currentMove = Vector3f::Zero();

    ativarCamera =  true;
    firstMouseMove = true;
    rotFactor = 0.001f;
    factor =1.0f;
}

Eigen::Matrix4f Camera::getViewMatrix(){

    if( currentMove.norm() > std::numeric_limits<float>::epsilon()){
        position += currentMove * 0.004;
        currentMove = Vector3f::Zero();
    }
    Eigen::Matrix4f R = Eigen::Matrix4f::Identity();

    R(0,0) = dirX(0);
    R(0,1) = dirX(1);
    R(0,2) = dirX(2);

    R(1,0) = dirY(0);
    R(1,1) = dirY(1);
    R(1,2) = dirY(2);

    R(2,0) = dirZ(0);
    R(2,1) = dirZ(1);
    R(2,2) = dirZ(2);


    Eigen::Matrix4f T = Math::translationMat(-position);

    Eigen::Matrix4f V = R * T;

    return V;


}

//GET
Eigen::Matrix4f Camera::getProjectionMatrix(){

    return projectionMatrix;
}
Eigen::Vector3f Camera::getPosition(){
    return position;
}

void Camera::moveForward(){
    currentMove -= (dirZ * factor);
}
void Camera::moveBackward(){
    currentMove += (dirZ * factor);
}

void Camera::moveRight(){
    currentMove += (dirX * factor);
}
void Camera::moveLeft(){
    currentMove -= (dirX *factor);
}

void Camera::moveUp(){
    currentMove +=(dirY * factor);
}
void Camera::moveDown(){
    currentMove -=(dirY*factor);
}

bool Camera::getAtivarCamera(){
    return ativarCamera;
}
void Camera::setAtivarCamera(bool onOff){
    ativarCamera = onOff;
}
void Camera::rotate(double x, double y){

    if(!ativarCamera)
	    return;

    if(firstMouseMove){
        firstMouseMove=false;
        oldMouseX=x;
        oldMouseY=y;
        return;
    }

    double movX = x - oldMouseX;
    double movY = y - oldMouseY;
    oldMouseY = y;
    oldMouseX = x;

    if(movX!= 0){
        yaw-=movX * rotFactor;
    }

    Math::spherical2cartesian(pitch,yaw,dirZ);
    dirZ.normalize();

    Eigen::Vector3f auxY(0.0f,1.0f,0.0f);
    dirX = auxY.cross(dirZ);
    dirX.normalize();

    dirY= dirZ.cross(dirX);
    dirY.normalize();


    if(movY!=0){
        double angle = movY * rotFactor;
        double aux = pitch + angle;
        if(abs(aux) < M_PI_2){
            pitch+=angle;
        }
    }
}

float Camera::getFactor(){
    return factor;  
}
void Camera::setFactor( float fator){
    factor = fator;
}

float Camera::getRotFactor(){
    return rotFactor;
}

void Camera::setRotFactor( float rf){
    rotFactor = rf;
}