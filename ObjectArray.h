#ifndef OBJECT_ARRAY_H_INCLUDED
#define OBJECT_ARRAY_H_INCLUDED


#include "Object.h"

class ObjectArray : public Object
{
public:
    ObjectArray();
    
    void create(std::vector<Eigen::Vector3f> vertices, unsigned int vtsVerticePtr);

    void render();
    void deleteVAOVBOs();

    static std::vector<Eigen::Vector3f> getCubeVertices();
    static std::vector<Eigen::Vector3f> getPyramidVertices();
    
private:
    int nVertices;
    unsigned int VAO;
    unsigned int vertVBO;
    //unsigned int corVBO;
    //unsigned int idxVBO;
};

#endif