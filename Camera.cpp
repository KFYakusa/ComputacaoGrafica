#include "Camera.h"
#include "Math.h"
#include <limits>
Camera::Camera(){
    projectionMatrix = Eigen::Matrix4f::Identity();
    position = Eigen::Vector3f::Zero();
}

void Camera::create(Eigen::Matrix4f proj, Eigen::Vector3f pos,Eigen::Vector3f target){
    projectionMatrix = proj;
    position = pos;
    dirZ = position - target;
    dirZ.normalize();

    Eigen::Vector3f auxY(0.0f,1.0f,0.0f);
    dirX = auxY.cross(dirZ);
    dirX.normalize();

    dirY= dirZ.cross(dirX);
    dirY.normalize();


}

//GET
Eigen::Matrix4f Camera::getProjectionMatrix(){
    return projectionMatrix;
}
Eigen::Vector3f Camera::getPosition(){
    return position;
}
Eigen::Matrix4f Camera::getViewMatrix(){
    if(currentMove.norm() >  std::numeric_limits<float>::epsilon()){
        position+=currentMove*0.05;
        currentMove = Eigen::Vector3f::Zero();
    }
    Eigen::Matrix4f V = Eigen::Matrix4f::Identity();
    
    V(0,0) = dirX(0);    
    V(0,1) = dirX(1);    
    V(0,2) = dirX(2);  

    V(1,0) = dirY(0);    
    V(1,1) = dirY(1);    
    V(1,2) = dirY(2);  

    V(2,0) = dirZ(0);    
    V(2,1) = dirZ(1);    
    V(2,2) = dirZ(2);  

    Eigen::Matrix4f T = Math::translationMat(-position);

    Eigen::Matrix4f Final = V * T;
    return Final;
}

void Camera::moveForward(){
    currentMove -= dirZ;
}
void Camera::moveBackward(){
    currentMove += dirZ;
}

void Camera::moveRight(){
    currentMove += dirX;
}
void Camera::moveLeft(){
    currentMove -= dirX;
}

void Camera::moveUp(){
    currentMove +=dirY;
}
void Camera::moveDown(){
    currentMove -=dirY;
}
