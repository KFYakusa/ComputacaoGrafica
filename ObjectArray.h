#ifndef OBJECT_ARRAY_H_INCLUDED
#define OBJECT_ARRAY_H_INCLUDED

#include <limits>

#include "Object.h"

#define NULL_SHADER_POINTER std::numeric_limits<unsigned int>::max()


enum OBJECT_TYPE { COORD=0, COORD_COLOR=1, COORD_TEXT=2, COORD_TEXT_COLOR=3, COORD_MULTI_TEXT=4, COORD_TEXT_BLEND=5};

class ObjectArray : public Object
{
public:
    bool hasAlpha;
    ObjectArray();
    
    void create(std::vector<Eigen::Vector3f> vertices, unsigned int vtsVerticePtr,
        std::vector<Eigen::Vector2f> texture=std::vector<Eigen::Vector2f>(0), unsigned int vtsTexturePtr=NULL_SHADER_POINTER,
        std::vector<Eigen::Vector3f> colors=std::vector<Eigen::Vector3f>(0), unsigned int vtsColorsPtr=NULL_SHADER_POINTER);

    void render();
    void deleteAll();

    int getType();

    void setTexture(std::string texPath, bool verticalFlip, bool mipmap, bool anisotropicFilter);
    void setTexture(std::string texPath1, std::string texPath2, bool verticalFlip, bool mipmap, bool anisotropicFilter);
    void setAlpha(bool hA);

    bool getAlpha();
    static void getPlane(std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector2f> &texture);
    static void getPlane(std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector2f> &texture, std::vector<Eigen::Vector3f> &colors);
    static void getFloor(std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector2f> &texture);
    
private:
    int nVertices;
    unsigned int VAO;
    unsigned int vertVBO;
    unsigned int texVBO;
    unsigned int colorVBO;
    unsigned int texturePtr1;
    unsigned int texturePtr2;


    //unsigned int idxVBO;
};

#endif