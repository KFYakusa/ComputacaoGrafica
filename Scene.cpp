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
    currentObjectID = 0;
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
int Scene::getCurrentObjectID(){
    return currentObjectID;
}

std::vector<ObjectArray>* Scene::getSceneObjects(){
    return &objects;
}


std::vector<ObjectPLY>* Scene::getScenePLYObjects(){
    return &objectsPLY;
}


ObjectLines* Scene::getFloor(){
    return &floor;
}

Camera* Scene::getCamera(){
    return &camera;
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
void Scene::setCurrentObjectID(int id){
    currentObjectID = id;
}
