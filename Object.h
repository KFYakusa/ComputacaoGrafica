#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <vector>
#include <string>
#include <Eigen/Dense>

class Object
{
public:
    Object();

    void setColor(Eigen::Vector3f c);
    void setPosition(Eigen::Vector3f p);
    void setRotation(Eigen::Matrix4f r);
    void setScale(Eigen::Vector3f s);

    Eigen::Vector3f getColor();
    Eigen::Vector3f getPosition();
    Eigen::Matrix4f getRotation();
    Eigen::Matrix4f getTranslation();
    Eigen::Matrix4f getInverseTranslation();
    Eigen::Matrix4f getScaleMat();
    Eigen::Vector3f getScale();
    
private:
    Eigen::Matrix4f rotation;
    Eigen::Vector3f position;
    Eigen::Vector3f color;
    Eigen::Vector3f scale;
    
    int nVertices;
};

#endif