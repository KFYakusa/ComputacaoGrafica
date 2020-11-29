#include "Scene.h"
#include <iostream>


using namespace std;


Scene *Scene::instance = nullptr;

Scene::Scene(){
    
    glslMajorVersion = 4;
    glslMinorVersion = 3;
    winWidth = 800;
    winHeight = 600;
    fullscreen = false;
    VSYNC = false;
    glWin = nullptr;
    rtf = 0.0;
    lastTime = 0.0;
}


Scene* Scene::getInstance(){
    if (!instance)
        instance = new Scene;

    //Scene::printInstancePointer();

    return instance;
}

void Scene::deleteInstance(){    
    
    if (instance){
        delete instance;
        instance = nullptr;
    }
        
	//Scene::printInstancePointer();
}


void Scene::printInstancePointer(){
    std::cout << "Endereco da variavel instance: " << reinterpret_cast<unsigned long long>(instance) << std::endl;
}


//GET
int Scene::getGlslMajorVersion(){
    return glslMajorVersion;
}
int Scene::getGlslMinorVersion(){
    return glslMinorVersion;
}
int Scene::getWinWidth(){
    return winWidth;
}
int Scene::getWinHeight(){
    return winHeight;
}
std::string Scene::getWinTitle(){
    return winTitle;
}
bool Scene::isFullscreen(){
    return fullscreen;
}
bool Scene::isVSYNC(){
    return VSYNC;
}
GLFWwindow* Scene::getGlfwWindow(){
    return glWin;
}
std::vector<unsigned int>*Scene::getSeneShaderPrograms(){
    return &programs;
}
std::vector<ObjectArray>* Scene::getSeneObjects(){
    return &objects;
}

 void Scene::setScale(float s){
     scaleGUI = s;
 }
float Scene::getScale(){
    return scaleGUI;
}


Camera* Scene::getCamera(){
    return &camera;
}

double Scene::getRTF(){
    return rtf;
}
double Scene::getLastTime(){
    return lastTime;
}


//SET
void Scene::setGlslMajorVersion(int majorVersion){
    glslMajorVersion = majorVersion;
}
void Scene::setGlslMinorVersion(int minorVersion){
    glslMinorVersion = minorVersion;
}
void Scene::setWinWidth(int w){
    winWidth = w;
}
void Scene::setWinHeight(int h){
    winHeight = h;
}
void Scene::setWinTitle(std::string title){
    winTitle = title;
}
void Scene::setFullscreen(bool fScreen){
    fullscreen = fScreen;
}
void Scene::setVSYNC(bool vsync){
    VSYNC = vsync;
}
void Scene::setGlfwWindow(GLFWwindow* win){
    glWin = win;
}
void Scene::setRTF(double RTF){
    rtf = RTF;
}
void Scene::setLastTime(double LastTime){
    lastTime = LastTime;
}