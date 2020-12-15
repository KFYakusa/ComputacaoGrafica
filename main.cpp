
#include <iostream>
#include <iomanip> 
#include <string> 
#include <vector>
#include <streambuf>
#include <map>
#include <fstream>

#include <Eigen/Dense>

#include <GL/glew.h> //GLEW
#include <GLFW/glfw3.h> //GLFW

#include "Scene.h"
#include "Shader.h"
#include "Math.h"
#include "Screenshot.h"

using namespace std;
using namespace Eigen;

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


#define EXCEPTION(msg) \
{ \
	std::set_terminate([](){ std::cout << "ERRO na linha " <<  __LINE__ << " do arquivo " << __FILE__  << "\t-> " << msg << std::endl; std::abort();}); \
	std::terminate(); \
} 


//inicializar OPENGL e imGUI, chamar o método no inicio do MAIN
void initOpenGL();

//chamar a função ANTES de renderizar um quadro -> limpa os buffer do OPENGL, calcula FPS/RTF
void beforeRender();

//renderiza um quadro
void render();

//chamar a função DEPOIS de renderizar um quadro -> swapBuffers, eventos de teclado
void afterRender();

//remove a janela e libera memória
void deleteWindow();

//verifica se usuário clicou para fechar a janela [X] ou pressionou ESC
bool isOpenWindow();
//pega posição do mouse
void mouseEvent(GLFWwindow* window, double xpos, double ypos);
//evento de teclado (o evento é disparado cada vez que usuário pressiona uma tecla)
//ideal para ativar ou desavivar uma funcionalidade do sistema via teclado
void keyEvent(GLFWwindow* win, int key, int scancode, int action, int mods);

//evento de teclado (o evento é disparado a cada passada do loop de renderização)
//ideal para controlar a movimentação da câmera
void renderkeyEvent();

//cria um novo frame para as janelas 2D -> GUI
void guiNewFrame();

//renderiza as janelas 2D -> GUI
void guiRender();

void ReadPLYWtexture(std::vector<ObjectArray> *objects, unsigned int *vtsVerticePtr, int *vtsTexturePtr, string nomeImagem,Vector3f position);

int main(void) {

	//1 - INICIAR CENA, fazer isso antes de inicializar o OPENGL!
	Scene *scene = Scene::getInstance();
	scene->setFullscreen(false); //tela cheia?
	scene->setWinWidth(1024); //dimensão da janela
	scene->setWinHeight(768);
	scene->setWinTitle("Exemplo");
	scene->setGlslMajorVersion(4);  //GLSL 4.3
	scene->setGlslMinorVersion(3);
	scene->setVSYNC(true);
	
	//2 - INICIAR OPENGL -> execute esta função sempre depois de inicializar a cena!
	initOpenGL();


	//https://www.khronos.org/opengl/wiki/Layout_Qualifier_(GLSL)
	//ponteiros para as variaveis de entrada do Vertex Shader
	//o valor do ponteiro é o valor especificado para LOCATION no Vertex Shader!
	GLuint vtsVerticePtr = 0; //layout (location=0)
	GLint vtsTexturePtr = 1; //layout (location=1)
	GLint vtsColorPtr = 2; //layout (location=2)

	
	//cria e compila os shaders!
	std::vector<string> vertex(6);
	std::vector<string> fragment(6);
	vertex[COORD] = "coord-vertex.glsl"; fragment[COORD] = "coord-frag.glsl";
	vertex[COORD_COLOR] = "coordColor-vertex.glsl"; fragment[COORD_COLOR] = "coordColor-frag.glsl";
	vertex[COORD_TEXT] = "coordTex-vertex.glsl"; fragment[COORD_TEXT] = "coordTex-frag.glsl";
	vertex[COORD_TEXT_COLOR] = "coordTexColor-vertex.glsl"; fragment[COORD_TEXT_COLOR] = "coordTexColor-frag.glsl";
	vertex[COORD_MULTI_TEXT] = "coordMultiTex-vertex.glsl"; fragment[COORD_MULTI_TEXT] = "coordMultiTex-frag.glsl";
	vertex[COORD_TEXT_BLEND] = "coordTextBlend-vertex.glsl"; fragment[COORD_TEXT_BLEND] = "coordTextBlend-frag.glsl";
	for(size_t i=0; i<vertex.size();i++){
		unsigned int program;
		if (!Shader::createProgram(&program, "shaders/"+vertex[i], "shaders/"+fragment[i])) { //ocorreu erro na criação do programa ??
			deleteWindow();
			EXCEPTION( "Erro ao criar os shaders" );
		}

		scene->getSeneShaderPrograms()->push_back(program);
	}
	
	//CÂMERA
	//Matriz de projeção perspectiva e posicionamento
	float FOV = Math::degree2rad(60.0f);
	float zNear = 0.1f;
	float zFar = 500.0f;
	float aspectRatio = float(scene->getWinWidth()) / float(scene->getWinHeight());
	Matrix4f projectionMat = Math::perspectiveProjMat(FOV, aspectRatio, zNear, zFar);
	// Vector3f posCamera = Vector3f(0.0f, 10.0f, 50.0f);
	Vector3f posCamera = Vector3f(0.0f, 0.0f, 10.0f);
	Vector3f posTarget= Vector3f(0.0f, 0.0f, 0.0f);

	//atribui a matriz de projeção e posição da camera a cena
	scene->getCamera()->create(projectionMat, posCamera, posTarget);


	//pega o vetor de objetos da cena (vamos colocar os objetos aqui para poder ter acesso global)
	vector<ObjectArray> *objects = scene->getSeneObjects();
	vector<Vector3f> vertices;
	vector<Vector2f> texture;
	vector<Vector3f> colors;


	ObjectArray::getPlane(vertices,texture, colors);

	// ObjectArray objetoCorEstatica;
	// objetoCorEstatica.create(vertices, vtsVerticePtr);
	// objetoCorEstatica.setPosition(Vector3f(-7.5f, 1.0f, -3.0f));
	// objetoCorEstatica.setColor(Vector3f(0.0f, 0.8f, 0.2f));
	// scene->getSeneObjects()->push_back(objetoCorEstatica);


	// ObjectArray objetoCorVertice;
	// objetoCorVertice.create(vertices, vtsVerticePtr, vector<Vector2f>(0), NULL_SHADER_POINTER, colors, vtsColorPtr);
	// objetoCorVertice.setPosition(Vector3f(-5.0f, 1.0f, -3.0f));
	// scene->getSeneObjects()->push_back(objetoCorVertice);



	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaFundacao", Vector3f(0.0f,0.0f,0.0f));

	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaFundacao2", Vector3f(0.0f,0.0f,0.0f));

	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaParedes", Vector3f(0.0f,0.0f,0.0f));

	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaPiso", Vector3f(0.0f,0.0f,0.0f));
	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaPiso2", Vector3f(0.0f,0.0f,0.0f));

	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaEscada", Vector3f(0.0f,0.0f,0.0f));
	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaEscrivaninha", Vector3f(0.0f,0.0f,0.0f));

	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaEstoque", Vector3f(0.0f,0.0f,0.0f));
	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaEstoque1", Vector3f(0.0f,0.0f,0.0f));
	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaEstoque2", Vector3f(0.0f,0.0f,0.0f));
	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaEstoque3", Vector3f(0.0f,0.0f,0.0f));
	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaEstante", Vector3f(0.0f,0.0f,0.0f));
	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaParedes2", Vector3f(0.0f,0.0f,0.0f));


	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaMesa", Vector3f(0.0f,0.0f,0.0f));

	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaCadeira", Vector3f(0.0f,0.0f,0.0f));
	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaBanco", Vector3f(0.0f,0.0f,0.0f));
	ReadPLYWtexture(objects,&vtsVerticePtr,&vtsTexturePtr,"TrabalhoCasaTeto", Vector3f(0.0f,0.0f,0.0f));

	ObjectArray objetoTextura1;
	objetoTextura1.create(vertices, vtsVerticePtr, texture, vtsTexturePtr);
	objetoTextura1.setPosition(Vector3f(-2.5f, 0.2f, -3.0f));
	objetoTextura1.setTexture("../textures/MinecraftGrass.png", true, true, true);
	
	objetoTextura1.setScale(Vector3f(0.5f,0.5f,0.5f) );
	scene->getSeneObjects()->push_back(objetoTextura1);

	ObjectArray objetoTextura2;
	objetoTextura2.create(vertices, vtsVerticePtr, texture, vtsTexturePtr);
	objetoTextura2.setPosition(Vector3f(-2.5f, 0.2f, -3.0f));
	objetoTextura2.setTexture("../textures/MinecraftGrass.png", true, true, true);
	objetoTextura2.setRotation(Math::yRotationMat(1.57));
	objetoTextura2.setScale(Vector3f(0.5f,0.5f,0.5f));
	scene->getSeneObjects()->push_back(objetoTextura2);
	// ObjectArray objetoTextura2;
	// objetoTextura2.create(vertices, vtsVerticePtr, texture, vtsTexturePtr);
	// objetoTextura2.setPosition(Vector3f(0.0f, 1.0f, -3.0f));
	// objetoTextura2.setTexture("../textures/maca.jpg", true, false, false);
	// scene->getSeneObjects()->push_back(objetoTextura2);


	// ObjectArray objetoTexturaCor;
	// objetoTexturaCor.create(vertices, vtsVerticePtr, texture, vtsTexturePtr, colors, vtsColorPtr);
	// objetoTexturaCor.setPosition(Vector3f(2.5f, 1.0f, -3.0f));
	// objetoTexturaCor.setTexture("../textures/maca.jpg", true, false, false);
	// scene->getSeneObjects()->push_back(objetoTexturaCor);


	// ObjectArray objetoMultiTextura;
	// objetoMultiTextura.create(vertices, vtsVerticePtr, texture, vtsTexturePtr);
	// objetoMultiTextura.setPosition(Vector3f(5.0f, 1.0f, -3.0f));
	// objetoMultiTextura.setTexture("../textures/maca.jpg", "../textures/maca2.jpg", true, false, false);
	// scene->getSeneObjects()->push_back(objetoMultiTextura);


	// vector<Vector3f> floorVert;
	// vector<Vector2f> floorText;
	// ObjectArray::getFloor(floorVert,floorText);
	// ObjectArray objetoChao;
	// objetoChao.create(floorVert, vtsVerticePtr, floorText, vtsTexturePtr);
	// objetoChao.setPosition(Vector3f(0.0f, -0.2f, 0.0f));
	// objetoChao.setTexture("../texture/chao.jpg", true, false, false);
	// // objetoChao.setTexture("../texture/chao.jpg", true, true, false);
	// // objetoChao.setTexture("../texture/chao.jpg", true, true, true);
	// scene->getSeneObjects()->push_back(objetoChao);

	


	
	//LOOP de renderização. A cada passada, um quadro é renderizo!
	while (isOpenWindow()) {

		beforeRender();
		
		render();

		afterRender();
		
	} //fim da renderização
	
	//remove VAO e VBOs alocados para os objetos
	for(size_t i=0; i<objects->size(); i++)
		objects->at(i).deleteAll();
		

	//remove os shaders
	for(size_t i=0; i<scene->getSeneShaderPrograms()->size(); i++)
		glDeleteProgram( scene->getSeneShaderPrograms()->at(i) );

		
	//remove a janela 
	if(scene->getGlfwWindow())
		glfwDestroyWindow(scene->getGlfwWindow());
	glfwTerminate();

	//remove a cena
	Scene::deleteInstance();

	return EXIT_SUCCESS;


} // fim main()

void keyEvent(GLFWwindow* win, int key, int scancode, int action, int mods){

	if (key == GLFW_KEY_PRINT_SCREEN && action == GLFW_PRESS){
	
		Screenshot::saveScreenshot("../screenshot/",  Scene::getInstance()->getWinWidth(), Scene::getInstance()->getWinHeight());
	}
	if(key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS){
		if(Scene::getInstance()->getCamera()->getAtivarCamera()){
			glfwSetInputMode(win,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
			Scene::getInstance()->getCamera()->setAtivarCamera(false);
		}else{
			glfwSetInputMode(win,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
			Scene::getInstance()->getCamera()->setAtivarCamera(true);
		}
	}
	
}

void renderkeyEvent(){

	GLFWwindow *win = Scene::getInstance()->getGlfwWindow();
	Camera *cam = Scene::getInstance()->getCamera();

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS){
		if(cam->getAtivarCamera())
			return
		cam->moveForward();
		// Matrix4f R = objects->at(ID).getTranslation();
		// // Matrix4f R = objects->at(ID).getRotation() * Math::yRotationMat(-0.01f);
		
		// objects->at(ID).setRotation(R);
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS){
		// Matrix4f R = objects->at(ID).getRotation() * Math::yRotationMat(-0.01f);
		// objects->at(ID).setRotation(R);
		if(cam->getAtivarCamera())
			return
		cam->moveBackward();
	}
	if(glfwGetKey(win,GLFW_KEY_D)==GLFW_PRESS){
		if(cam->getAtivarCamera())
			return
		cam->moveRight();
	}
	if(glfwGetKey(win,GLFW_KEY_A)==GLFW_PRESS){
		if(cam->getAtivarCamera())
			return
		cam->moveLeft();
	}
	if(glfwGetKey(win,GLFW_KEY_SPACE)==GLFW_PRESS){
		if(cam->getAtivarCamera())
			return
		cam->moveUp();
	}
	if(glfwGetKey(win,GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS){
		if(cam->getAtivarCamera())
			return
		cam->moveDown();
	}

	// if(glfwGetKey(win,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS){
	// 	if(cam->getAtivarCamera())
	// 		return;
	// 	Scene::getInstance()->getCamera()->setFactor(Scene::getInstance()->getCamera()->getFactor() * 10.0f);
	// }

}


void guiNewFrame(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


void guiRender(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void deleteWindow(){
	//remove a janela 
	GLFWwindow* window = Scene::getInstance()->getGlfwWindow();

	if(window)
		glfwDestroyWindow(window);
	glfwTerminate();
}

bool isOpenWindow(){
	GLFWwindow* window = Scene::getInstance()->getGlfwWindow();
	return (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);
}

void beforeRender(){
	//Limpa os buffers que armazenam os valores de pronfundidade de cada fragmento para seu valor inicial (1.0)
	//Caso isso não seja feito, os fragmentos de novos triângulos serão comparados com os fragmentos renderizados do quadro anterior!
	// glClear(GL_DEPTH_BUFFER_BIT);

	// //Limpa os buffers que armazenam os valores de cor
	// glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double lastTime = Scene::getInstance()->getLastTime();
	double RTF = glfwGetTime() - lastTime;
	lastTime= glfwGetTime();
	Scene::getInstance()->setRTF(RTF);
	Scene::getInstance()->setLastTime(lastTime);

}

void afterRender(){
	
	//std::cout << "glEndDraw(): Endereco de GLFWwindow: " << reinterpret_cast<unsigned long long>(Scene::getInstance()->getGlfwWindow()) << std::endl;
	//processa os eventos de teclado e mouse
	glfwPollEvents();

	//atualiza a janela de visualização com o quadro renderizado
	glfwSwapBuffers(Scene::getInstance()->getGlfwWindow());
}



void render(){
	

	Scene *scene = Scene::getInstance();
	static bool wireframe = false;
	static float mixFactor = 0.5f;
	static float movFactor = 1.0f;
	static float rotFactor = 1.0f;
	Camera* cam = Scene::getInstance()->getCamera();


	Matrix4f PV  = cam->getProjectionMatrix() * cam->getViewMatrix(); //matriz view -> projeção + rotacao + posicionamento da câmera
	Matrix4f matPVM, ObjTrans, R;
	GLuint matPVMRef, corRef, mixFactorRef, program;


	if(wireframe)
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );





	//DRAW -> todos os tipos de objetos!
	{
		
		std::vector<ObjectArray> *objects = Scene::getInstance()->getSeneObjects();
		for(size_t i = 0; i<objects->size(); i++){
			
			
			if(objects->at(i).getType()==COORD){
				program = Scene::getInstance()->getSeneShaderPrograms()->at(COORD);
				glUseProgram(program);

				corRef = glGetUniformLocation(program, "color");
				glUniform3fv(corRef, 1, objects->at(i).getColor().data());
			}
			else if(objects->at(i).getType()==COORD_COLOR){
				program = Scene::getInstance()->getSeneShaderPrograms()->at(COORD_COLOR);
				glUseProgram(program);
			}
			else if(objects->at(i).getType()==COORD_TEXT_BLEND){
				program = Scene::getInstance()->getSeneShaderPrograms()->at(COORD_TEXT_BLEND);
				glUseProgram(program);
			}
			else if(objects->at(i).getType()==COORD_TEXT){
				program = Scene::getInstance()->getSeneShaderPrograms()->at(COORD_TEXT);
				glUseProgram(program);
			}
			else if(objects->at(i).getType()==COORD_TEXT_COLOR){
				program = Scene::getInstance()->getSeneShaderPrograms()->at(COORD_TEXT_COLOR);
				glUseProgram(program);
			}
			else if(objects->at(i).getType()==COORD_MULTI_TEXT){
				program = Scene::getInstance()->getSeneShaderPrograms()->at(COORD_MULTI_TEXT);
				glUseProgram(program);

				mixFactorRef = glGetUniformLocation(program, "mixFactor");
				glUniform1f(mixFactorRef, mixFactor);
				
			}


			matPVMRef = glGetUniformLocation(program, "PVM");
			matPVM = PV * objects->at(i).getTranslation() * objects->at(i).getRotation() * objects->at(i).getScaleMat();
			glUniformMatrix4fv(matPVMRef, 1, GL_FALSE, matPVM.data());
			objects->at(i).render();

		}
		

	}
	
	//eventos do teclado
	renderkeyEvent();

	//IMGUI
	{
		guiNewFrame(); //novo frame (um único frame, mesmo que varias janelas sejam criadas)

		ImGuiWindowFlags somenteTexto; //flags
		somenteTexto = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoScrollbar  | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize;
		//somenteTexto = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		//somenteTexto |= ImGuiWindowFlags_NoScrollbar  | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize;
		//somenteTexto |= ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs


		if(!Scene::getInstance()->getCamera()->getAtivarCamera()){

		ImGui::Begin("Título da janela", nullptr, somenteTexto); //cria uma janela
		ImGui::SetWindowPos(ImVec2(0,0)); //posição
		ImGui::Text("FPS: %.1f",  ImGui::GetIO().Framerate);
		ImGui::SameLine();
		ImGui::Text("RTF: %.0001",Scene::getInstance()->getRTF() * 100.0f);
		ImGui::Text(" ALT to release mouse, W,A,S,D to move, Space and CTRL to height of camera.");
		if (ImGui::Button("wireframe"))   
			wireframe = !wireframe;
		ImGui::SliderFloat("Mix Factor", &mixFactor, 0.0f, 1.0f);
		ImGui::SliderFloat("Camera Mov Speed", &movFactor, 0.0f, 100.0f);
		ImGui::SliderFloat("Camera Rot Speed", &rotFactor, 0.0f, 10.0f);
		ImGui::End(); //finaliza a criação da janela
		
		}
		


		Scene::getInstance()->getCamera()->setFactor(  movFactor * 100.0f * Scene::getInstance()->getRTF());
		Scene::getInstance()->getCamera()->setRotFactor( rotFactor * 0.01f  * Scene::getInstance()->getRTF());

		guiRender(); //renderiza todas as janelas criadas
	}

} //fim render()

void initOpenGL(){
	
	//pega a instância global da classe cena
	Scene *scene = Scene::getInstance();


	if (!glfwInit())
		EXCEPTION("Falha ao executar glfwInit()");
	
	//define que a versão minímina da GLSL para executar este programa
	std::string glslVersion = Shader::shaderProcessorVersion(scene->getGlslMajorVersion(), scene->getGlslMinorVersion());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, scene->getGlslMajorVersion());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, scene->getGlslMinorVersion());
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	if(scene->isFullscreen()){ //tela cheia ?
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		//pega informacoes do monitor
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
 		
		//cout << mode->redBits << endl;
		//cout << mode->greenBits << endl;
		//cout << mode->blueBits << endl;
		//cout << mode->refreshRate << endl << endl;

		window = glfwCreateWindow(mode->width, mode->height, scene->getWinTitle().c_str(), glfwGetPrimaryMonitor(), NULL); //tela cheia
	}
	else
		window = glfwCreateWindow(scene->getWinWidth(), scene->getWinHeight(), scene->getWinTitle().c_str(), NULL, NULL);
	
	
	//veririca se a janela foi criada com sucesso
	if (!window)
    {
        glfwTerminate();
        EXCEPTION("Falha na criacao da janela: glfwCreateWindow() ");
    }


	//atribui a janela a cena para poder ter acesso global
	scene->setGlfwWindow(window);

	//cria o contexto de renderização
    glfwMakeContextCurrent(window);


	//inicializa a GleW
	if (glewInit() != GLEW_OK)
		EXCEPTION("Falha glewInit() ");

	//VSYNC
	//Monitor de 240HZ -> 0= DESATIVADO, 1= 240FPS, 2=120FPS, 3=80FPS
	if(scene->isVSYNC())
		glfwSwapInterval(1); 
	else
		glfwSwapInterval(0); 


	//registra a callback de evento de teclado
	glfwSetKeyCallback(window, keyEvent);

	glfwSetCursorPosCallback(window, mouseEvent);

	glfwSetInputMode(scene->getGlfwWindow(),GLFW_CURSOR,GLFW_CURSOR_DISABLED);

	//ativar o teste de profundidade para cada fragmento processado dentro do fragment shader
	//antes de cada fragmento ser desenhado é realizada uma comparação com o valor de profundidade
	glEnable(GL_DEPTH_TEST);

	//define qual algoritmo sera usado para comparar os valores de profundidade dentro do fragment shader
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDepthFunc.xhtml
	//glDepthFunc(GL_LEQUAL);
	// glDepthFunc(GL_LESS);


	//IMGUI -> inicializar
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //ImGui::StyleColorsDark(); // cor da janela
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true); //janela de renderização
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());

} //fim initOpenGL()




void mouseEvent(GLFWwindow* window, double xpos, double ypos){
	// cout<<"["<<xpos<<", "<<ypos<<"]"<<endl;

	Scene::getInstance()->getCamera()->rotate(xpos,ypos);


}

void ReadPLYWtexture(std::vector<ObjectArray> *objects, unsigned int *vtsVerticePtr, int *vtsTexturePtr, string nomeImagem,Vector3f position ){
	float dadosV[8];
	float dadosF[4];
	int nVertex;
	int nFaces;
	int numberProperties=0;
	string parameter;
	vector<Vector3f>auxV;
	vector<Vector2f>auxT;
	vector<Vector3i>faces;
	vector<Vector3f>vertices;
	vector<Vector2f>texture;
	string nomeFile ="../models/"+ nomeImagem + ".ply";
	cout<<"nomeFile: "<<nomeFile<<endl;
	streambuf *cinbuf = cin.rdbuf();
	ifstream file(nomeFile);

	if(file.is_open()){
		cin.rdbuf(file.rdbuf());
	}

	while(true){
		cin>> parameter;
		
		if(!cin)
			break;
		if(parameter.size() > 0 && parameter == "comment"){
			string aux;
			getline(cin,aux);
		}
		if( parameter == "property"){
			numberProperties++;
			string aux;
			getline(cin,aux);
		}
		if( parameter == "element"){
			cin>>parameter;
			if(parameter == "vertex"){
				cin>>nVertex;
			}else if (parameter == "face"){
				cin>>nFaces;
			}
		}
		if(parameter == "end_header"){
			int contador=nVertex;
			while(contador>0){
				for(int i=0;i<numberProperties-1;i++){
					cin>>dadosV[i];
				}
				auxV.push_back(Vector3f(dadosV[0],dadosV[1],dadosV[2]));
				auxT.push_back(Vector2f(dadosV[6],dadosV[7]));
				contador--;
			}
			while(nFaces>0){	
				for(int i =0;i<4;i++){
					cin>>dadosF[i];
					if(i>0){
						vertices.push_back(auxV[dadosF[i]]);
						texture.push_back(auxT[dadosF[i]]);
					}
				}
				nFaces--;
			}
		}
	}

	file.close();
	std::cin.rdbuf(cinbuf);
	

	
	// create the object
	// ObjectPLY objPLYWcolor;
	// objPLYWcolor.create(vertices, *vtsVerticePtr,*vtsColorPtr,cor);
	// objPLYWcolor.setPosition(position);
	// if(nomeImagem=="mercury.ply"){
	// 	objPLYWcolor.setEscale(Vector3f(0.4f,0.4f,0.4f));
	// }
	// objects->push_back(objPLYWcolor); //add 

	string textureName = "../textures/" +nomeImagem+".png";
	// bool hasAlpha = false;
	ObjectArray objetoTextura;
	objetoTextura.create(vertices, *vtsVerticePtr, texture, *vtsTexturePtr);
	objetoTextura.setPosition(position);
	objetoTextura.setTexture(textureName, true, true, true);
	objects->push_back(objetoTextura);

};
