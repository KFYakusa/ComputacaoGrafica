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

    bool getAtivarCamera();
    void setAtivarCamera(bool on);
    void setFactor( float fator);
    void setRotFactor( float rotFator);

    void rotate(double x,double y);

    void moveForward();
    void moveRight();
    void moveBackward();
    void moveLeft();
    void moveUp();
    void moveDown();
    float getFactor();
    float getRotFactor();


    
private:
    Eigen::Matrix4f projectionMatrix;
    Eigen::Vector3f position;
    Eigen::Vector3f currentMove;

    bool ativarCamera;
    Eigen::Vector3f dirZ, dirY, dirX; // para construir matriz de rotação
    double oldMouseX,oldMouseY;
    bool firstMouseMove;
    float pitch,yaw;
    float factor;
    float rotFactor;
};

#endif