#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED


#include <string>
#include <vector>
#include <Eigen/Dense>
#include "ObjectArray.h"
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
    
    int getCurrentObjectID();
    std::vector<ObjectArray>* getSeneObjects(); //retorna a REFERENCIA de objects
    std::vector<unsigned int>* getSeneShaderPrograms();
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
    double getRTF();
    double getLastTime();

    void setScale(float s);
    float getScale();
    
    void setRTF(double RTF);
    void setLastTime(double LastTime);

private:
    static void printInstancePointer();

    //atributos opengl/glfw
    int glslMajorVersion;
    int glslMinorVersion;
    int winWidth;
    int winHeight;
    double rtf;
    double lastTime;
    std::string winTitle;
    bool fullscreen;
    bool VSYNC;
    bool wireframe;
    GLFWwindow* glWin;

    //atributos cenas
    std::vector<unsigned int> programs; //shader program
    std::vector<ObjectArray> objects;
    Camera camera;
    float scaleGUI;
    
    //singleton
    static Scene* instance; //singleton! -> https://www.tutorialspoint.com/Explain-Cplusplus-Singleton-design-pattern
};



#endif