#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <cmath>
#include <Eigen/Dense>
class Camera
{
public:
    Camera();

    void create(Eigen::Matrix4f projectionMatrix, Eigen::Vector3f position, Eigen::Vector3f target);
    
    //GET
    Eigen::Matrix4f getProjectionMatrix();
    Eigen::Matrix4f getViewMatrix();
    Eigen::Vector3f getPosition();
    
    void moveForward();
    void moveRight();
    void moveBackward();
    void moveLeft();
    void moveUp();
    void moveDown();
    
private:
    Eigen::Matrix4f projectionMatrix;
    Eigen::Vector3f position;
    Eigen::Vector3f currentMove;

    Eigen::Vector3f dirZ, dirY, dirX; // construir matrirz de rotacao
};

#endif