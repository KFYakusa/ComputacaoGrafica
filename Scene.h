#ifndef OPENGL_H_INCLUDED
#define OPENGL_H_INCLUDED


#include <string>
#include <vector>
#include <Eigen/Dense>
#include "ObjectPLY.h"
#include "Camera.h"

#include <GLFW/glfw3.h> //GLFW

class Scene
{
public:
    Scene();
    
    //singleton
    static void deleteInstance();
    static Scene* getInstance();

    //GET
    int getGlslMajorVersion();
    int getGlslMinorVersion();
    int getWinWidth();
    int getWinHeight();
    std::string getWinTitle();
    bool isFullscreen();
    bool isVSYNC();
    GLFWwindow* getGlfwWindow();

    std::vector<unsigned int>* getSeneShaderPrograms(); //retorna a REFERENCIA de objects
    int getCurrentObjectID();
    std::vector<ObjectPLY>* getSeneObjects(); //retorna a REFERENCIA de objects
    Camera* getCamera(); //retorna a REFERENCIA de camera


    //SET
    void setGlslMajorVersion(int majorVersion);
    void setGlslMinorVersion(int minorVersion);
    void setWinWidth(int w);
    void setWinHeight(int h);
    void setWinTitle(std::string title);
    void setFullscreen(bool fScreen);
    void setVSYNC(bool vsync);
    void setGlfwWindow(GLFWwindow* win);
    void setCurrentObjectID(int id);
    

private:
    static void printInstancePointer();

    //atributos opengl/glfw
    int glslMajorVersion;
    int glslMinorVersion;
    int winWidth;
    int winHeight;
    std::string winTitle;
    bool fullscreen;
    bool VSYNC;
    bool wireframe;
    GLFWwindow* glWin;

    //atributos cenas
    std::vector<unsigned int> programs; //shader program
    int currentObjectID;
    std::vector<ObjectPLY> objects;
    Camera camera;
    
    //singleton
    static Scene* instance; //singleton! -> https://www.tutorialspoint.com/Explain-Cplusplus-Singleton-design-pattern
};



#endif