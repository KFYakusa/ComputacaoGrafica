
#include <iostream>
#include <iomanip> 
#include <string> 
#include <vector>
#include <streambuf>

#include <Eigen/Dense>

#include <GL/glew.h> //GLEW
#include <GLFW/glfw3.h> //GLFW

#include "Scene.h"
#include "Shader.h"
#include "Math.h"

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

// Read the PLY image and return a Object, (object list, pointer to Vert GPU variable, point to GPU Color Variable,  Image name, Position )
void ReadPLY(std::vector<ObjectPLY> *objects, GLuint *vtsVerticePtr, GLint *vtsColorPtr, string nomeImagem,Vector3f position){
	float dadosV[12];
	float dadosF[4];
	int nVertex;
	int nFaces;
	string parameter;
	vector<Vector3f>auxV;
	vector<Vector3f>auxC;
	vector<Vector3i>faces;
	vector<Vector3f>vertices;
	vector<Vector3f> cor;
	streambuf *cinbuf = cin.rdbuf();
	ifstream file(nomeImagem);

	if(file.is_open()){
		cin.rdbuf(file.rdbuf());
	}

	while(true){
		cin>> parameter;
		cout<< parameter;
		if(!cin)
			break;
		if(parameter.size() > 0 && parameter == "comment"){
			string aux;
			getline(cin,aux);
			cout<<endl<<aux<<endl;
		}
		if(parameter.size() > 0 && parameter == "element"){
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
				for(int i=0;i<12;i++){
					cin>>dadosV[i];
				}
				auxV.push_back(Vector3f(dadosV[0],dadosV[1],dadosV[2]));
				auxC.push_back(Vector3f(dadosV[8]/255.0f,dadosV[9]/255.0f,dadosV[10]/255.0f));
				contador--;
			}
			while(nFaces>0){	
				for(int i =0;i<4;i++){
					cin>>dadosF[i];
					if(i>0){
						vertices.push_back(auxV[dadosF[i]]);
						cor.push_back(auxC[dadosF[i]]);
					}
				}
				nFaces--;
			}
		}
	}

	file.close();
	std::cin.rdbuf(cinbuf);

		// create the object
	ObjectPLY obj1;
	obj1.create(vertices, *vtsVerticePtr,*vtsColorPtr,cor);
	obj1.setPosition(position);
	objects->push_back(obj1); //add 
};


int main(void) {
	float dadosVertices[12];
	float dadosFaces[4];
	vector<Vector3f> auxV;
	vector<Vector3f> faces;
	vector<Vector3f> vertices;
	vector<Vector3f> corVertices;


	//1 - INICIAR CENA, fazer isso antes de inicializar o OPENGL!
	Scene *scene = Scene::getInstance();
	scene->setFullscreen(false); //tela cheia?
	scene->setWinWidth(1024); //dimensão da janela
	scene->setWinHeight(768);
	scene->setWinTitle("ExercícioPLY");
	scene->setGlslMajorVersion(4);  //GLSL 4.3
	scene->setGlslMinorVersion(3);
	scene->setVSYNC(true);
	
	//2 - INICIAR OPENGL -> execute esta função sempre depois de inicializar a cena!
	initOpenGL();


	//https://www.khronos.org/opengl/wiki/Layout_Qualifier_(GLSL)
	//ponteiros para as variaveis de entrada do Vertex Shader
	//o valor do ponteiro é o valor especificado para LOCATION no Vertex Shader!
	GLuint vtsVerticePtr = 0; //layout (location=0)
	GLint vtsColorPtr = 1; //layout (location=1)
	
	//cria o programa e os shaders: vertex shader e fragment shader
	unsigned int program;
	if (!Shader::createProgram(&program, "vertShader.glsl", "fragShader.glsl")) { //ocorreu erro na criação do programa ??
		deleteWindow();
		EXCEPTION( "Erro ao criar programa!");
	}

	//atribui o programa a cena. Assim, temos acesso global a este ponteiro!
	scene->getSeneShaderPrograms()->push_back(program);

	//CÂMERA
	//Matriz de projeção perspectiva e posicionamento
	float FOV = Math::degree2rad(60.0f);
	float zNear = 0.1f;
	float zFar = 500.0f;
	float aspectRatio = float(scene->getWinWidth()) / float(scene->getWinHeight());
	Matrix4f projectionMat = Math::perspectiveProjMat(FOV, aspectRatio, zNear, zFar);
	Vector3f posCamera = Vector3f(0.0f, 2.0f, 10.0f);

	//atribui a matriz de projeção e posição da camera a cena
	scene->getCamera()->create(projectionMat, posCamera);

	//pega o vetor de objetos da cena (vamos colocar os objetos aqui para poder ter acesso global)
	std::vector<ObjectPLY> *objects = scene->getSeneObjects();
	
	//cria os vértices dos objetos cubo, torus e plano
	ReadPLY(objects,&vtsVerticePtr, &vtsColorPtr,"cubo.ply" ,Vector3f(-2.3f, 0.0f, 0.0));
	ReadPLY(objects,&vtsVerticePtr, &vtsColorPtr,"torus.ply",Vector3f(2.3f, 0.0f, 0.0) );
	ReadPLY(objects,&vtsVerticePtr, &vtsColorPtr,"plano.ply",Vector3f(0.0f, 2.0f, 0.0) );
	
	// vector<Vector3f> pyramidVertices = ObjectPLY::getPyramidVertices();
	// ObjectPLY obj1;
	// obj1.create(pyramidVertices, vtsVerticePtr);
	// obj1.setPosition(Vector3f(-2.3f, 0.0f, 0.0));
	// obj1.setColor(Vector3f(1.0f, 1.0f, 0.0));
	// objects->push_back(obj1); //add 
	
	// ObjectPLY obj2;
	// obj2.create(cubeVertices, vtsVerticePtr);
	// obj2.setPosition(Vector3f(0.0f, 0.0f, -3.0));
	// obj2.setColor(Vector3f(0.0f, 0.0f, 1.0));
	// objects->push_back(obj2); //add 

	// ObjectPLY obj3;
	// obj3.create(cubeVertices, vtsVerticePtr);
	// obj3.setPosition(Vector3f(2.3f, 0.0f, 0.0));
	// obj3.setColor(Vector3f(1.0f, 0.0f, 0.0));
	// objects->push_back(obj3); //add 
	
	//LOOP de renderização. A cada passada, um quadro é renderizo!
	while (isOpenWindow()) {

		beforeRender();
		
		render();

		afterRender();
		
	} //fim da renderização
	
	//remove VAO e VBOs alocados para os objetos
	for(size_t i=0; i<objects->size(); i++)
		objects->at(i).deleteVAOVBOs();
		

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

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS){
		Scene::getInstance()->setCurrentObjectID(0);
	}
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS){
		Scene::getInstance()->setCurrentObjectID(1);
	}
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS){
		Scene::getInstance()->setCurrentObjectID(2);
	}
	
}

void renderkeyEvent(){

	GLFWwindow *win = Scene::getInstance()->getGlfwWindow();
	int ID = Scene::getInstance()->getCurrentObjectID();
	std::vector<ObjectPLY> *objects = Scene::getInstance()->getSeneObjects();

	if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS){
		Matrix4f R = objects->at(ID).getRotation() * Math::xRotationMat(0.001f);
		objects->at(ID).setRotation(R);
	}
	if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS){
		Matrix4f R = objects->at(ID).getRotation() * Math::zRotationMat(-0.001f);
		objects->at(ID).setRotation(R);
	}

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
	glClear(GL_DEPTH_BUFFER_BIT);

	//Limpa os buffers que armazenam os valores de cor
	glClear(GL_COLOR_BUFFER_BIT);
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
	Camera* cam = Scene::getInstance()->getCamera();

	Matrix4f tranMat = Math::translationMat(- cam->getPosition()); //translação inversa!
	Matrix4f perspectiveProjMat = cam->getProjectionMatrix(); //matriz de projeção
	Matrix4f PV  = perspectiveProjMat * tranMat; //matriz view -> projeção + posicionamento da câmera
	Matrix4f matPVM, ObjTrans, R;
	GLuint matPVMRef, corRef, program;

	if(wireframe)
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	
	//DRAW -> agora vamos desenhar toda a cena
	{
		std::vector<ObjectPLY> *objects = Scene::getInstance()->getSeneObjects();
		program = Scene::getInstance()->getSeneShaderPrograms()->at(0);
		//especifica os shaders que serão usados na renderização
		glUseProgram(program);
		
		//le o endereço de memória das variáveis do vertexshader: cor e PVM
		matPVMRef = glGetUniformLocation(program, "PVM");
		corRef = glGetUniformLocation(program, "cor");


		//animação do objeto atual
		int currentObjectID = Scene::getInstance()->getCurrentObjectID();
		R = objects->at(currentObjectID).getRotation() * Math::xRotationMat(0.01f);
		objects->at(currentObjectID).setRotation(R);


		//renderização de todos os objetos
		for(size_t i = 0; i<objects->size(); i++){
			
			matPVM = PV * objects->at(i).getTranslation() * objects->at(i).getRotation();	

			glUniformMatrix4fv(matPVMRef, 1, GL_FALSE, matPVM.data());
			glUniform3fv(corRef, 1, objects->at(i).getColor().data());
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

		ImGui::Begin("Título da janela", nullptr, somenteTexto); //cria uma janela
		ImGui::SetWindowPos(ImVec2(0,0)); //posição
		ImGui::Text("FPS: %.1f",  ImGui::GetIO().Framerate);
		ImGui::SameLine();
		if (ImGui::Button("wireframe"))   
			wireframe = !wireframe;
		ImGui::End(); //finaliza a criação da janela

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

	//ativar o teste de profundidade para cada fragmento processado dentro do fragment shader
	//antes de cada fragmento ser desenhado é realizada uma comparação com o valor de profundidade
	glEnable(GL_DEPTH_TEST);

	//define qual algoritmo sera usado para comparar os valores de profundidade dentro do fragment shader
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDepthFunc.xhtml
	//glDepthFunc(GL_LEQUAL);
	glDepthFunc(GL_LESS);


	//IMGUI -> inicializar
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //ImGui::StyleColorsDark(); // cor da janela
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true); //janela de renderização
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());

} //fim initOpenGL()



