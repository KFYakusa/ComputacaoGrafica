#ifndef OBJECT_LINES_H_INCLUDED
#define OBJECT_LINES_H_INCLUDED


#include "Object.h"

class ObjectLines : public Object
{
public:
    ObjectLines();
    
    void create( unsigned int vtsVerticePtr, int gridSize);

    void render();
    void deleteVAOVBOs();

private:
    int nVertices;
    unsigned int VAO;
    unsigned int vertVBO;

};

#endif