#ifndef FUNCOES_H_INCLUDED
#define FUNCOES_H_INCLUDED

#include <cmath>
#include <Eigen/Dense>
class Math
{
public:

    static void spherical2cartesian(float phi,float theta, Eigen::Vector3f &v) {
        v(2) = cos(phi) * cos(theta);
        v(0) = cos(phi) * sin(theta);
        v(1) = sin(phi);
    }
    static void cartesian2spherical(Eigen::Vector3f &v,float &phi,float &theta) {
        theta = atan2(v[0],v[2]);
        phi = asinf(v[1]);
    }

    static float degree2rad(float g) {
        float r = g * (float)(M_PI / 180.0);

        return r;
    }


    static double degree2rad(double g) {
        double r = g * (double)(M_PI / 180.0);

        return r;
    }

    static Eigen::Matrix4f translationMat(Eigen::Vector3f fator){

        Eigen::Matrix4f mat = Eigen::Matrix4f::Identity();
        mat(0,3) = fator.x();
        mat(1,3) = fator.y();
        mat(2,3) = fator.z();

        return mat;
    }

    static Eigen::Matrix4f scaleMat(Eigen::Vector3f fator){

        Eigen::Matrix4f mat = Eigen::Matrix4f::Identity();
        mat(0,0) = fator.x();
        mat(1,1) = fator.y();
        mat(2,2) = fator.z();

        return mat;
    }

    //passar radianos
     static Eigen::Matrix4f zRotationMat(float fator){

        Eigen::Matrix4f mat = Eigen::Matrix4f::Identity();

        float s = sin(fator);
        float c = cos(fator);

        mat(0,0) = c;
        mat(0,1) = -s;

        mat(1,0) = s;
        mat(1,1) = c;

        return mat;
    }

    static Eigen::Matrix4f xRotationMat(float fator){

        Eigen::Matrix4f mat = Eigen::Matrix4f::Identity();

        float s = sin(fator);
        float c = cos(fator);

        mat(1,1) = c;
        mat(1,2) = -s;

        mat(2,1) = s;
        mat(2,2) = c;

        return mat;
    }

    static Eigen::Matrix4f yRotationMat(float fator){

        Eigen::Matrix4f mat = Eigen::Matrix4f::Identity();

        float s = sin(fator);
        float c = cos(fator);

        mat(0,0) = c;
        mat(0,2) = s;

        mat(2,0) = -s;
        mat(2,2) = c;

        return mat;
    }
    
    static Eigen::Matrix4f perspectiveProjMat(float FOV, float aspectRatio, float zNear, float zFar){
        Eigen::Matrix4f mat = Eigen::Matrix4f::Zero();
        float halfFOV = tan(FOV/2.0f);

        mat(0,0) = 1.0f / (halfFOV * aspectRatio);
        mat(1,1) = 1.0f / halfFOV;
        mat(2,2) = (zNear+zFar) / (-zFar + zNear);
        mat(2,3) = (2.0f * zNear * zFar) / (-zFar + zNear);
        mat(3,2) = -1.0f;
    
        return mat;
    }



};

#endif