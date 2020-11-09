#include "Camera.h"

Camera::Camera(){
    projectionMatrix = Eigen::Matrix4f::Identity();
    position = Eigen::Vector3f::Zero();
}

void Camera::create(Eigen::Matrix4f proj, Eigen::Vector3f pos){
    projectionMatrix = proj;
    position = pos;
}

//GET
Eigen::Matrix4f Camera::getProjectionMatrix(){
    return projectionMatrix;
}
Eigen::Vector3f Camera::getPosition(){
    return position;
}
