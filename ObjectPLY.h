#ifndef OBJECT_PLY_H_INCLUDED
#define OBJECT_PLY_H_INCLUDED


#include "Object.h"

class ObjectPLY : public Object
{
public:
    ObjectPLY();
    void create(std::vector<Eigen::Vector3f> vertices, unsigned int vtsVerticePtr,unsigned int vtsCorptr, std::vector<Eigen::Vector3f> cor);

    void render();
    void deleteVAOVBOs();

    
private:
    int nVertices;
    unsigned int VAO;
    unsigned int vertVBO;
    unsigned int corVBO;
    //unsigned int idxVBO;
};

#endif