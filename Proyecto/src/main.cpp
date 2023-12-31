#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>
#include <string.h>

//std includes
#include <string>
#include <iostream>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

// Font rendering include
#include "Headers/FontTypeRendering.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// ShadowBox include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

#include <windows.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

GLFWwindow *window;


//*******************Declaracion de Shaders
Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
// Shader para dibujar un objeto con solo textura
Shader shaderTexture;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
// Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
//Shader para las particulas de fuego
Shader shaderParticlesFire;
//Shader para las particulas de Smoke
Shader shaderParticlesSmoke;
//Shader particulas lluvia
Shader shaderParticlesLluvia;

//************Declaracion de Camaras. Primera Persona y Tercera persona

std::shared_ptr<FirstPersonCamera> camera1(new FirstPersonCamera());
std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 10.0;

//************Inicializacion de Cajas y esferas de tipo Box y Sphere
Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Sphere esfera1(10, 10);
Box boxCollider;
Sphere sphereCollider(10, 10);
Cylinder rayModel(10, 10, 1.0, 1.0, 1.0);
Box boxIntro;
Box boxViewDepth;
Box boxLightViewBox;

// Declaracion de MODELOS VARIOS
// Models complex instances
Model modelRock;
Model modelAircraft;
Model modelEclipseChasis;
Model modelEclipseRearWheels;
Model modelEclipseFrontalWheels;
Model modelHeliChasis;
Model modelHeliHeli;
Model modelHeliHeliBack;
Model modelLambo;
Model modelLamboLeftDor;
Model modelLamboRightDor;
Model modelLamboFrontLeftWheel;
Model modelLamboFrontRightWheel;
Model modelLamboRearLeftWheel;
Model modelLamboRearRightWheel;
// Dart lego
Model modelDartLegoBody;
Model modelDartLegoHead;
Model modelDartLegoMask;
Model modelDartLegoLeftArm;
Model modelDartLegoRightArm;
Model modelDartLegoLeftHand;
Model modelDartLegoRightHand;
Model modelDartLegoLeftLeg;
Model modelDartLegoRightLeg;

// Buzz
Model modelBuzzTorso;
Model modelBuzzHead;
Model modelBuzzLeftArm;
Model modelBuzzLeftForeArm;
Model modelBuzzLeftHand;
//Hierba
Model modelGrass;


//modelos de lÃƒÂ¡mparas
Model modelLamp1;
Model modelLamp2;
Model modelLamp2Post;
Model modelLamp3;
Model modelLamp4;
// Modelos animados
// Luffy
Model luffyModelAnimate;
// Cowboy
Model cowboyModelAnimate;
// Guardian con lampara
Model guardianModelAnimate;
// Cybog
Model zombiModelAnimate;
//Enemy
Model enemyModelAnimate;
// Fountain
Model modelFountain;
//Fruta puntos
Model modelFruta;


Model modelArbol1a;
Model modelArbol1b;
Model modelArbol1c;

Model modelArbol2a;
Model modelArbol2b;
Model modelArbol2c;

Model modelArbol3a;
Model modelArbol3b;

Model modelAtaud1a;
Model modelAtaud1b;
Model modelAtaud1c;
Model modelAtaud1d;

Model modelAtaud2a;
Model modelAtaud2b;
Model modelAtaud2c;
Model modelAtaud2d;

Model modelArco;

Model modelZombi_1a;
Model modelZombi_1b;
Model modelZombi_1c;
Model modelZombi_1d;

Model modelZombi_2a;
Model modelZombi_2b;
Model modelZombi_2c;

Model modelZombi_3a;
Model modelZombi_3b;
Model modelZombi_3c;






// Terrain model instance
//Terrain terrain(-1, -1, 200, 8, "../Textures/heightmap.png");
Terrain terrain(-1, -1, 200, 8, "../Textures/alturas_nuevo.png"); //nuevo mapa de alturas
//Terrain terrain(-1, -1, 200, 8, "../Textures/Texture_Set_Vol_64_Grass_png/Vol_64_5_Height.png");
//Terrain terrain(-1, -1, 400, 8, "../Textures/mapperAlturas2.png");

//************ Declaracion de VARIABLES de TEXTURAS, incluso de las texturas animadas
ShadowBox * shadowBox;

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;
GLuint textureInit1ID, textureInit2ID, textureActivaID, textureScreenID,
		textureScreen1ID, textureScreen2ID, textureScreen3ID, textureScreen4ID, textureScreen5ID;
GLuint textureParticleFountainID,textureParticleFireID, texId;


//*********** DEclaraciÃƒÂ³n de Variables de control de Boton de la pantalla de inicio
bool iniciaPartida = false, presionarOpcion = false;

// Modelo para el render del texto. Para el Texto en Pantalla
FontTypeRendering::FontTypeRendering *modelText;

//*****************DeclaraciÃƒÂ³n de elementos para CUBEMAP
GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };


//************** carga de texturas para CUBEMAP*************
std::string fileNames[6] = { "../Textures/ulukai/corona_ft.tga",
		"../Textures/ulukai/corona_bk.tga",
		"../Textures/ulukai/corona_up.tga",
		"../Textures/ulukai/corona_dn.tga",
		"../Textures/ulukai/corona_rt.tga",
		"../Textures/ulukai/corona_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

//***************Definiciones e iniciaciÃƒÂ³n de los Modelos Matrices de los modelos  
//Se definen los modelos de matrices a manejar en el escenario
// Model matrix definitions
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixAircraft = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
glm::mat4 modelMatrixBuzz = glm::mat4(1.0f);
glm::mat4 modelMatrixLuffy = glm::mat4(1.0f);  //<<<<<<<<<<<<LUFFY
glm::mat4 modelMatrixCowboy = glm::mat4(1.0f);
glm::mat4 modelMatrixGuardian = glm::mat4(1.0f);
glm::mat4 modelMatrixZombi = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemy = glm::mat4(1.0f);  // <<<<<ENEMY
glm::mat4 modelMatrixFountain = glm::mat4(1.0f);
glm::mat4 modelMatrixFruta = glm::mat4(1.0f);
glm::mat4 modelMatrixFruta2 = glm::mat4(1.0f);
glm::mat4 modelMatrixFruta3 = glm::mat4(1.0f);
glm::mat4 modelMatrixFruta4 = glm::mat4(1.0f);
glm::mat4 modelMatrixFruta5 = glm::mat4(1.0f);
glm::mat4 modelMatrixFruta6 = glm::mat4(1.0f);
glm::mat4 modelMatrixFruta7 = glm::mat4(1.0f);
glm::mat4 modelMatrixFruta8 = glm::mat4(1.0f);
glm::mat4 modelMatrixFruta9 = glm::mat4(1.0f);


glm::mat4 modelMatrixArbol1a = glm::mat4(1.0f);
glm::mat4 modelMatrixArbol1b = glm::mat4(1.0f);
glm::mat4 modelMatrixArbol1c = glm::mat4(1.0f);


glm::mat4 modelMatrixArbol2a = glm::mat4(1.0f);
glm::mat4 modelMatrixArbol2b = glm::mat4(1.0f);
glm::mat4 modelMatrixArbol2c = glm::mat4(1.0f);

glm::mat4 modelMatrixArbol3a = glm::mat4(1.0f);
glm::mat4 modelMatrixArbol3b = glm::mat4(1.0f);


glm::mat4 modelMatrixAtaud1a = glm::mat4(1.0f);
glm::mat4 modelMatrixAtaud1b = glm::mat4(1.0f);
glm::mat4 modelMatrixAtaud1c = glm::mat4(1.0f);
glm::mat4 modelMatrixAtaud1d = glm::mat4(1.0f);

glm::mat4 modelMatrixAtaud2a = glm::mat4(1.0f);
glm::mat4 modelMatrixAtaud2b = glm::mat4(1.0f);
glm::mat4 modelMatrixAtaud2c = glm::mat4(1.0f);
glm::mat4 modelMatrixAtaud2d = glm::mat4(1.0f);

glm::mat4 modelMatrixArco = glm::mat4(1.0f);

glm::mat4 modelMatrixZombi_1a = glm::mat4(1.0f); 
glm::mat4 modelMatrixZombi_1b = glm::mat4(1.0f); 
glm::mat4 modelMatrixZombi_1c = glm::mat4(1.0f); 
glm::mat4 modelMatrixZombi_1d = glm::mat4(1.0f); 
glm::mat4 modelMatrixZombi_2a = glm::mat4(1.0f); 
glm::mat4 modelMatrixZombi_2b = glm::mat4(1.0f); 

glm::mat4 modelMatrixZombi_2c = glm::mat4(1.0f); 
glm::mat4 modelMatrixZombi_3a = glm::mat4(1.0f); 
glm::mat4 modelMatrixZombi_3b = glm::mat4(1.0f); 
glm::mat4 modelMatrixZombi_3c = glm::mat4(1.0f); 



int sumaColisionOBB_OBB = 0;
int puntuacion =0;
int puntuacionTotal = 0;
bool gameOver = false;
//char puntuacion1[20] = "100 puntos", puntuacion2[20]= "200 puntos", puntuacion3[20]= "300 puntos", 
//		puntuacion4[20]="400 puntos", puntuacion5[20]= "500 puntos";



//********* Variables para manejar Animacion por KEYFRAME desde OPENGL
float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
float rotBuzzHead = 0.0, rotBuzzLeftarm = 0.0, rotBuzzLeftForeArm = 0.0, rotBuzzLeftHand = 0.0;
int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Joints interpolations Dart Lego
std::vector<std::vector<float>> keyFramesDartJoints;
std::vector<std::vector<glm::mat4>> keyFramesDart;
int indexFrameDartJoints = 0;
int indexFrameDartJointsNext = 1;
float interpolationDartJoints = 0.0;
int maxNumPasosDartJoints = 20;
int numPasosDartJoints = 0;
int indexFrameDart = 0;
int indexFrameDartNext = 1;
float interpolationDart = 0.0;
int maxNumPasosDart = 200;
int numPasosDart = 0;

// Joints interpolations Buzz
std::vector<std::vector<float>> keyFramesBuzzJoints;
std::vector<std::vector<glm::mat4>> keyFramesBuzz;
int indexFrameBuzzJoints = 0;
int indexFrameBuzzJointsNext = 1;
float interpolationBuzzJoints = 0.0;
int maxNumPasosBuzzJoints = 20;
int numPasosBuzzJoints = 0;
int indexFrameBuzz = 0;
int indexFrameBuzzNext = 1;
float interpolationBuzz = 0.0;
int maxNumPasosBuzz = 100;
int numPasosBuzz = 0;

// Var animate helicopter
float rotHelHelY = 0.0;
float rotHelHelBack = 0.0;
float x1 = 0.0f, z1=0.0f;
bool limite_acercar = false;
float energiaVida = 100;

// Var animate lambo dor
int stateDoor = 0;
float dorRotCount = 0.0;

//******** Se declaran en Vectores de LUMINARIAS. Directional, Pointlights y Spotlight
//Las coordenadas van de acuerdo al mapa de blending de 4 capas establecido
// lamps position
std::vector<glm::vec3> lamp1Position =  {
	glm :: vec3 ( -84.27 , 0 , 50.66 ) ,
	glm :: vec3 ( -35.68 , 0 , 72.13 ) ,
	glm :: vec3 ( -12.21 , 0 , 23.18 )
};

std :: vector <float> lamp1Orientation = {
    -17.0 , -82.67 , 23.7
} ;


std::vector<glm::vec3> lamp2Position =  {
	glm::vec3(35.78, 0, 48.1),
	glm::vec3(64.4,0,5.43)
};

std :: vector <float> lamp2Orientation = {
	21.37,-65.0
} ;

std::vector<glm::vec3> lamp3Position =  {
	glm::vec3(37.5, 0, -21.08),
	glm::vec3(-33.39,0,-35.41)
};

std :: vector <float> lamp3Orientation = {
	240,30
} ;

std::vector<glm::vec3> lamp4Position =  {
	glm::vec3(69.56, 0, 70.99),
	glm::vec3(-19.08,0,-6.21)
};

std :: vector <float> lamp4Orientation = {
	21.37,-65.0
} ;
                                     
 
//Para *****************los OBJETOS TRANSPARENTES se establece su etiqueta y coordenada vec3 (Alpha) 
// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"aircraft", glm::vec3(10.0, 0.0, -17.5)},
		{"lambo", glm::vec3(23.0, 0.0, 0.0)},
		{"heli", glm::vec3(5.0, 10.0, -5.0)},
		{"fountain", glm::vec3(-1.07, 0.0, -3.62)},
		{"fire", glm::vec3(0.0, 0.0, 7.0)}
};

double deltaTime;
double currTime, lastTime;

//*******************para el SALTO de LUFFY y Golpe de LUFFY
// Jump variables
bool isJump = false;
bool isPunch = false;
bool isRecolect = false;
float GRAVITY = 1.81;
double tmv = 0;
double startTimeJump = 0;

//declaracion de variables PARA el CAMBIO de CAMARA
bool isThirdCamera=true,changingCamera=false;


//****************Variables para control de acciones de LUFFY 
int esperar = 5; //6
int saltar = 4;
int correr = 1;
int recoger = 3;
int golpe = 0; 
int desvanecer = 2;
int caminar_enemy = 0;
int golpe_enemy = 1;
int desvanecer_enemy = 2;
float golpe1 = 0.0;
float recogerFruta1 = 0.0;
int animationLuffyIndex = esperar;
int animationEnemyIndex = caminar_enemy;
int contador1 = 0;
int contador3 = 0;
int contador4 = 0;
int contador5 = 0;
bool saltoV = false;
bool opcionPresionarBoton1 = false;
bool opcionPresionarBoton2 = false;
bool opcionPresionarBoton3 = false;

bool visibleFruta1 = true;
bool visibleFruta2 = true;
bool visibleFruta3 = true;
bool visibleFruta4 = true;
bool visibleFruta5 = true;
bool visibleFruta6 = true;
bool visibleFruta7 = true;
bool visibleFruta8 = true;
bool visibleFruta9 = true;

bool visibleZombi = true;
bool visibleZombi_1a = true;
bool visibleZombi_1b = true;
bool visibleZombi_1c = true;
bool visibleZombi_1d = true;
bool visibleZombi_2a = true;
bool visibleZombi_2b = true;
bool visibleZombi_2c = true;
bool visibleZombi_3a = true;
bool visibleZombi_3b = true;
bool visibleZombi_3c = true;

//
// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 8000;
double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 0.4, particleLifetime = 4.0;  //para cambio de particula de la fogata
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;


// Colliders
//********** arreglos para los colisionadores de OBB y SBB
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Variables animacion maquina de estados eclipse
const float avance = 0.1;
const float giroEclipse = 0.5f;
float rotWheelsX = 0.0;
float rotWheelsY = 0.0;


//Para el sonido del sistema se definen y establecen las variables y Arrays

// OpenAL Defines
#define NUM_BUFFERS 9
#define NUM_SOURCES 9
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
//Source3
ALfloat source3Pos[]=  { 2.0, 0.0, 0.0 };
ALfloat source3Vel[]=  { 0.0, 0.0, 0.0 };
//Source4
ALfloat source4Pos[]={ 2.0, 0.0, 0.0 };
ALfloat source4Vel[]={ 2.0, 0.0, 0.0 };
//Source5
ALfloat source5Pos[]={0.0,0.0,0.0};
ALfloat source5Vel[]={0.0,0.0,0.0};
//Source6
ALfloat source6Pos[]={0.0,0.0,0.0};
ALfloat source6Vel[]={0.0,0.0,0.0};
//Source7
ALfloat source7Pos[]={0.0,0.0,0.0};
ALfloat source7Vel[]={0.0,0.0,0.0};
//Source8
ALfloat source8Pos[]={0.0,0.0,0.0};
ALfloat source8Vel[]={0.0,0.0,0.0};
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
//******** para que el sonido se inicie desde el inicio de ejecucion del programa
std::vector<bool> sourcesPlay = {true, true, true};

// Framesbuffers
GLuint depthMap, depthMapFBO;

//********* DEfinicion de las funciones principalmente para la
// la inicializacion del programa como los preparados y renderizados
// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderSolidScene();
void renderAlphaScene(bool render = true);
void renderScene();
void applicationLoop();


//****** Esta Clase inicializa los BÃƒÅ¡FERES DE PARTICULAS

void initParticleBuffers() {
	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	//Llena el bÃƒÂºfer de tiempo
	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}


//******** FunciÃƒÂ³n de iniciaciÃƒÂ³n del bÃƒÂºferes de partÃƒÂ­culas de Fuego

void initParticleBuffersFire() {
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for(unsigned int i = 0; i < nParticlesFire; i++ ) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine {};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER,0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}


// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//AquÃƒÂ­ se cargan SHADERS desde la carpeta de Shaders
	// InicializaciÃƒÂ³n de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	//shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", {"Position", "Velocity", "Age"});
	shaderParticlesFire.initialize("../Shaders/particlesLLuvia.vs", "../Shaders/particlesLluvia.fs", {"Position", "Velocity", "Age"});
	
	//shaderParticlesSmoke.initialize("../Shaders/particlesSmoke.vs", "../Shaders/particlesSmoke.fs", {"Position", "Velocity", "Age"});
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
	
	/*shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");*/

	// ******************Inicializacion de los objetos.
	
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(5.0f, 5.0f, 5.0f)); 

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	rayModel.init();
	rayModel.setShader(&shader);
	rayModel.setColor(glm::vec4(1.0));

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWalls.init();
	boxWalls.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	esfera1.init();
	esfera1.setShader(&shaderMulLighting);

	boxIntro.init();
	boxIntro.setShader(&shaderTexture);
	boxIntro.setScale(glm::vec3(2.0, 2.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);

	//****Se cargan los modelos .obj  .fbx   y otros

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	// Eclipse
	modelEclipseChasis.loadModel("../models/Eclipse/2003eclipse_chasis.obj");
	modelEclipseChasis.setShader(&shaderMulLighting);
	modelEclipseFrontalWheels.loadModel("../models/Eclipse/2003eclipse_frontal_wheels.obj");
	modelEclipseFrontalWheels.setShader(&shaderMulLighting);
	modelEclipseRearWheels.loadModel("../models/Eclipse/2003eclipse_rear_wheels.obj");
	modelEclipseRearWheels.setShader(&shaderMulLighting);
	// Helicopter
	modelHeliChasis.loadModel("../models/Helicopter/Mi_24_chasis.obj");
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.loadModel("../models/Helicopter/Mi_24_heli.obj");
	modelHeliHeli.setShader(&shaderMulLighting);
	modelHeliHeliBack.loadModel("../models/Helicopter/Mi_24_heli_rear.obj");
	modelHeliHeliBack.setShader(&shaderMulLighting);
	// Lamborginhi
	modelLambo.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	// Dart Lego
	modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	modelDartLegoRightLeg.setShader(&shaderMulLighting);

	
	// Buzz
	modelBuzzTorso.loadModel("../models/buzz/buzzlightyTorso.obj");
	modelBuzzTorso.setShader(&shaderMulLighting);
	modelBuzzHead.loadModel("../models/buzz/buzzlightyHead.obj");
	modelBuzzHead.setShader(&shaderMulLighting);
	modelBuzzLeftArm.loadModel("../models/buzz/buzzlightyLeftArm.obj");
	modelBuzzLeftArm.setShader(&shaderMulLighting);
	modelBuzzLeftForeArm.loadModel("../models/buzz/buzzlightyLeftForearm.obj");
	modelBuzzLeftForeArm.setShader(&shaderMulLighting);
	modelBuzzLeftHand.loadModel("../models/buzz/buzzlightyLeftHand.obj");
	modelBuzzLeftHand.setShader(&shaderMulLighting);

	
	//Modelos de las Lamparas
	modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	modelLamp1.setShader(&shaderMulLighting);

	modelLamp2.loadModel("../models/Luminarias/lampara_cementerio2Lamp.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelLamp2Post.loadModel("../models/Luminarias/lampara_cementerio2Poste.obj");
	modelLamp2Post.setShader(&shaderMulLighting);

	modelLamp3.loadModel("../models/Luminarias/Poste1/Poste1.obj");
	modelLamp3.setShader(&shaderMulLighting);

	modelLamp4.loadModel("../models/Luminarias/Poste2/Poste2.obj");
	modelLamp4.setShader(&shaderMulLighting);


	// Modelo LUFFY personaje principal
	//luffyModelAnimate.loadModel("../models/luffy/personaje2.fbx");
	luffyModelAnimate.loadModel("../models/luffy/todas_luffy_6.fbx");
	luffyModelAnimate.setShader(&shaderMulLighting);

	//Enemy OTRO personaje principal

	//Se pueden agregar otros modelos, despues de haber definido los Modelos arriba

	
	enemyModelAnimate.loadModel("../models/Enemy/Enemy_todas.fbx");
	enemyModelAnimate.setShader(&shaderMulLighting);


	
	// Cowboy
	cowboyModelAnimate.loadModel("../models/cowboy/Character Running.fbx");
	cowboyModelAnimate.setShader(&shaderMulLighting);

	// Guardian
	guardianModelAnimate.loadModel("../models/boblampclean/boblampclean.md5mesh");
	guardianModelAnimate.setShader(&shaderMulLighting);

	// Zombi
	//zombiModelAnimate.loadModel("../models/action_zombies/Z_Char1_corriendo.fbx");
	zombiModelAnimate.loadModel("../models/action_zombies/Z_Char1_corriendo.fbx");
	zombiModelAnimate.setShader(&shaderMulLighting);

	//Grass
	modelGrass.loadModel("../models/grass/grassModel.obj");
	modelGrass.setShader(&shaderMulLighting);

	//Fruta puntos
	modelFruta.loadModel("../models/Fruta_puntos/fruta_Puntos.obj");
	modelFruta.setShader(&shaderMulLighting);

	//Carga de la Fountain
	modelFountain.loadModel("../models/estatua_cementerio_3/estatua_cementerio_3_.fbx");
	modelFountain.setShader(&shaderMulLighting);




	modelArbol1a.loadModel("../models/arbol_cementerio_1/arbol_cementario_1__.obj");
	modelArbol1a.setShader(&shaderMulLighting);

	modelArbol1b.loadModel("../models/arbol_cementerio_1/arbol_cementario_1__.obj");
	modelArbol1b.setShader(&shaderMulLighting);

	modelArbol1c.loadModel("../models/arbol_cementerio_1/arbol_cementario_1__.obj");
	modelArbol1c.setShader(&shaderMulLighting);

	modelArbol2a.loadModel("../models/arbol_cementerio_2/arbol_cementerio_2.obj");
	modelArbol2a.setShader(&shaderMulLighting);

	modelArbol2b.loadModel("../models/arbol_cementerio_2/arbol_cementerio_2.obj");
	modelArbol2b.setShader(&shaderMulLighting);

	modelArbol2c.loadModel("../models/arbol_cementerio_2/arbol_cementerio_2.obj");
	modelArbol2c.setShader(&shaderMulLighting);

	modelArbol3a.loadModel("../models/arbol_cementerio_3/arbol_cementerio_3.obj");
	modelArbol3a.setShader(&shaderMulLighting);

	modelArbol3b.loadModel("../models/arbol_cementerio_3/arbol_cementerio_3.obj");
	modelArbol3b.setShader(&shaderMulLighting);

	modelAtaud1a.loadModel("../models/ataud/ataud1.fbx");
	modelAtaud1a.setShader(&shaderMulLighting);

	modelAtaud1b.loadModel("../models/ataud/ataud1.fbx");
	modelAtaud1b.setShader(&shaderMulLighting);

	modelAtaud1c.loadModel("../models/ataud/ataud1.fbx");
	modelAtaud1c.setShader(&shaderMulLighting);

	modelAtaud1d.loadModel("../models/ataud/ataud1.fbx");
	modelAtaud1d.setShader(&shaderMulLighting);

	modelAtaud2a.loadModel("../models/ataud_2/ataud2.fbx");
	modelAtaud2a.setShader(&shaderMulLighting);

	modelAtaud2b.loadModel("../models/ataud_2/ataud2.fbx");
	modelAtaud2b.setShader(&shaderMulLighting);

	modelAtaud2c.loadModel("../models/ataud_2/ataud2.fbx");
	modelAtaud2c.setShader(&shaderMulLighting);

	modelAtaud2d.loadModel("../models/ataud_2/ataud2.fbx");
	modelAtaud2d.setShader(&shaderMulLighting);

	modelArco.loadModel("../models/arco_cementerio/arco_cementerio.obj");;
	modelArco.setShader(&shaderMulLighting);


	
	modelZombi_1a.loadModel("../models/action_zombies/Z_Char1_camina_B.fbx");
	modelZombi_1a.setShader(&shaderMulLighting);

	modelZombi_1b.loadModel("../models/action_zombies/Z_Char1_camina_B.fbx");
	modelZombi_1b.setShader(&shaderMulLighting);

	modelZombi_1c.loadModel("../models/action_zombies/Z_Char1_camina_B.fbx");
	modelZombi_1c.setShader(&shaderMulLighting);

	modelZombi_1d.loadModel("../models/action_zombies/Z_Char1_camina_B.fbx");
	modelZombi_1d.setShader(&shaderMulLighting);

	modelZombi_2a.loadModel("../models/action_zombies/Z_Char1_camina_C.fbx");
	modelZombi_2a.setShader(&shaderMulLighting);

	modelZombi_2b.loadModel("../models/action_zombies/Z_Char1_camina_C.fbx");
	modelZombi_2b.setShader(&shaderMulLighting);

	modelZombi_2c.loadModel("../models/action_zombies/Z_Char1_camina_C.fbx");
	modelZombi_2c.setShader(&shaderMulLighting);

	modelZombi_3a.loadModel("../models/action_zombies/Z_Char1_camina_.fbx");
	modelZombi_3a.setShader(&shaderMulLighting);

	modelZombi_3b.loadModel("../models/action_zombies/Z_Char1_camina_.fbx");
	modelZombi_3b.setShader(&shaderMulLighting);

	modelZombi_3c.loadModel("../models/action_zombies/Z_Char1_camina_.fbx");
	modelZombi_3c.setShader(&shaderMulLighting);

	


	// Carga de Terreno Terreno
	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));//agregado

	// Se inicializa el model de RENDER TEXT. Renderizado de texto
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelText->Initialize();

	//********** PAra Cambio de CAMARA
	if(isThirdCamera==true)
	{
		camera->setPosition(glm::vec3(0.0,0.0, 10.0));
		camera->setDistanceFromTarget(distanceFromTarget);
		camera->setSensitivity(1.0);
	} 
	if(isThirdCamera==false)
	{
		camera1->setPosition(camera->getPosition());	
	}

	//camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	//camera->setDistanceFromTarget(distanceFromTarget);
	//camera->setSensitivity(1.0);
	
	//************************* CARGA DE TEXTURAS VARIAS
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData()) {
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
			skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	// Definiendo la textura a utilizar
	//Texture textureCesped("../Textures/grassy2.png");
	Texture textureCesped("../Textures/Vol_64_3_Base_Color_256.png");
	
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureCesped.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureCesped.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		std::cout << "Numero de canales :=> " << textureCesped.getChannels() << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, textureCesped.getWidth(), textureCesped.getHeight(), 0,
		textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureCesped.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage();


	// Defininiendo texturas del mapa de mezclas
	// Definiendo la textura
	//Texture textureR("../Textures/mud.png");
	Texture textureR("../Textures/GroundGravel_BaseColor_v03_256.png");
	
	textureR.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainRID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureR.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, textureR.getWidth(), textureR.getHeight(), 0,
		textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureR.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureR.freeImage(); // Liberamos memoria

	// Definiendo la textura
	//Texture textureG("../Textures/grassFlowers.png");
	Texture textureG("../Textures/Vol_24_4_Base_Color_256.png");

	textureG.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainGID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureG.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, textureG.getWidth(), textureG.getHeight(), 0,
		textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureG.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureG.freeImage(); // Liberamos memoria

	// Definiendo la textura
	//Texture textureB("../Textures/path.png");
	Texture textureB("../Textures/Vol_64_5_Base_Color_256.png");
	textureB.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureB.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, textureB.getWidth(), textureB.getHeight(), 0,
		textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureB.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureB.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureBlendMap("../Textures/blendmapFinal.png");
	textureBlendMap.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBlendMapID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlendMap.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlendMap.getWidth(), textureBlendMap.getHeight(), 0,
		textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlendMap.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlendMap.freeImage(); // Liberamos memoria


	//********** TEXTURAS DE LA Pantalla Inicial
	// Definiendo la textura  



	Texture textureIntro1("../Textures/PantallaInicio1.png");
	textureIntro1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit1ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit1ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro1.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro1.getWidth(), textureIntro1.getHeight(), 0,
		textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro1.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro1.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro2("../Textures/PantallaInicio2.png");
	textureIntro2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro2.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro2.getWidth(), textureIntro2.getHeight(), 0,
		textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro2.freeImage(); // Liberamos memoria

	
	Texture textureScreen("../Textures/Screen.png");
	textureScreen.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreenID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreenID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen.getWidth(), textureScreen.getHeight(), 0,
		textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen.freeImage(); // Liberamos memoria


	// Definiendo la textura
	Texture textureScreen1("../Textures/barra_activa1.png");
	textureScreen1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreen1ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreen1ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen1.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen1.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen.getWidth(), textureScreen.getHeight(), 0,
		textureScreen1.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen1.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen1.freeImage(); // Liberamos memoria

	Texture textureScreen2("../Textures/barra_activa2.png");
	textureScreen2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreen2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreen2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen2.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen2.getWidth(), textureScreen2.getHeight(), 0,
		textureScreen2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen2.freeImage(); // Liberamos memoria

	Texture textureScreen3("../Textures/barra_activa3.png");
	textureScreen3.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreen3ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreen3ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen3.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen3.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen3.getWidth(), textureScreen3.getHeight(), 0,
		textureScreen3.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen3.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen3.freeImage(); // Liberamos memoria

	Texture textureScreen4("../Textures/barra_activa4.png");
	textureScreen4.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreen4ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreen4ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen4.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen4.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen4.getWidth(), textureScreen4.getHeight(), 0,
		textureScreen4.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen4.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen4.freeImage(); // Liberamos memoria

	Texture textureScreen5("../Textures/barra_activa5.png");
	textureScreen5.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreen5ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreen5ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen5.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen5.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen5.getWidth(), textureScreen5.getHeight(), 0,
		textureScreen5.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen5.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen5.freeImage(); // Liberamos memoria

	// Definiendo la textura de la FUENTE
	//Texture textureParticlesFountain("../Textures/bluewater.png");
	Texture textureParticlesFountain("../Textures/MoradoFuente.png");
	textureParticlesFountain.loadImage(); // Cargar la textura
	glGenTextures(1, &textureParticleFountainID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimizaciÃƒÂ³n
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureParticlesFountain.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureParticlesFountain.getChannels() == 3 ? GL_RGB : GL_RGBA, textureParticlesFountain.getWidth(), textureParticlesFountain.getHeight(), 0,
		textureParticlesFountain.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureParticlesFountain.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureParticlesFountain.freeImage(); // Liberamos memoria


	//****Textura de las particulas de Fuego
	Texture textureParticleFire("../Textures/lluvia.png");
	textureParticleFire.loadImage();
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (textureParticleFire.getData()) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureParticleFire.getWidth(), textureParticleFire.getHeight(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, textureParticleFire.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleFire.freeImage();

	


	//**********Algoritmo para GENERADOR DE PARTICULAS

	std::uniform_real_distribution<float> distr01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for( int i = 0; i < randData.size(); i++ ) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f,-5.8f,0.0f)));  //<< indica el sentido hacia arriba o hacia abajo la dereccion de las particulas
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f, 10.0f, 0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0,1,0);
	n = glm::cross(glm::vec3(1,0,0), v);
	if( glm::length(n) < 0.00001f ) {
		n = glm::cross(glm::vec3(0,1,0), v);
	}
	u = glm::cross(v,n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

	/*******************************************
	 * Inicializacion de los buffers de la fuente
	 *******************************************/
	initParticleBuffers();

	/*******************************************
	 * Inicializacion de los buffers del fuego
	 *******************************************/
	initParticleBuffersFire();

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el BÃƒÅ¡FER DE PROFUNDIDAD  
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	/*******************************************
	 * OpenAL init  SONIDO
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/fountain.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/Synthezx.wav");//salto
	buffer[2] = alutCreateBufferFromFile("../sounds/step.wav");
	buffer[3] = alutCreateBufferFromFile("../sounds/Jumping.wav");//ambiente
	buffer[4] = alutCreateBufferFromFile("../sounds/Binks.wav");//menu
	buffer[5] = alutCreateBufferFromFile("../sounds/Myuu_Final_Boss.wav");//boss
	buffer[6] = alutCreateBufferFromFile("../sounds/gore_hit.wav");//--luffy es golpeado
	buffer[7] = alutCreateBufferFromFile("../sounds/block_hit.wav");//-luffy golpea
	buffer[8] = alutCreateBufferFromFile("../sounds/fruit_pick_2.wav");//-luffy recoge fruta
    
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR){
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	//fuente
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 3.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	//ambiental
	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 0.08f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 1000);

	//run
	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 2000);

	//jump
	alSourcef(source[3], AL_PITCH, 1.0f);
	alSourcef(source[3], AL_GAIN, 4.0);
	alSourcefv(source[3], AL_POSITION, source3Pos);
	alSourcefv(source[3], AL_VELOCITY, source3Vel);
	alSourcei(source[3], AL_BUFFER, buffer[3]);
	alSourcei(source[3], AL_LOOPING, AL_TRUE);
	alSourcef(source[3], AL_MAX_DISTANCE, 2000);

    //menu
	alSourcef(source[4], AL_PITCH, 1.0f);
	alSourcef(source[4], AL_GAIN, 3.0f);
	alSourcefv(source[4], AL_POSITION, source4Pos);
	alSourcefv(source[4], AL_VELOCITY, source4Vel);
	alSourcei(source[4], AL_BUFFER, buffer[4]);
	alSourcei(source[4], AL_LOOPING, AL_TRUE);
	alSourcef(source[4], AL_MAX_DISTANCE, 2000);

    //boss
	alSourcef(source[5], AL_PITCH, 1.0f);
	alSourcef(source[5], AL_GAIN, 0.3f);
	alSourcefv(source[5], AL_POSITION, source5Pos);
	alSourcefv(source[5], AL_VELOCITY, source5Vel);
	alSourcei(source[5], AL_BUFFER, buffer[5]);
	alSourcei(source[5], AL_LOOPING, AL_TRUE);
	alSourcef(source[5], AL_MAX_DISTANCE, 2000);

    //luffy_loss
	alSourcef(source[6], AL_PITCH, 1.0f);
	alSourcef(source[6], AL_GAIN, 0.3f);
	alSourcefv(source[6], AL_POSITION, source6Pos);
	alSourcefv(source[6], AL_VELOCITY, source6Vel);
	alSourcei(source[6], AL_BUFFER, buffer[6]);
	alSourcei(source[6], AL_LOOPING, AL_TRUE);
	alSourcef(source[6], AL_MAX_DISTANCE, 2000);

    //luffy_hit
	alSourcef(source[7], AL_PITCH, 1.0f);
	alSourcef(source[7], AL_GAIN, 0.3f);
	alSourcefv(source[7], AL_POSITION, source7Pos);
	alSourcefv(source[7], AL_VELOCITY, source7Vel);
	alSourcei(source[7], AL_BUFFER, buffer[7]);
	alSourcei(source[7], AL_LOOPING, AL_TRUE);
	alSourcef(source[7], AL_MAX_DISTANCE, 2000);

    //luffy_eat
	alSourcef(source[8], AL_PITCH, 1.0f);
	alSourcef(source[8], AL_GAIN, 0.3f);
	alSourcefv(source[8], AL_POSITION, source8Pos);
	alSourcefv(source[8], AL_VELOCITY, source8Vel);
	alSourcei(source[8], AL_BUFFER, buffer[8]);
	alSourcei(source[8], AL_LOOPING, AL_TRUE);
	alSourcef(source[8], AL_MAX_DISTANCE, 2000);

}


//******funcion de DESTRUCTORES 
void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderParticlesFountain.destroy();
	shaderParticlesFire.destroy();
	/*shaderParticlesFountain.destroy();*/

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxWalls.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();
	esfera1.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	rayModel.destroy();

	boxIntro.destroy();
	boxViewDepth.destroy();
	boxLightViewBox.destroy();

	// Custom objects Delete
	modelAircraft.destroy();
	modelDartLegoBody.destroy();
	modelDartLegoHead.destroy();
	modelDartLegoLeftArm.destroy();
	modelDartLegoLeftHand.destroy();
	modelDartLegoLeftLeg.destroy();
	modelDartLegoMask.destroy();
	modelDartLegoRightArm.destroy();
	modelDartLegoRightHand.destroy();
	modelDartLegoRightLeg.destroy();
	modelEclipseChasis.destroy();
	modelEclipseFrontalWheels.destroy();
	modelEclipseRearWheels.destroy();
	modelHeliChasis.destroy();
	modelHeliHeli.destroy();
	modelHeliHeliBack.destroy();
	modelLambo.destroy();
	modelLamboFrontLeftWheel.destroy();
	modelLamboFrontRightWheel.destroy();
	modelLamboLeftDor.destroy();
	modelLamboRearLeftWheel.destroy();
	modelLamboRearRightWheel.destroy();
	modelLamboRightDor.destroy();
	modelRock.destroy();
	modelBuzzHead.destroy();
	modelBuzzLeftArm.destroy();
	modelBuzzLeftForeArm.destroy();
	modelBuzzLeftHand.destroy();
	modelBuzzTorso.destroy();


	//Lamparas
	modelLamp1.destroy();
	modelLamp2.destroy();
	modelLamp2Post.destroy();
	modelLamp3.destroy();
	modelLamp4.destroy();

	
	luffyModelAnimate.destroy();
	cowboyModelAnimate.destroy();
	guardianModelAnimate.destroy();
	zombiModelAnimate.destroy();
	enemyModelAnimate.destroy();
	modelFountain.destroy();
	modelFruta.destroy();


	modelArbol1a.destroy();
	modelArbol1b.destroy();
	modelArbol1c.destroy();

	modelArbol2a.destroy();
	modelArbol2b.destroy();
	modelArbol2c.destroy();

	modelArbol3a.destroy();
	modelArbol3b.destroy();

	modelAtaud1a.destroy();
	modelAtaud1b.destroy();
	modelAtaud1c.destroy();
	modelAtaud1d.destroy();

	modelAtaud2a.destroy();
	modelAtaud2b.destroy();
	modelAtaud2c.destroy();
	modelAtaud2d.destroy();

	modelArco.destroy();

	modelZombi_1a.destroy();
	

	modelZombi_1b.destroy();
	

	modelZombi_1c.destroy();
	

	modelZombi_1d.destroy();
	

	modelZombi_2a.destroy();
	

	modelZombi_2b.destroy();
	

	modelZombi_2c.destroy();
	

	modelZombi_3a.destroy();
	

	modelZombi_3b.destroy();
	

	modelZombi_3c.destroy();
	


	// Terrains objects Delete
	terrain.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureInit1ID);
	glDeleteTextures(1, &textureInit2ID);
	glDeleteTextures(1, &textureScreenID);
	glDeleteTextures(1, &textureParticleFountainID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fountain particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVel);
	glDeleteBuffers(1, &startTime);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticles);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);
}


//********Funciones para cpontrol del ratÃƒÂ³n (MOUSE)
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

//******** funcion de ENTRADA DE PROCESO

bool processInput(bool continueApplication) {
	sourcesPlay[4]=true;
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if(!gameOver)
	{
	if(!iniciaPartida ){
		bool presionarEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
		if(textureActivaID == textureInit1ID && presionarEnter){
			iniciaPartida = true;
			textureActivaID = textureScreenID;
		}
		else if(!presionarOpcion && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			presionarOpcion = true;
			if(textureActivaID == textureInit1ID)
				textureActivaID = textureInit2ID;
			else if(textureActivaID == textureInit2ID)
				textureActivaID = textureInit1ID;
		}
		else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
			presionarOpcion = false;
	}

	}

	if(gameOver)
	{
	
	iniciaPartida = false;
	//********** TEXTURAS DE LA Pantalla Inicial
	// Definiendo la textura
	textureActivaID = textureInit1ID;
	if(!iniciaPartida ){
		bool presionarEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
		if(textureActivaID == textureInit1ID && presionarEnter){
			iniciaPartida = true;
			textureActivaID = textureScreenID;
		}
		else if(!presionarOpcion && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			presionarOpcion = true;
			if(textureActivaID == textureInit1ID)
				textureActivaID = textureInit2ID;
			else if(textureActivaID == textureInit2ID)
				textureActivaID = textureInit1ID;
		}
		else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
			presionarOpcion = false;
	}

	}

	//***para INICIAR LA PARTIDA

	
	
	if (sumaColisionOBB_OBB >=1.0 && sumaColisionOBB_OBB <20.0)
	{
		textureActivaID = textureScreen1ID;
		
	}
	if (sumaColisionOBB_OBB>=21.0 && sumaColisionOBB_OBB <40.0)
	{
		textureActivaID = textureScreen2ID;
		
	}
	if (sumaColisionOBB_OBB>=41.0 && sumaColisionOBB_OBB <60.0)
	{
		textureActivaID = textureScreen3ID;
		
	}
	if (sumaColisionOBB_OBB>=61.0 && sumaColisionOBB_OBB <70.0)
	{
		textureActivaID = textureScreen4ID;
		
	}
	if (sumaColisionOBB_OBB>=71.0 && sumaColisionOBB_OBB <80.0)
	{
		textureActivaID = textureScreen5ID;
		
	}
	if (sumaColisionOBB_OBB>=81.0&& sumaColisionOBB_OBB <100.0)
	{
		textureActivaID = textureScreenID;

		//sumaColisionOBB_OBB = 0.0;
		
	}
	


	//** PARA CAMBIO DE CAMARA
	if (isThirdCamera)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	}
	else
	{
		
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera1->moveFrontCamera(true, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera1->moveFrontCamera(false, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera1->moveRightCamera(false, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera1->moveRightCamera(true, deltaTime);
		if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)== GLFW_PRESS)
			camera1->mouseMoveCamera(offsetX, offsetY, deltaTime);
		
	}

	
	//*************PARA EJECUTAR EL CAMBIO DE CAMARA 
	// CAMBIO DE CAMARA CONTROL+K
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS )
	{
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{
			changingCamera = true;
			camera1->setPosition(glm::vec3(camera->getPosition()));
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
		{
			if (changingCamera)
			{
				isThirdCamera = !isThirdCamera;
				std::cout << "Changed Camera" << std::endl;
			}
			changingCamera = false;
		}
		
	}

	offsetX = 0;
	offsetY = 0;
	

	//if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	//	camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	//if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		//camera->mouseMoveCamera(0.0, offsetY, deltaTime);

	
	

	// *******SELECCION DE MODEL POR KEYFRAMES 
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 4)
			modelSelected = 0;
		if(modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		if(modelSelected == 3)
			fileName = "../animaciones/animation_buzz_joints.txt";
		if (modelSelected == 4)
			fileName = "../animaciones/animation_buzz.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Guardar key frames
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = true;
		if(myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = false;
		myfile.close();
		if(modelSelected == 1)
			keyFramesDartJoints = getKeyRotFrames(fileName);
		if (modelSelected == 2)
			keyFramesDart = getKeyFrames(fileName);
		if(modelSelected == 3)
			keyFramesBuzzJoints = getKeyRotFrames(fileName);
		if (modelSelected == 4)
			keyFramesBuzz = getKeyFrames(fileName);
	}
	if(availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
		saveFrame = true;
		availableSave = false;
	}if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	// Dart Lego model movements
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg -= 0.02;
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(-0.02, 0.0, 0.0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(0.02, 0.0, 0.0));
	
	// Movimientos de buzz
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzLeftarm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzLeftarm -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzLeftForeArm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzLeftForeArm -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotBuzzLeftHand += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotBuzzLeftHand -= 0.02;
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, 0.02));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, -0.02));
	animationLuffyIndex = esperar;


	//***********DECISIONES PARA ACCIONES DE LUFFY
	// Gira a la izquierda, Giro a la derecha
	// Avanza, y retrocede
	// Controles de luffy
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		modelMatrixLuffy = glm::rotate(modelMatrixLuffy, 0.05f, glm::vec3(0, 1, 0));
		animationLuffyIndex = correr;
		sourcesPlay[2]=true;
	} else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		modelMatrixLuffy = glm::rotate(modelMatrixLuffy, -0.05f, glm::vec3(0, 1, 0));
		animationLuffyIndex = correr;
		sourcesPlay[2]=true;
	}
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		modelMatrixLuffy = glm::translate(modelMatrixLuffy, glm::vec3(0.0, 0.0, 0.5));
		animationLuffyIndex = correr;
		sourcesPlay[2]=true;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		modelMatrixLuffy = glm::translate(modelMatrixLuffy, glm::vec3(0.0, 0.0, -0.5));
		animationLuffyIndex = correr;
		sourcesPlay[2]=true;
	}
	//Movimiento LUFFY GOLPEA
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
		isPunch = true;
		
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE){
		golpe1 = 0.0f;
	}
	
	//movimiento LUffy RECOGE OBJETO
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){
		opcionPresionarBoton1 = true;
		
	} else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE){
		recogerFruta1 = 0.0f;
	}

	if(opcionPresionarBoton1){
		animationLuffyIndex = recoger;
		recogerFruta1 = 5.0f;
		contador3 +=1;
		if(contador3 >=20){
			contador3 = 0;
			opcionPresionarBoton1 = false;
		}
	}
	//movimiento Luffy SE DESVANECE
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
		opcionPresionarBoton2 = true;
	} 

	if(opcionPresionarBoton2){
		animationLuffyIndex = desvanecer;
		contador4 +=1;
		if(contador4 >=60){
			contador4 =0;
			opcionPresionarBoton2 = false;
		}
	}
	//Luffy SALTA
	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if(!isJump && keySpaceStatus){
		isJump = true;
		startTimeJump = currTime;
		tmv = 0;
		sourcesPlay[3]=true;
		
		
	}
	//control del salto
	if(isJump==true){
		animationLuffyIndex = saltar;
	
	}
	//control del golpe
	if(isPunch==true){
		animationLuffyIndex = golpe;
		golpe1 = 20.0f;
		contador1 += 1;
		if(contador1 > 20)
		{
			isPunch = false;
			contador1 = 0;
		}
			
	}

	
	//*********CaracterÃƒÂ­sticas para el Joystick Para mover el personaje principal LUFFY
	if (modelSelected == 0 && glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
		std::cout << "Esta presente el joystick" << std::endl;
		int axesCount, buttonCount;
		const float * axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		std::cout << "NÃƒÂºmero de ejes disponibles :=>" << axesCount << std::endl;
		std::cout << "Left Stick X axis: " << axes[0] << std::endl;
		std::cout << "Left Stick Y axis: " << axes[1] << std::endl;
		std::cout << "Left Trigger/L2: " << axes[2] << std::endl;
		std::cout << "Right Stick X axis: " << axes[3] << std::endl;
		std::cout << "Right Stick Y axis: " << axes[4] << std::endl;
		std::cout << "Right Trigger/R2: " << axes[5] << std::endl;


		if(fabs(axes[1]) > 0.2){
			modelMatrixLuffy = glm::translate(modelMatrixLuffy, glm::vec3(0, 0, -axes[1] * 0.8));
			animationLuffyIndex = correr;
			sourcesPlay[2]=true;
		}if(fabs(axes[0]) > 0.2){
			modelMatrixLuffy = glm::rotate(modelMatrixLuffy, glm::radians(-axes[0] * 3.5f), glm::vec3(0, 1, 0));
			animationLuffyIndex = correr;
			sourcesPlay[2]=true;
		}

		//Movimiento de la camara alrededor de LUFFY


		if(fabs(axes[2]) > 0.2){
			camera->mouseMoveCamera(axes[2], 0.0, deltaTime);
			
		}
		
		if(fabs(axes[3]) > 0.2){
			camera->mouseMoveCamera(0.0, axes[3], deltaTime);
		}

				


		const unsigned char * buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		std::cout << "NÃƒÂºmero de botones disponibles :=>" << buttonCount << std::endl;
		if(buttons[0] == GLFW_PRESS)
			std::cout << "Se presiona x" << std::endl;

		if(!isJump && buttons[0] == GLFW_PRESS){
			isJump = true;
			startTimeJump = currTime;
			tmv = 0;
			sourcesPlay[3]=true;
		}
		//Golpe
		if(!isPunch && buttons[1] == GLFW_PRESS){
			isPunch = true;
			
		
		}
		//Recoger
		if(!opcionPresionarBoton1 && buttons[2] == GLFW_PRESS){
			opcionPresionarBoton1 = true;
			
		}
		//Desvanecer
		if(!opcionPresionarBoton2 && buttons[3] == GLFW_PRESS){
			opcionPresionarBoton2 = true;
			
		}

		

				
		
		
	}

	glfwPollEvents();
	return continueApplication;
}


//******Funcion de Prepraracion de Escena
void prepareScene(){

	//skyboxSphere.setShader(&shaderSkybox);  // <<Esta es para las particulas Fuego

	terrain.setShader(&shaderTerrain);
	
	//modelRock.setShader(&shaderMulLighting);

	//modelAircraft.setShader(&shaderMulLighting);

	// Eclipse
	/*modelEclipseChasis.setShader(&shaderMulLighting);
	modelEclipseFrontalWheels.setShader(&shaderMulLighting);
	modelEclipseRearWheels.setShader(&shaderMulLighting);*/

	// Helicopter
	/*modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.setShader(&shaderMulLighting);
	modelHeliHeliBack.setShader(&shaderMulLighting);*/

	// Lamborginhi
	/*modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.setShader(&shaderMulLighting);*/

	// Dart Lego
	/*modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.setShader(&shaderMulLighting);*/

	//Lamp models
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.setShader(&shaderMulLighting);
	modelLamp2Post.setShader(&shaderMulLighting);
	modelLamp3.setShader(&shaderMulLighting);
	modelLamp4.setShader(&shaderMulLighting);
	//Buzz
	/*modelBuzzTorso.setShader(&shaderMulLighting);
	modelBuzzHead.setShader(&shaderMulLighting);
	modelBuzzLeftArm.setShader(&shaderMulLighting);
	modelBuzzLeftForeArm.setShader(&shaderMulLighting);
	modelBuzzLeftHand.setShader(&shaderMulLighting);*/

	//Grass
	//modelGrass.setShader(&shaderMulLighting);

	//Luffy
	luffyModelAnimate.setShader(&shaderMulLighting);  //<<LUFFY personaje principal

	//Cowboy
	//cowboyModelAnimate.setShader(&shaderMulLighting);

	//Guardian
	//guardianModelAnimate.setShader(&shaderMulLighting);

	//Zombi //Zombie
	zombiModelAnimate.setShader(&shaderMulLighting);

	//Enemy
	enemyModelAnimate.setShader(&shaderMulLighting);  //<<Otro personaje principal


	// Fountain
	modelFountain.setShader(&shaderMulLighting);
	//Fruta puntos
	modelFruta.setShader(&shaderMulLighting);


	modelArbol1a.setShader(&shaderMulLighting);

	
	modelArbol1b.setShader(&shaderMulLighting);

	
	modelArbol1c.setShader(&shaderMulLighting);

	
	modelArbol2a.setShader(&shaderMulLighting);

	
	modelArbol2b.setShader(&shaderMulLighting);

	
	modelArbol2c.setShader(&shaderMulLighting);

	
	modelArbol3a.setShader(&shaderMulLighting);

	
	modelArbol3b.setShader(&shaderMulLighting);

	
	modelAtaud1a.setShader(&shaderMulLighting);

	
	modelAtaud1b.setShader(&shaderMulLighting);

	
	modelAtaud1c.setShader(&shaderMulLighting);

	
	modelAtaud1d.setShader(&shaderMulLighting);

	
	modelAtaud2a.setShader(&shaderMulLighting);

	
	modelAtaud2b.setShader(&shaderMulLighting);

	
	modelAtaud2c.setShader(&shaderMulLighting);

	
	modelAtaud2d.setShader(&shaderMulLighting);

	
	modelArco.setShader(&shaderMulLighting);



	modelZombi_1a.setShader(&shaderMulLighting);
	

	modelZombi_1b.setShader(&shaderMulLighting);
	

	modelZombi_1c.setShader(&shaderMulLighting);
	

	modelZombi_1d.setShader(&shaderMulLighting);
	

	modelZombi_2a.setShader(&shaderMulLighting);
	

	modelZombi_2b.setShader(&shaderMulLighting);
	

	modelZombi_2c.setShader(&shaderMulLighting);
	

	modelZombi_3a.setShader(&shaderMulLighting);
	

	modelZombi_3b.setShader(&shaderMulLighting);
	

	modelZombi_3c.setShader(&shaderMulLighting);
	


}


//************* Preparacion de Profundidad de Escena 
void prepareDepthScene(){

	//skyboxSphere.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);
	
	//modelRock.setShader(&shaderDepth);

	//modelAircraft.setShader(&shaderDepth);

	// Eclipse
	//modelEclipseChasis.setShader(&shaderDepth);
	//modelEclipseFrontalWheels.setShader(&shaderDepth);
	//modelEclipseRearWheels.setShader(&shaderDepth);

	// Helicopter
	//modelHeliChasis.setShader(&shaderDepth);
	//modelHeliHeli.setShader(&shaderDepth);
	//modelHeliHeliBack.setShader(&shaderDepth);

	// Lamborginhi
	/*modelLambo.setShader(&shaderDepth);
	modelLamboLeftDor.setShader(&shaderDepth);
	modelLamboRightDor.setShader(&shaderDepth);
	modelLamboFrontLeftWheel.setShader(&shaderDepth);
	modelLamboFrontRightWheel.setShader(&shaderDepth);
	modelLamboRearLeftWheel.setShader(&shaderDepth);
	modelLamboRearRightWheel.setShader(&shaderDepth);*/

	// Dart Lego
	/*modelDartLegoBody.setShader(&shaderDepth);
	modelDartLegoMask.setShader(&shaderDepth);
	modelDartLegoHead.setShader(&shaderDepth);
	modelDartLegoLeftArm.setShader(&shaderDepth);
	modelDartLegoRightArm.setShader(&shaderDepth);
	modelDartLegoLeftHand.setShader(&shaderDepth);
	modelDartLegoRightHand.setShader(&shaderDepth);
	modelDartLegoLeftLeg.setShader(&shaderDepth);
	modelDartLegoRightLeg.setShader(&shaderDepth);*/

	//Lamp models
	modelLamp1.setShader(&shaderDepth);
	modelLamp2.setShader(&shaderDepth);
	modelLamp2Post.setShader(&shaderDepth);
	modelLamp3.setShader(&shaderDepth);
	modelLamp4.setShader(&shaderDepth);
	
	//Buzz
	/*modelBuzzTorso.setShader(&shaderDepth);
	modelBuzzHead.setShader(&shaderDepth);
	modelBuzzLeftArm.setShader(&shaderDepth);
	modelBuzzLeftForeArm.setShader(&shaderDepth);
	modelBuzzLeftHand.setShader(&shaderDepth);*/

	//Grass
	//modelGrass.setShader(&shaderDepth);

	//Luffy
	luffyModelAnimate.setShader(&shaderDepth);

	//Cowboy
	//cowboyModelAnimate.setShader(&shaderDepth);

	//Enemy
	enemyModelAnimate.setShader(&shaderDepth);

	//Guardian
	//guardianModelAnimate.setShader(&shaderDepth);

	//Zombi //Zombie
	zombiModelAnimate.setShader(&shaderDepth);

	// Fountain
	modelFountain.setShader(&shaderDepth);

	//Fruta puntos
	modelFruta.setShader(&shaderDepth);


	modelArbol1a.setShader(&shaderDepth);

	
	modelArbol1b.setShader(&shaderDepth);

	
	modelArbol1c.setShader(&shaderDepth);

	
	modelArbol2a.setShader(&shaderDepth);

	
	modelArbol2b.setShader(&shaderDepth);

	
	modelArbol2c.setShader(&shaderDepth);

	
	modelArbol3a.setShader(&shaderDepth);

	
	modelArbol3b.setShader(&shaderDepth);

	
	modelAtaud1a.setShader(&shaderDepth);

	
	modelAtaud1b.setShader(&shaderDepth);

	
	modelAtaud1c.setShader(&shaderDepth);

	
	modelAtaud1d.setShader(&shaderDepth);

	
	modelAtaud2a.setShader(&shaderDepth);

	
	modelAtaud2b.setShader(&shaderDepth);

	
	modelAtaud2c.setShader(&shaderDepth);

	
	modelAtaud2d.setShader(&shaderDepth);

	
	modelArco.setShader(&shaderDepth);

	modelZombi_1a.setShader(&shaderDepth);
	

	modelZombi_1b.setShader(&shaderDepth);
	

	modelZombi_1c.setShader(&shaderDepth);
	

	modelZombi_1d.setShader(&shaderDepth);
	

	modelZombi_2a.setShader(&shaderDepth);
	

	modelZombi_2b.setShader(&shaderDepth);
	

	modelZombi_2c.setShader(&shaderDepth);
	

	modelZombi_3a.setShader(&shaderDepth);
	

	modelZombi_3b.setShader(&shaderDepth);
	

	modelZombi_3c.setShader(&shaderDepth);

}


	//****** Renderizado de SOLIDOS 
void renderSolidScene(){
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	// Se activa la textura del agua
	//***********Mezclado de Texturas
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	shaderTerrain.setInt("backgroundTexture", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(80, 80)));
	terrain.setPosition(glm::vec3(100, 0, 100));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/
	//Rock render
	matrixModelRock[3][1] = terrain.getHeightTerrain(matrixModelRock[3][0], matrixModelRock[3][2]);
	//modelRock.render(matrixModelRock);
	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	// Render for the eclipse car
	modelMatrixEclipse[3][1] = terrain.getHeightTerrain(modelMatrixEclipse[3][0], modelMatrixEclipse[3][2]);
	glm::mat4 modelMatrixEclipseChasis = glm::mat4(modelMatrixEclipse);
	modelMatrixEclipseChasis = glm::scale(modelMatrixEclipse, glm::vec3(0.5, 0.5, 0.5));
	//modelEclipseChasis.render(modelMatrixEclipseChasis);

	glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis);
	modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483 ));
	modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsY, glm::vec3(0, 1, 0));
	modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsX, glm::vec3(1, 0, 0));
	modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
	//modelEclipseFrontalWheels.render(modelMatrixFrontalWheels);

	glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
	modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157 ));
	modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, rotWheelsX, glm::vec3(1, 0, 0));
	modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
	//modelEclipseRearWheels.render(modelMatrixRearWheels);

	//Renderizado de LÃƒÂMPARAS
	//Render lamp1
		for(int i=0; i<lamp1Position.size(); i++){
			lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x,lamp1Position[i].z);
			modelLamp1.setPosition(lamp1Position[i]);
			modelLamp1.setScale(glm::vec3(0.5));
			modelLamp1.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
			modelLamp1.render();
		}
		//Render lamp2
		for(int i=0; i<lamp2Position.size(); i++){
			lamp2Position[i].y = terrain.getHeightTerrain(lamp2Position[i].x,lamp2Position[i].z);
			modelLamp2.setPosition(lamp2Position[i]);
			modelLamp2.setScale(glm::vec3(2.0));  //escala de la nueva luminaria
			modelLamp2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
			modelLamp2.render();
			modelLamp2Post.setPosition(lamp2Position[i]);
			modelLamp2Post.setScale(glm::vec3(2.0));
			modelLamp2Post.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
			modelLamp2Post.render();
		}

		for(int i=0; i<lamp3Position.size(); i++){
			lamp3Position[i].y = terrain.getHeightTerrain(lamp3Position[i].x,lamp3Position[i].z);
			modelLamp3.setPosition(lamp3Position[i]);
			modelLamp3.setScale(glm::vec3(0.05));
			modelLamp3.setOrientation(glm::vec3(0, lamp3Orientation[i], 0));
			modelLamp3.render();
			
		}

		for(int i=0; i<lamp4Position.size(); i++){
			lamp4Position[i].y = terrain.getHeightTerrain(lamp4Position[i].x,lamp4Position[i].z);
			modelLamp4.setPosition(lamp4Position[i]);
			modelLamp4.setScale(glm::vec3(0.05));
			modelLamp4.setOrientation(glm::vec3(0, lamp4Orientation[i], 0));
			modelLamp4.render();
			
		}


	// Dart lego
	// Se deshabilita el cull faces IMPORTANTE para la capa
	glDisable(GL_CULL_FACE);
	modelMatrixDart[3][1] = terrain.getHeightTerrain(modelMatrixDart[3][0], modelMatrixDart[3][2]);
	glm::mat4 modelMatrixDartBody = glm::mat4(modelMatrixDart);
	modelMatrixDartBody = glm::scale(modelMatrixDartBody, glm::vec3(0.5, 0.5, 0.5));
	//modelDartLegoBody.render(modelMatrixDartBody);
	glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDartBody);
	modelMatrixDartHead = glm::rotate(modelMatrixDartHead, rotDartHead, glm::vec3(0, 1, 0));
	//modelDartLegoHead.render(modelMatrixDartHead);
	//modelDartLegoMask.render(modelMatrixDartHead);
	glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDartBody);
	modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
	modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
	modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
	modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
	//modelDartLegoLeftArm.render(modelMatrixDartLeftArm);
	glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
	modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
	modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
	modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
	modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
	//modelDartLegoLeftHand.render(modelMatrixDartLeftHand);
	glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDartBody);
	modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
	modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
	modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
	modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
	//modelDartLegoRightArm.render(modelMatrixDartRightArm);
	glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
	modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
	modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
	modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
	modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
	//modelDartLegoRightHand.render(modelMatrixDartRightHand);
	glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDartBody);
	modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
	modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
	modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
	//modelDartLegoLeftLeg.render(modelMatrixDartLeftLeg);
	glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDartBody);
	modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
	modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
	modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
	//modelDartLegoRightLeg.render(modelMatrixDartRightLeg);
	// Se regresa el cull faces IMPORTANTE para la capa
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	glm::mat4 modelMatrixTorso = glm::mat4(modelMatrixBuzz);
	modelMatrixTorso = glm::scale(modelMatrixTorso, glm::vec3(3.0));
	//modelBuzzTorso.render(modelMatrixTorso);
	glm::mat4 modelMatrizHead = glm::mat4(modelMatrixTorso);
	modelMatrizHead = glm::translate(modelMatrizHead, glm::vec3(0, 0, -0.017506));
	modelMatrizHead = glm::rotate(modelMatrizHead, rotBuzzHead, glm::vec3(0, 1, 0));
	modelMatrizHead = glm::translate(modelMatrizHead, glm::vec3(0, 0, 0.017506));
	//modelBuzzHead.render(modelMatrizHead);

	glm::mat4 modelMatrixLeftArm = glm::mat4(modelMatrixTorso);
	modelMatrixLeftArm = glm::translate(modelMatrixLeftArm, glm::vec3(0.179974, 0.577592, -0.022103));
	modelMatrixLeftArm = glm::rotate(modelMatrixLeftArm, glm::radians(-65.0f), glm::vec3(0, 0, 1));
	modelMatrixLeftArm = glm::rotate(modelMatrixLeftArm, rotBuzzLeftarm, glm::vec3(0, 1, 0));
	modelMatrixLeftArm = glm::translate(modelMatrixLeftArm, glm::vec3(-0.179974, -0.577592, 0.022103));
	//modelBuzzLeftArm.render(modelMatrixLeftArm);

	glm::mat4 modelMatrixLeftForeArm = glm::mat4(modelMatrixLeftArm);
	modelMatrixLeftForeArm = glm::translate(modelMatrixLeftForeArm, glm::vec3(0.298368, 0.583773, 0.008465));
	modelMatrixLeftForeArm = glm::rotate(modelMatrixLeftForeArm, rotBuzzLeftForeArm, glm::vec3(0, 1, 0));
	modelMatrixLeftForeArm = glm::translate(modelMatrixLeftForeArm, glm::vec3(-0.298368, -0.583773, -0.008465));
	//modelBuzzLeftForeArm.render(modelMatrixLeftForeArm);

	glm::mat4 modelMatrixLeftHand = glm::mat4(modelMatrixLeftForeArm);
	modelMatrixLeftHand = glm::translate(modelMatrixLeftHand, glm::vec3(0.416066, 0.587046, 0.076258));
	modelMatrixLeftHand = glm::rotate(modelMatrixLeftHand, glm::radians(45.0f), glm::vec3(0, 1, 0));
	modelMatrixLeftHand = glm::rotate(modelMatrixLeftHand, rotBuzzLeftHand, glm::vec3(1, 0, 0));
	modelMatrixLeftHand = glm::rotate(modelMatrixLeftHand, glm::radians(-45.0f), glm::vec3(0, 1, 0));
	modelMatrixLeftHand = glm::translate(modelMatrixLeftHand, glm::vec3(-0.416066, -0.587046, -0.076258));
	//modelBuzzLeftHand.render(modelMatrixLeftHand);
	glEnable(GL_CULL_FACE);
	/*****************************************
	 * Objetos animados por huesos
	 * **************************************/
	//*********** Primeramente LUFFY
	glDisable(GL_CULL_FACE);
	glm::vec3 ejey = glm::normalize(terrain.getNormalTerrain(modelMatrixLuffy[3][0], modelMatrixLuffy[3][2]));
	glm::vec3 ejex = glm::vec3(modelMatrixLuffy[0]);
	glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	modelMatrixLuffy[0] = glm::vec4(ejex, 0.0);
	modelMatrixLuffy[1] = glm::vec4(ejey, 0.0);
	modelMatrixLuffy[2] = glm::vec4(ejez, 0.0);
	modelMatrixLuffy[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixLuffy[3][0], modelMatrixLuffy[3][2]);
	tmv = currTime - startTimeJump;
	if(modelMatrixLuffy[3][1] < terrain.getHeightTerrain(modelMatrixLuffy[3][0], modelMatrixLuffy[3][2])){
		isJump = false;
		modelMatrixLuffy[3][1] = terrain.getHeightTerrain(modelMatrixLuffy[3][0], modelMatrixLuffy[3][2]);
	}
	glm::mat4 modelMatrixLuffyBody = glm::mat4(modelMatrixLuffy);
	modelMatrixLuffyBody = glm::scale(modelMatrixLuffyBody, glm::vec3(0.009f));//.021
	luffyModelAnimate.setAnimationIndex(animationLuffyIndex);
	luffyModelAnimate.render(modelMatrixLuffyBody);
	//animationLuffyIndex = 1;
	glEnable(GL_CULL_FACE);

	//***Termina LUFFY

	glDisable(GL_CULL_FACE);
	modelMatrixCowboy[3][1] = terrain.getHeightTerrain(modelMatrixCowboy[3][0], modelMatrixCowboy[3][2]);
	glm::mat4 modelMatrixCowboyBody = glm::mat4(modelMatrixCowboy);
	modelMatrixCowboyBody = glm::scale(modelMatrixCowboyBody, glm::vec3(0.0021f));
	//cowboyModelAnimate.render(modelMatrixCowboyBody);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixGuardian[3][1] = terrain.getHeightTerrain(modelMatrixGuardian[3][0], modelMatrixGuardian[3][2]);
	glm::mat4 modelMatrixGuardianBody = glm::mat4(modelMatrixGuardian);
	modelMatrixGuardianBody = glm::scale(modelMatrixGuardianBody, glm::vec3(0.04f));
	//guardianModelAnimate.render(modelMatrixGuardianBody);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixZombi[3][1] = terrain.getHeightTerrain(modelMatrixZombi[3][0], modelMatrixZombi[3][2]);
	glm::mat4 modelMatrixZombiBody = glm::mat4(modelMatrixZombi);
	//if(!limite_acercar)
	//	modelMatrixZombiBody = glm::translate(modelMatrixZombiBody, glm::vec3(x1, 0.0f, 0.0f)); //<<para acercarse a Luffy
	modelMatrixZombiBody = glm::scale(modelMatrixZombiBody, glm::vec3(0.015f));
	zombiModelAnimate.setAnimationIndex(0);
	if(visibleZombi)
		zombiModelAnimate.render(modelMatrixZombiBody);
	glEnable(GL_CULL_FACE);

	//**** INICIA personaje ENEMY
	//if(modelSelected==5){
	glDisable(GL_CULL_FACE);
	modelMatrixEnemy[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixEnemy[3][0], modelMatrixEnemy[3][2]);
	tmv = currTime - startTimeJump;
					
	if(modelMatrixEnemy[3][1] < terrain.getHeightTerrain(modelMatrixEnemy[3][0], modelMatrixEnemy[3][2])){
		isJump = false;
		modelMatrixEnemy[3][1] = terrain.getHeightTerrain(modelMatrixEnemy[3][0], modelMatrixEnemy[3][2]);
	}
	glm::vec3 ejeyEnemy = glm::normalize(terrain.getNormalTerrain(modelMatrixEnemy[3][0], modelMatrixEnemy[3][2]));
	glm::vec3 ejexEnemy = glm::vec3(modelMatrixEnemy[0]);
	glm::vec3 ejezEnemy = glm::normalize(glm::cross(ejexEnemy, ejeyEnemy));
	ejexEnemy = glm::normalize(glm::cross(ejeyEnemy, ejezEnemy));
	modelMatrixEnemy[0] = glm::vec4(ejexEnemy, 0.0);
	modelMatrixEnemy[1] = glm::vec4(ejeyEnemy, 0.0);
	modelMatrixEnemy[2] = glm::vec4(ejezEnemy, 0.0);
	
	modelMatrixEnemy[3][1] = terrain.getHeightTerrain(modelMatrixEnemy[3][0], modelMatrixEnemy[3][2]);
	glm::mat4 modelMatrixEnemyBody = glm::mat4(modelMatrixEnemy);
	modelMatrixEnemyBody = glm::rotate(modelMatrixEnemyBody, glm::radians(-90.0f), glm::vec3(0,1,0));
	modelMatrixEnemyBody = glm::scale(modelMatrixEnemyBody, glm::vec3(0.5f));
	enemyModelAnimate.setAnimationIndex(animationEnemyIndex);
	enemyModelAnimate.render(modelMatrixEnemyBody);
	glEnable(GL_CULL_FACE);

	//******** Termina ENEMY

	// Grass
	glDisable(GL_CULL_FACE);
	glm::vec3 grassPosition = glm::vec3(0.0, 0.0, 0.0);
	grassPosition.y = terrain.getHeightTerrain(grassPosition.x, grassPosition.z);
	modelGrass.setPosition(grassPosition);
	//modelGrass.render();
	glEnable(GL_CULL_FACE);


	// Fountain
	glDisable(GL_CULL_FACE);
	modelMatrixFountain[3][1] = terrain.getHeightTerrain(modelMatrixFountain[3][0] , modelMatrixFountain[3][2]) + 0.2;
	glm::mat4 modelMatrixFountainCopy = glm::scale(modelMatrixFountain, glm::vec3(1.5f, 1.5f, 1.5f));
	modelFountain.render(modelMatrixFountainCopy);
	glEnable(GL_CULL_FACE);

	// Fruta puntos
	glDisable(GL_CULL_FACE);
	modelMatrixFruta[3][1] = terrain.getHeightTerrain(modelMatrixFruta[3][0] , modelMatrixFruta[3][2]);
	glm::mat4 modelMatrixFrutaCopy = glm::scale(modelMatrixFruta, glm::vec3(2.0f, 2.0f, 2.0f));
	if(visibleFruta1)
		modelFruta.render(modelMatrixFrutaCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixFruta2[3][1] = terrain.getHeightTerrain(modelMatrixFruta2[3][0] , modelMatrixFruta2[3][2]);
	glm::mat4 modelMatrixFruta2Copy = glm::scale(modelMatrixFruta2, glm::vec3(2.0f, 2.0f, 2.0f));
	if(visibleFruta2)
		modelFruta.render(modelMatrixFruta2Copy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixFruta3[3][1] = terrain.getHeightTerrain(modelMatrixFruta3[3][0] , modelMatrixFruta3[3][2]);
	glm::mat4 modelMatrixFruta3Copy = glm::scale(modelMatrixFruta3, glm::vec3(2.0f, 2.0f, 2.0f));
	if(visibleFruta3)
		modelFruta.render(modelMatrixFruta3Copy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixFruta4[3][1] = terrain.getHeightTerrain(modelMatrixFruta4[3][0] , modelMatrixFruta4[3][2]);
	glm::mat4 modelMatrixFruta4Copy = glm::scale(modelMatrixFruta4, glm::vec3(2.0f, 2.0f, 2.0f));
	if(visibleFruta4)
		modelFruta.render(modelMatrixFruta4Copy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixFruta5[3][1] = terrain.getHeightTerrain(modelMatrixFruta5[3][0] , modelMatrixFruta5[3][2]);
	glm::mat4 modelMatrixFruta5Copy = glm::scale(modelMatrixFruta5, glm::vec3(2.0f, 2.0f, 2.0f));
	if(visibleFruta5)
		modelFruta.render(modelMatrixFruta5Copy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixFruta6[3][1] = terrain.getHeightTerrain(modelMatrixFruta6[3][0] , modelMatrixFruta6[3][2]);
	glm::mat4 modelMatrixFruta6Copy = glm::scale(modelMatrixFruta6, glm::vec3(2.0f, 2.0f, 2.0f));
	if(visibleFruta6)
		modelFruta.render(modelMatrixFruta6Copy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixFruta7[3][1] = terrain.getHeightTerrain(modelMatrixFruta7[3][0] , modelMatrixFruta7[3][2]);
	glm::mat4 modelMatrixFruta7Copy = glm::scale(modelMatrixFruta7, glm::vec3(2.0f, 2.0f, 2.0f));
	if(visibleFruta7)
		modelFruta.render(modelMatrixFruta7Copy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixFruta8[3][1] = terrain.getHeightTerrain(modelMatrixFruta8[3][0] , modelMatrixFruta8[3][2]);
	glm::mat4 modelMatrixFruta8Copy = glm::scale(modelMatrixFruta8, glm::vec3(2.0f, 2.0f, 2.0f));
	if(visibleFruta8)
		modelFruta.render(modelMatrixFruta8Copy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixFruta9[3][1] = terrain.getHeightTerrain(modelMatrixFruta9[3][0] , modelMatrixFruta9[3][2]);
	glm::mat4 modelMatrixFruta9Copy = glm::scale(modelMatrixFruta9, glm::vec3(2.0f, 2.0f, 2.0f));
	if(visibleFruta9)
		modelFruta.render(modelMatrixFruta9Copy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixArbol1a[3][1] = terrain.getHeightTerrain(modelMatrixArbol1a[3][0] , modelMatrixArbol1a[3][2]);
	glm::mat4 modelMatrixArbol1aCopy = glm::scale(modelMatrixArbol1a, glm::vec3(10.0f));
	modelArbol1a.render(modelMatrixArbol1aCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixArbol1b[3][1] = terrain.getHeightTerrain(modelMatrixArbol1b[3][0] , modelMatrixArbol1b[3][2]);
	glm::mat4 modelMatrixArbol1bCopy = glm::scale(modelMatrixArbol1b, glm::vec3(10.0f));
	modelArbol1b.render(modelMatrixArbol1bCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixArbol1c[3][1] = terrain.getHeightTerrain(modelMatrixArbol1c[3][0] , modelMatrixArbol1c[3][2]);
	glm::mat4 modelMatrixArbol1cCopy = glm::scale(modelMatrixArbol1c, glm::vec3(10.0f));
	modelArbol1c.render(modelMatrixArbol1cCopy);
	glEnable(GL_CULL_FACE);


	glDisable(GL_CULL_FACE);
	modelMatrixArbol2a[3][1] = terrain.getHeightTerrain(modelMatrixArbol2a[3][0] , modelMatrixArbol2a[3][2]);
	glm::mat4 modelMatrixArbol2aCopy = glm::scale(modelMatrixArbol2a, glm::vec3(10.0f));
	modelArbol2a.render(modelMatrixArbol2aCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixArbol2b[3][1] = terrain.getHeightTerrain(modelMatrixArbol2b[3][0] , modelMatrixArbol2b[3][2]);
	glm::mat4 modelMatrixArbol2bCopy = glm::scale(modelMatrixArbol2b, glm::vec3(10.0f));
	modelArbol2b.render(modelMatrixArbol2bCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixArbol2c[3][1] = terrain.getHeightTerrain(modelMatrixArbol2c[3][0] , modelMatrixArbol2c[3][2]);
	glm::mat4 modelMatrixArbol2cCopy = glm::scale(modelMatrixArbol2c, glm::vec3(10.0f));
	modelArbol2c.render(modelMatrixArbol2cCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixArbol3a[3][1] = terrain.getHeightTerrain(modelMatrixArbol3a[3][0] , modelMatrixArbol3a[3][2]);
	glm::mat4 modelMatrixArbol3aCopy = glm::scale(modelMatrixArbol3a, glm::vec3(10.0f));
	modelArbol3a.render(modelMatrixArbol3aCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixArbol3b[3][1] = terrain.getHeightTerrain(modelMatrixArbol3b[3][0] , modelMatrixArbol3b[3][2]);
	glm::mat4 modelMatrixArbol3bCopy = glm::scale(modelMatrixArbol3b, glm::vec3(10.0f));
	modelArbol3b.render(modelMatrixArbol3bCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixAtaud1a[3][1] = terrain.getHeightTerrain(modelMatrixAtaud1a[3][0] , modelMatrixAtaud1a[3][2]);
	glm::mat4 modelMatrixAtaud1aCopy = glm::scale(modelMatrixAtaud1a, glm::vec3(2.0f, 2.0f, 2.0f));
	modelAtaud1a.render(modelMatrixAtaud1aCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixAtaud1b[3][1] = terrain.getHeightTerrain(modelMatrixAtaud1b[3][0] , modelMatrixAtaud1b[3][2]);
	glm::mat4 modelMatrixAtaud1bCopy = glm::scale(modelMatrixAtaud1b, glm::vec3(2.0f, 2.0f, 2.0f));
	modelAtaud1b.render(modelMatrixAtaud1bCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixAtaud1c[3][1] = terrain.getHeightTerrain(modelMatrixAtaud1c[3][0] , modelMatrixAtaud1c[3][2]);
	glm::mat4 modelMatrixAtaud1cCopy = glm::scale(modelMatrixAtaud1c, glm::vec3(2.0f, 2.0f, 2.0f));
	modelAtaud1c.render(modelMatrixAtaud1cCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixAtaud1d[3][1] = terrain.getHeightTerrain(modelMatrixAtaud1d[3][0] , modelMatrixAtaud1d[3][2]);
	glm::mat4 modelMatrixAtaud1dCopy = glm::scale(modelMatrixAtaud1d, glm::vec3(2.0f, 2.0f, 2.0f));
	modelAtaud1d.render(modelMatrixAtaud1dCopy);
	glEnable(GL_CULL_FACE);
	

	glDisable(GL_CULL_FACE);
	modelMatrixAtaud2a[3][1] = terrain.getHeightTerrain(modelMatrixAtaud2a[3][0] , modelMatrixAtaud2a[3][2]);
	glm::mat4 modelMatrixAtaud2aCopy = glm::scale(modelMatrixAtaud2a, glm::vec3(2.0f, 2.0f, 2.0f));
	modelAtaud2a.render(modelMatrixAtaud2aCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixAtaud2b[3][1] = terrain.getHeightTerrain(modelMatrixAtaud2b[3][0] , modelMatrixAtaud2b[3][2]);
	glm::mat4 modelMatrixAtaud2bCopy = glm::scale(modelMatrixAtaud2b, glm::vec3(2.0f, 2.0f, 2.0f));
	modelAtaud2b.render(modelMatrixAtaud2bCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixAtaud2c[3][1] = terrain.getHeightTerrain(modelMatrixAtaud2c[3][0] , modelMatrixAtaud2c[3][2]);
	glm::mat4 modelMatrixAtaud2cCopy = glm::scale(modelMatrixAtaud2c, glm::vec3(2.0f, 2.0f, 2.0f));
	modelAtaud2c.render(modelMatrixAtaud2cCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixAtaud2d[3][1] = terrain.getHeightTerrain(modelMatrixAtaud2d[3][0] , modelMatrixAtaud2d[3][2]);
	glm::mat4 modelMatrixAtaud2dCopy = glm::scale(modelMatrixAtaud2d, glm::vec3(2.0f, 2.0f, 2.0f));
	modelAtaud2d.render(modelMatrixAtaud2dCopy);
	glEnable(GL_CULL_FACE);
	
	glDisable(GL_CULL_FACE);
	modelMatrixArco[3][1] = terrain.getHeightTerrain(modelMatrixArco[3][0] , modelMatrixArco[3][2]);
	glm::mat4 modelMatrixArcoCopy = glm::scale(modelMatrixArco, glm::vec3(3.0f, 3.0f, 3.0f));
	modelArco.render(modelMatrixArcoCopy);
	glEnable(GL_CULL_FACE);
	
	//renderizado de Zombis
	glDisable(GL_CULL_FACE);
	modelMatrixZombi_1a[3][1] = terrain.getHeightTerrain(modelMatrixZombi_1a[3][0] , modelMatrixZombi_1a[3][2]);
	glm::mat4 modelMatrixZombi_1aCopy = glm::scale(modelMatrixZombi_1a, glm::vec3(0.015f));
	if(visibleZombi_1a)
		modelZombi_1a.render(modelMatrixZombi_1aCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixZombi_1b[3][1] = terrain.getHeightTerrain(modelMatrixZombi_1b[3][0] , modelMatrixZombi_1b[3][2]);
	glm::mat4 modelMatrixZombi_1bCopy = glm::scale(modelMatrixZombi_1b, glm::vec3(0.015f));
	if(visibleZombi_1b)
		modelZombi_1b.render(modelMatrixZombi_1bCopy);
	glEnable(GL_CULL_FACE);


	glDisable(GL_CULL_FACE);
	modelMatrixZombi_1c[3][1] = terrain.getHeightTerrain(modelMatrixZombi_1c[3][0] , modelMatrixZombi_1c[3][2]);
	glm::mat4 modelMatrixZombi_1cCopy = glm::scale(modelMatrixZombi_1c, glm::vec3(0.015f));
	if(visibleZombi_1c)
		modelZombi_1c.render(modelMatrixZombi_1cCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixZombi_1d[3][1] = terrain.getHeightTerrain(modelMatrixZombi_1d[3][0] , modelMatrixZombi_1d[3][2]);
	glm::mat4 modelMatrixZombi_1dCopy = glm::scale(modelMatrixZombi_1d, glm::vec3(0.015f));
	if(visibleZombi_1d)
		modelZombi_1d.render(modelMatrixZombi_1dCopy);
	glEnable(GL_CULL_FACE);
	
	glDisable(GL_CULL_FACE);
	modelMatrixZombi_2a[3][1] = terrain.getHeightTerrain(modelMatrixZombi_2a[3][0] , modelMatrixZombi_2a[3][2]);
	glm::mat4 modelMatrixZombi_2aCopy = glm::scale(modelMatrixZombi_2a, glm::vec3(0.015f));
	if(visibleZombi_2a)
		modelZombi_2a.render(modelMatrixZombi_2aCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixZombi_2b[3][1] = terrain.getHeightTerrain(modelMatrixZombi_2b[3][0] , modelMatrixZombi_2b[3][2]);
	glm::mat4 modelMatrixZombi_2bCopy = glm::scale(modelMatrixZombi_2b, glm::vec3(0.015f));
	if(visibleZombi_2b)
		modelZombi_2b.render(modelMatrixZombi_2bCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixZombi_2c[3][1] = terrain.getHeightTerrain(modelMatrixZombi_2c[3][0] , modelMatrixZombi_2c[3][2]);
	glm::mat4 modelMatrixZombi_2cCopy = glm::scale(modelMatrixZombi_2c, glm::vec3(0.015f));
	if(visibleZombi_2c)
		modelZombi_2c.render(modelMatrixZombi_2cCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixZombi_3a[3][1] = terrain.getHeightTerrain(modelMatrixZombi_3a[3][0] , modelMatrixZombi_3a[3][2]);
	glm::mat4 modelMatrixZombi_3aCopy = glm::scale(modelMatrixZombi_3a, glm::vec3(0.015f));
	if(visibleZombi_3a)
		modelZombi_3a.render(modelMatrixZombi_3aCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixZombi_3b[3][1] = terrain.getHeightTerrain(modelMatrixZombi_3b[3][0] , modelMatrixZombi_3b[3][2]);
	glm::mat4 modelMatrixZombi_3bCopy = glm::scale(modelMatrixZombi_3b, glm::vec3(0.015f));
	if(visibleZombi_3b)
		modelZombi_3b.render(modelMatrixZombi_3bCopy);
	glEnable(GL_CULL_FACE);

	glDisable(GL_CULL_FACE);
	modelMatrixZombi_3c[3][1] = terrain.getHeightTerrain(modelMatrixZombi_3c[3][0] , modelMatrixZombi_3c[3][2]);
	glm::mat4 modelMatrixZombi_3cCopy = glm::scale(modelMatrixZombi_3c, glm::vec3(0.015f));
	if(visibleZombi_3c)
		modelZombi_3c.render(modelMatrixZombi_3cCopy);
	glEnable(GL_CULL_FACE);

	

	/*******************************************
	 * Skybox
	 *******************************************/
	GLint oldCullFaceMode;
	GLint oldDepthFuncMode;
	// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
	shaderSkybox.setFloat("skybox", 0);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glActiveTexture(GL_TEXTURE0);
	skyboxSphere.render();
	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}

//************ TErmina Renderizado de Objeto SÃƒÂ³lidos


//************ Inicia Renderizado de Objetos TRANSPARENTES
void renderAlphaScene(bool render){
	/**********
	 * Update the position with alpha objects
	 */
	// Update the aircraft
	blendingUnsorted.find("aircraft")->second = glm::vec3(modelMatrixAircraft[3]);
	// Update the lambo
	blendingUnsorted.find("lambo")->second = glm::vec3(modelMatrixLambo[3]);
	// Update the helicopter
	blendingUnsorted.find("heli")->second = glm::vec3(modelMatrixHeli[3]);
	//Update the fountain
	
	

	/********** Ordenamiento de Objetos Alpha (transparentes)
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for(itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++){
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Renderizado  de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	for(std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++){
		if(it->second.first.compare("aircraft") == 0){
			// Render for the aircraft model
			glm::mat4 modelMatrixAircraftBlend = glm::mat4(modelMatrixAircraft);
			modelMatrixAircraftBlend[3][1] = terrain.getHeightTerrain(modelMatrixAircraftBlend[3][0], modelMatrixAircraftBlend[3][2]) + 2.0;
			//modelAircraft.render(modelMatrixAircraftBlend);
		}
		else if(it->second.first.compare("lambo") == 0){
			// Lambo car
			glm::mat4 modelMatrixLamboBlend = glm::mat4(modelMatrixLambo);
			modelMatrixLamboBlend[3][1] = terrain.getHeightTerrain(modelMatrixLamboBlend[3][0], modelMatrixLamboBlend[3][2]);
			modelMatrixLamboBlend = glm::scale(modelMatrixLamboBlend, glm::vec3(1.3, 1.3, 1.3));
			//modelLambo.render(modelMatrixLamboBlend);
			glActiveTexture(GL_TEXTURE0);
			glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboBlend);
			modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08676, 0.707316, 0.982601));
			modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
			modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08676, -0.707316, -0.982601));
			//modelLamboLeftDor.render(modelMatrixLamboLeftDor);
			//modelLamboRightDor.render(modelMatrixLamboBlend);
			//modelLamboFrontLeftWheel.render(modelMatrixLamboBlend);
			//modelLamboFrontRightWheel.render(modelMatrixLamboBlend);
			//modelLamboRearLeftWheel.render(modelMatrixLamboBlend);
			//modelLamboRearRightWheel.render(modelMatrixLamboBlend);
			// Se regresa el cull faces IMPORTANTE para las puertas
		}
		else if(it->second.first.compare("heli") == 0){
			// Helicopter
			glm::mat4 modelMatrixHeliChasis = glm::mat4(modelMatrixHeli);
			//modelHeliChasis.render(modelMatrixHeliChasis);

			glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
			modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, -0.249548));
			modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, rotHelHelY, glm::vec3(0, 1, 0));
			modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, 0.249548));
			//modelHeliHeli.render(modelMatrixHeliHeli);
		}

		else if(render && it->second.first.compare("fountain") == 0){
			/********** Renderizado de las particulas ALpha (Fuente, agua)
			 * Init Render particles systems
			 */
			glm::mat4 modelMatrixParticlesFountain = glm::mat4(1.0f);
			modelMatrixParticlesFountain = glm::translate(modelMatrixParticlesFountain, it->second.second);
			modelMatrixParticlesFountain[3][1] = terrain.getHeightTerrain(modelMatrixParticlesFountain[3][0], modelMatrixParticlesFountain[3][2]) + 0.36 * 10.0;
			modelMatrixParticlesFountain = glm::scale(modelMatrixParticlesFountain, glm::vec3(3.0, 3.0, 3.0));
			currTimeParticlesAnimation = TimeManager::Instance().GetTime();
			if(currTimeParticlesAnimation - lastTimeParticlesAnimation > 10.0)
				lastTimeParticlesAnimation = currTimeParticlesAnimation;
			//glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			// Set the point size
			glPointSize(10.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
			shaderParticlesFountain.turnOn();
			shaderParticlesFountain.setFloat("Time", float(currTimeParticlesAnimation - lastTimeParticlesAnimation));
			shaderParticlesFountain.setFloat("ParticleLifetime", 3.5f);
			shaderParticlesFountain.setInt("ParticleTex", 0);
			shaderParticlesFountain.setVectorFloat3("Gravity", glm::value_ptr(glm::vec3(0.0f, -0.3f, 0.0f)));
			shaderParticlesFountain.setMatrix4("model", 1, false, glm::value_ptr(modelMatrixParticlesFountain));
			glBindVertexArray(VAOParticles);
			glDrawArrays(GL_POINTS, 0, nParticles);
			glDepthMask(GL_TRUE);
			//glEnable(GL_DEPTH_TEST);
			shaderParticlesFountain.turnOff();
			/**********
			 * End Render particles systems
			 */
		}
		else if(render && it->second.first.compare("fire") == 0){
			/**********Renderizado de particulas Alpha Fuego(Fire)
			 * Init Render particles systems
			 */
			
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1-drawBuf]);
			glVertexAttribDivisor(0,0);
			glVertexAttribDivisor(1,0);
			glVertexAttribDivisor(2,0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0);
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));
			shaderParticlesFire.setVectorFloat3("colorHumo", glm::value_ptr(glm::vec3(0.59,0.02,0.59)));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0,1);
			glVertexAttribDivisor(1,1);
			glVertexAttribDivisor(2,1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

			/****************************Sonido para Particulas Fuego
			 * Open AL sound data
			 */
			source1Pos[0] = modelFireParticles[3].x;
			source1Pos[1] = modelFireParticles[3].y;
			source1Pos[2] = modelFireParticles[3].z;
			alSourcefv(source[1], AL_POSITION, source1Pos);

			/**********
			 * End Render particles systems
			 */
		}
	
	}
	glEnable(GL_CULL_FACE);

	glDisable(GL_BLEND);

	//Renderizado de la Pantalla de Entrada y de Ejecucion
	if(render){
		/************Render de imagen de frente**************/
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureActivaID);
		shaderTexture.setInt("outTexture", 0);
		glEnable(GL_BLEND);
		boxIntro.render();
		glDisable(GL_BLEND);

		//modelText->render("Texto en OpenGL", -1, 0);
		std::stringstream ss; ss <<puntuacion ;   //instruccion para obtener una entrada numero entero y convertirlo en cadena posteriormente
		modelText->render("Puntos: " +ss.str() , -1, 0);

		if(sumaColisionOBB_OBB>81)
		{
			std::stringstream ss; ss <<puntuacion ;   //instruccion para obtener una entrada numero entero y convertirlo en cadena posteriormente
			
			modelText->render("Puntos: " +ss.str()+"..............Perdiste JA JA JA JA JA JA.." , -1, 0);
			Sleep(1000);
			//gameOver = true;
			
		} else
		{
			std::stringstream ss; ss <<puntuacion ;   //instruccion para obtener una entrada numero entero y convertirlo en cadena posteriormente
			modelText->render("Puntos: " +ss.str() , -1, 0);
		} 

		
		
	}
	glEnable(GL_CULL_FACE);
}


//********** Renderizado de funciones SÃƒÂ³lidos y ALpha
void renderScene(){
	renderSolidScene();
	renderAlphaScene(false);
}


//************* Loop de aplicaciÃƒÂ³n
void applicationLoop() {
	bool psi = true;

	//*****variables para la camara
	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(27.5, 0, 30.0));
	modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(180.0f), glm::vec3(0, 1, 0));
	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;

	//************* Coordenadas de los modelos vec3

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 10.0, -5.0));

	modelMatrixAircraft = glm::translate(modelMatrixAircraft, glm::vec3(10.0, 2.0, -17.5));

	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(23.0, 0.0, 0.0));

	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));

	modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(15.0, 0.0, -10.0));

	modelMatrixLuffy = glm::translate(modelMatrixLuffy, glm::vec3(-77.34f, 0.05f, 45.80f));
	//modelMatrixLuffy = glm::rotate(modelMatrixLuffy, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixCowboy = glm::translate(modelMatrixCowboy, glm::vec3(13.0, 0.05, 0.0));

	modelMatrixGuardian = glm::translate(modelMatrixGuardian, glm::vec3(15, 0.05, 0.0));
	//modelMatrixGuardian = glm::rotate(modelMatrixGuardian, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));

	modelMatrixZombi = glm::translate(modelMatrixZombi, glm::vec3(5.0f, 0.0f,5.0f));
	//modelMatrixZombi = glm::rotate(modelMatrixZombi, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixEnemy = glm::translate(modelMatrixEnemy, glm::vec3(-76.33f,0.05f,-72.51f));
	
	modelMatrixFruta = glm::translate(modelMatrixFruta, glm::vec3(-97.65, 0.0, 94.75));
	//modelMatrixFruta = glm::rotate(modelMatrixFruta, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

	modelMatrixFruta2 = glm::translate(modelMatrixFruta2, glm::vec3(-32.53, 0.0, 25.76));

	modelMatrixFruta3 = glm::translate(modelMatrixFruta3, glm::vec3(2.57, 0.0, 89.31));
	modelMatrixFruta4 = glm::translate(modelMatrixFruta4, glm::vec3(41.79, 0.0, 86.45));
	modelMatrixFruta5 = glm::translate(modelMatrixFruta5, glm::vec3(92.17, 0.0, 72.13));
	modelMatrixFruta6 = glm::translate(modelMatrixFruta6, glm::vec3(89.02, 0.0, 37.5));
	modelMatrixFruta7 = glm::translate(modelMatrixFruta7, glm::vec3(-53.43, 0.0, -2.76));
	modelMatrixFruta8 = glm::translate(modelMatrixFruta8, glm::vec3(94.17, 0.0, -39.69));
	modelMatrixFruta9 = glm::translate(modelMatrixFruta9, glm::vec3(-94.94, 0.0, -95.41));


	modelMatrixArbol1a = glm::translate(modelMatrixArbol1a, glm::vec3(-78.41, 0.0, 79.58));
	modelMatrixArbol1b = glm::translate(modelMatrixArbol1b, glm::vec3(-9.63, 0.0, 13.45));
	modelMatrixArbol1c = glm::translate(modelMatrixArbol1c, glm::vec3(-46.85, 0.0, -36.54));

	modelMatrixArbol2a = glm::translate(modelMatrixArbol2a, glm::vec3(-86.64, 0.0, 29.48));
	modelMatrixArbol2b = glm::translate(modelMatrixArbol2b, glm::vec3(-15.36, 0.0, 89.06));
	modelMatrixArbol2c = glm::translate(modelMatrixArbol2c, glm::vec3(61.54, 0.0, 28.62));

	modelMatrixArbol3a = glm::translate(modelMatrixArbol3a, glm::vec3(-36.25, 0.0, 32.63));
	modelMatrixArbol3b = glm::translate(modelMatrixArbol3b, glm::vec3(37.21, 0.0, 60.68));

	modelMatrixAtaud1a = glm::translate(modelMatrixAtaud1a, glm::vec3(-47.65, 0.0, 70.08));
	modelMatrixAtaud1b = glm::translate(modelMatrixAtaud1b, glm::vec3(-15.07, 0.0, 62.11));
	modelMatrixAtaud1c = glm::translate(modelMatrixAtaud1c, glm::vec3(25.47, 0.0, 34.35));
	modelMatrixAtaud1d = glm::translate(modelMatrixAtaud1d, glm::vec3(65.55, 0.0, 15.17));

	modelMatrixAtaud2a = glm::translate(modelMatrixAtaud2a, glm::vec3(-31.96, 0.0, 48.95));
	modelMatrixAtaud2b = glm::translate(modelMatrixAtaud2b, glm::vec3(0.0, 0.0, 30.05));
	modelMatrixAtaud2c = glm::translate(modelMatrixAtaud2c, glm::vec3(38.35, 0.0, 9.16));
	modelMatrixAtaud2d = glm::translate(modelMatrixAtaud2d, glm::vec3(47.23, 0.0, -38.54));

	modelMatrixArco = glm::translate(modelMatrixArco, glm::vec3(-23.33, 0.0, 18.61));

	modelMatrixZombi_1a = glm::translate(modelMatrixZombi_1a, glm::vec3(-57.32, 0.0, 54.96)); 
	modelMatrixZombi_1b = glm::translate(modelMatrixZombi_1b, glm::vec3(10.87, 0.0, 54.11)); 
	modelMatrixZombi_1c = glm::translate(modelMatrixZombi_1c, glm::vec3(-17.08, 0.0, -44.56)); 
	modelMatrixZombi_1d = glm::translate(modelMatrixZombi_1d, glm::vec3(-41.79, 0.0, -80.62)); 
	modelMatrixZombi_2a = glm::translate(modelMatrixZombi_2a, glm::vec3(-28.24, 0.0, 64.69)); 
	modelMatrixZombi_2b = glm::translate(modelMatrixZombi_2b, glm::vec3(40.93, 0.0, 35.21)); 
	modelMatrixZombi_2c = glm::translate(modelMatrixZombi_2c, glm::vec3(-33.68, 0.0, -71.18)); 
	modelMatrixZombi_3a = glm::translate(modelMatrixZombi_3a, glm::vec3(-19.08, 0.0, 29.19)); 
	modelMatrixZombi_3b = glm::translate(modelMatrixZombi_3b, glm::vec3(-23.37, 0.0, -55.15)); 
	modelMatrixZombi_3c = glm::translate(modelMatrixZombi_3c, glm::vec3(54.96, 0.0, 15.74)); 
	
	
	modelMatrixFountain = glm::translate(modelMatrixFountain, glm::vec3(-1.07, 0.0, -3.62));

	// Variables to interpolation key frames
	fileName = "../animaciones/animation_dart_joints.txt";
	keyFramesDartJoints = getKeyRotFrames(fileName);
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");
	keyFramesBuzzJoints = getKeyRotFrames("../animaciones/animation_buzz_joints.txt");
	keyFramesBuzz = getKeyFrames("../animaciones/animation_buzz.txt");

	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	// IniciaciÃƒÂ³n de la pantalla de inicio
	textureActivaID = textureInit1ID;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, -10.0);

	//Para el manejo de las sombras de los objetos principales 
	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;
		std::vector<float> matrixBuzzJoints;
		std::vector<glm::mat4> matrixBuzz;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);

		if(modelSelected == 1){
			axis = glm::axis(glm::quat_cast(modelMatrixDart));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixDart));
			target = modelMatrixDart[3];
		}
		else{
			axis = glm::axis(glm::quat_cast(modelMatrixLuffy));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixLuffy));
			target = modelMatrixLuffy[3];
		}


		//***** Caracteristicas para la CAMARA 3a persona
		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		if(modelSelected == 1)
			angleTarget -= glm::radians(90.0f);

		//Cmabio de camara al presionar CTRL+K
		if (isThirdCamera==true)
		{
			camera->setAngleTarget(angleTarget);
			camera->setCameraTarget(target);
			camera->updateCamera();
			view = camera->getViewMatrix();
		}
		
		if(isThirdCamera==false)
		{
			view = camera1->getViewMatrix();
		}

		//camera->setCameraTarget(target);
		//camera->setAngleTarget(angleTarget);
		//camera->updateCamera();
		//glm::mat4 view = camera->getViewMatrix();

		shadowBox->update(screenWidth, screenHeight);
		glm::vec3 centerBox = shadowBox->getCenter();

		// Projection light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;
		lightView = glm::lookAt(centerBox, centerBox + glm::normalize(-lightPos), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		//*****************************
		//****Se configuran los shaders las Skybox, Multilighting, Terrain, 
		//   	Directional, Pointlight, de particles tanto Fountain y Fire 
		//******
		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para el fountain
		shaderParticlesFountain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));


		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderMulLighting.setFloat("density", 0.015);
		shaderMulLighting.setFloat("gradient", 0.5);
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderTerrain.setFloat("density", 0.015);
		shaderTerrain.setFloat("gradient", 0.5);
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.01, 0.01, 0.01)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.01, 0.01, 0.01)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderTerrain.setInt("spotLightCount", 1);
		glm::vec3 spotPosition = glm:: vec3(modelMatrixEnemy *  glm::vec4(0.0, 18.19, 0.0, 1.0));

		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.45, 0.3, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.6, 0.4, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.7, 0.5, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.09);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.02);
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(20.5f)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(25.0f)));

		shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.45, 0.3, 0.01)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.6, 0.4, 0.01)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.7, 0.5, 0.01)));
		shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderTerrain.setFloat("spotLights[0].constant", 1.0);
		shaderTerrain.setFloat("spotLights[0].linear", 0.09);
		shaderTerrain.setFloat("spotLights[0].quadratic", 0.02);
		shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(20.5f)));
		shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(25.0f)));

		/*
		LUCES LAMBO
		*/
		shaderMulLighting.setInt("spotLightCount", 2);
		shaderTerrain.setInt("spotLightCount", 2);
		glm::vec3 spotPositionLambo1 = glm:: vec3(modelMatrixLambo *  glm::vec4(0.76, 4.66, 5.2, 1.0));
		
		shaderMulLighting.setVectorFloat3("spotLights[1].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderMulLighting.setVectorFloat3("spotLights[1].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.03)));
		shaderMulLighting.setVectorFloat3("spotLights[1].position", glm::value_ptr(spotPositionLambo1));
		shaderMulLighting.setVectorFloat3("spotLights[1].direction", glm::value_ptr(spotPositionLambo1*glm::vec3(0.0,-1.0,0.0)));
		shaderMulLighting.setFloat("spotLights[1].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[1].linear", 0.09);
		shaderMulLighting.setFloat("spotLights[1].quadratic", 0.02);
		shaderMulLighting.setFloat("spotLights[1].cutOff", cos(glm::radians(20.0f)));
		shaderMulLighting.setFloat("spotLights[1].outerCutOff", cos(glm::radians(25.0f)));

		shaderTerrain.setVectorFloat3("spotLights[1].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderTerrain.setVectorFloat3("spotLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderTerrain.setVectorFloat3("spotLights[1].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.03)));
		shaderTerrain.setVectorFloat3("spotLights[1].position", glm::value_ptr(spotPositionLambo1));
		shaderTerrain.setVectorFloat3("spotLights[1].direction", glm::value_ptr(spotPositionLambo1*glm::vec3(0.0,-1.0,0.0)));
		shaderTerrain.setFloat("spotLights[1].constant", 1.0);
		shaderTerrain.setFloat("spotLights[1].linear", 0.09);
		shaderTerrain.setFloat("spotLights[1].quadratic", 0.02);
		shaderTerrain.setFloat("spotLights[1].cutOff", cos(glm::radians(20.0f)));
		shaderTerrain.setFloat("spotLights[1].outerCutOff", cos(glm::radians(25.0f)));

		shaderMulLighting.setInt("spotLightCount", 3);
		shaderTerrain.setInt("spotLightCount", 3);
		glm::vec3 spotPositionLambo2 = glm:: vec3(modelMatrixLambo *  glm::vec4(-0.76,4.66, 5.2, 1.0));
		
		shaderMulLighting.setVectorFloat3("spotLights[2].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderMulLighting.setVectorFloat3("spotLights[2].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.03)));
		shaderMulLighting.setVectorFloat3("spotLights[2].position", glm::value_ptr(spotPositionLambo2));
		shaderMulLighting.setVectorFloat3("spotLights[2].direction", glm::value_ptr(spotPositionLambo2*glm::vec3(0.0,-1.0,0.0)));
		shaderMulLighting.setFloat("spotLights[2].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[2].linear", 0.09);
		shaderMulLighting.setFloat("spotLights[2].quadratic", 0.02);
		shaderMulLighting.setFloat("spotLights[2].cutOff", cos(glm::radians(20.0f)));
		shaderMulLighting.setFloat("spotLights[2].outerCutOff", cos(glm::radians(25.0f)));

		shaderTerrain.setVectorFloat3("spotLights[2].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderTerrain.setVectorFloat3("spotLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderTerrain.setVectorFloat3("spotLights[2].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.03)));
		shaderTerrain.setVectorFloat3("spotLights[2].position", glm::value_ptr(spotPositionLambo2));
		shaderTerrain.setVectorFloat3("spotLights[2].direction", glm::value_ptr(spotPositionLambo2*glm::vec3(0.0,-1.0,0.0)));
		shaderTerrain.setFloat("spotLights[2].constant", 1.0);
		shaderTerrain.setFloat("spotLights[2].linear", 0.09);
		shaderTerrain.setFloat("spotLights[2].quadratic", 0.02);
		shaderTerrain.setFloat("spotLights[2].cutOff", cos(glm::radians(20.0f)));
		shaderTerrain.setFloat("spotLights[2].outerCutOff", cos(glm::radians(25.0f)));


		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", lamp1Position.size()+ lamp2Position.size()+lamp3Position.size()+lamp4Position.size());
		shaderTerrain.setInt("pointLightCount", lamp1Position.size()+ lamp2Position.size()+lamp3Position.size()+lamp4Position.size());
		
		for (int i = 0; i < lamp1Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp1Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(0.5, 0.5, 0.5));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0, 10.3585, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}

		
		for (int i = 0; i < lamp2Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
		}
		//shaderMulLighting.setInt("pointLightCount", 3);
		//shaderTerrain.setInt("pointLightCount", 3);
		for (int i = 0; i < lamp3Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp3Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp3Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size() + i) + "].quadratic", 0.02);
		}
		//shaderMulLighting.setInt("pointLightCount", 4);
		//shaderTerrain.setInt("pointLightCount", 4);
		for (int i = 0; i < lamp4Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp4Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp4Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size()+lamp2Position.size()+lamp3Position.size() + i) + "].quadratic", 0.02);
		}


		//Renderizado de la imagen de enfrente FIGURAS de FRUTAS al frente

		/************Render de imagen de frente**************/
		if(!iniciaPartida){
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureActivaID);
			shaderTexture.setInt("outTexture", 0);
			boxIntro.render();
			glfwSwapBuffers(window);
			continue;
		}

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene();
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		// reset viewport
		/*glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render Depth map to quad for visual debugging
		shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setFloat("near_plane", near_plane);
		shaderViewDepth.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		boxViewDepth.render();*/

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		renderSolidScene();

		

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/
		// Collider del dart vader lego
		/*glm::mat4 modelmatrixColliderDart = glm::mat4(modelMatrixDart);
		AbstractModel::OBB dartLegoBodyCollider;
		// Set the orientation of collider before doing the scale
		dartLegoBodyCollider.u = glm::quat_cast(modelMatrixDart);
		modelmatrixColliderDart = glm::scale(modelmatrixColliderDart, glm::vec3(0.5, 0.5, 0.5));
		modelmatrixColliderDart = glm::translate(modelmatrixColliderDart,
				glm::vec3(modelDartLegoBody.getObb().c.x,
						modelDartLegoBody.getObb().c.y,
						modelDartLegoBody.getObb().c.z));
		dartLegoBodyCollider.c = glm::vec3(modelmatrixColliderDart[3]);
		dartLegoBodyCollider.e = modelDartLegoBody.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		addOrUpdateColliders(collidersOBB, "dart", dartLegoBodyCollider, modelMatrixDart);*/

		// Collider del aricraft
		/*glm::mat4 modelMatrixColliderAircraft = glm::mat4(modelMatrixAircraft);
		AbstractModel::OBB aircraftCollider;
		// Set the orientation of collider before doing the scale
		aircraftCollider.u = glm::quat_cast(modelMatrixAircraft);
		modelMatrixColliderAircraft = glm::scale(modelMatrixColliderAircraft,
				glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderAircraft = glm::translate(
				modelMatrixColliderAircraft, modelAircraft.getObb().c);
		aircraftCollider.c = glm::vec3(modelMatrixColliderAircraft[3]);
		aircraftCollider.e = modelAircraft.getObb().e * glm::vec3(1.0, 1.0, 1.0);
		addOrUpdateColliders(collidersOBB, "aircraft", aircraftCollider, modelMatrixAircraft);*/

		//Collider del la rock
		/*AbstractModel::SBB rockCollider;
		glm::mat4 modelMatrixColliderRock= glm::mat4(matrixModelRock);
		modelMatrixColliderRock = glm::scale(modelMatrixColliderRock, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderRock = glm::translate(modelMatrixColliderRock, modelRock.getSbb().c);
		rockCollider.c = glm::vec3(modelMatrixColliderRock[3]);
		rockCollider.ratio = modelRock.getSbb().ratio * 1.0;
		addOrUpdateColliders(collidersSBB, "rock", rockCollider, matrixModelRock);*/


		// Collider del FRUTA puntos  <<<<<<<<<<Colisionadores de FRUTA Morada de PUNTOS
		
		AbstractModel::SBB fruta1Collider;
		glm::mat4 modelMatrixColliderFruta1= glm::mat4(modelMatrixFruta);
		modelMatrixColliderFruta1 = glm::scale(modelMatrixColliderFruta1, glm::vec3(1.2, 1.2, 1.2));
		modelMatrixColliderFruta1 = glm::translate(modelMatrixColliderFruta1, modelFruta.getSbb().c);
		fruta1Collider.c = glm::vec3(modelMatrixColliderFruta1[3]);
		fruta1Collider.ratio = modelFruta.getSbb().ratio * 1.2;
		addOrUpdateColliders(collidersSBB, "fruta1", fruta1Collider, modelMatrixFruta);
		

		AbstractModel::SBB fruta2Collider;
		glm::mat4 modelMatrixColliderFruta2= glm::mat4(modelMatrixFruta2);
		modelMatrixColliderFruta2 = glm::scale(modelMatrixColliderFruta2, glm::vec3(1.2, 1.2, 1.2));
		modelMatrixColliderFruta2 = glm::translate(modelMatrixColliderFruta2, modelFruta.getSbb().c);
		fruta2Collider.c = glm::vec3(modelMatrixColliderFruta2[3]);
		fruta2Collider.ratio = modelFruta.getSbb().ratio * 1.2;
		addOrUpdateColliders(collidersSBB, "fruta2", fruta2Collider, modelMatrixFruta2);

		AbstractModel::SBB fruta3Collider;
		glm::mat4 modelMatrixColliderFruta3= glm::mat4(modelMatrixFruta3);
		modelMatrixColliderFruta3 = glm::scale(modelMatrixColliderFruta3, glm::vec3(1.2, 1.2, 1.2));
		modelMatrixColliderFruta3 = glm::translate(modelMatrixColliderFruta3, modelFruta.getSbb().c);
		fruta3Collider.c = glm::vec3(modelMatrixColliderFruta3[3]);
		fruta3Collider.ratio = modelFruta.getSbb().ratio * 1.2;
		addOrUpdateColliders(collidersSBB, "fruta3", fruta3Collider, modelMatrixFruta3);

		AbstractModel::SBB fruta4Collider;
		glm::mat4 modelMatrixColliderFruta4= glm::mat4(modelMatrixFruta4);
		modelMatrixColliderFruta4 = glm::scale(modelMatrixColliderFruta4, glm::vec3(1.2, 1.2, 1.2));
		modelMatrixColliderFruta4 = glm::translate(modelMatrixColliderFruta4, modelFruta.getSbb().c);
		fruta4Collider.c = glm::vec3(modelMatrixColliderFruta4[3]);
		fruta4Collider.ratio = modelFruta.getSbb().ratio * 1.2;
		addOrUpdateColliders(collidersSBB, "fruta4", fruta4Collider, modelMatrixFruta4);

		AbstractModel::SBB fruta5Collider;
		glm::mat4 modelMatrixColliderFruta5= glm::mat4(modelMatrixFruta5);
		modelMatrixColliderFruta5 = glm::scale(modelMatrixColliderFruta5, glm::vec3(1.2, 1.2, 1.2));
		modelMatrixColliderFruta5 = glm::translate(modelMatrixColliderFruta5, modelFruta.getSbb().c);
		fruta5Collider.c = glm::vec3(modelMatrixColliderFruta5[3]);
		fruta5Collider.ratio = modelFruta.getSbb().ratio * 1.2;
		addOrUpdateColliders(collidersSBB, "fruta5", fruta5Collider, modelMatrixFruta5);	

		AbstractModel::SBB fruta6Collider;
		glm::mat4 modelMatrixColliderFruta6= glm::mat4(modelMatrixFruta6);
		modelMatrixColliderFruta6 = glm::scale(modelMatrixColliderFruta6, glm::vec3(1.2, 1.2, 1.2));
		modelMatrixColliderFruta6 = glm::translate(modelMatrixColliderFruta6, modelFruta.getSbb().c);
		fruta6Collider.c = glm::vec3(modelMatrixColliderFruta6[3]);
		fruta6Collider.ratio = modelFruta.getSbb().ratio * 1.2;
		addOrUpdateColliders(collidersSBB, "fruta6", fruta6Collider, modelMatrixFruta6);

		AbstractModel::SBB fruta7Collider;
		glm::mat4 modelMatrixColliderFruta7= glm::mat4(modelMatrixFruta7);
		modelMatrixColliderFruta7 = glm::scale(modelMatrixColliderFruta7, glm::vec3(1.2, 1.2, 1.2));
		modelMatrixColliderFruta7 = glm::translate(modelMatrixColliderFruta7, modelFruta.getSbb().c);
		fruta7Collider.c = glm::vec3(modelMatrixColliderFruta7[3]);
		fruta7Collider.ratio = modelFruta.getSbb().ratio * 1.2;
		addOrUpdateColliders(collidersSBB, "fruta7", fruta7Collider, modelMatrixFruta7);

		AbstractModel::SBB fruta8Collider;
		glm::mat4 modelMatrixColliderFruta8= glm::mat4(modelMatrixFruta8);
		modelMatrixColliderFruta8 = glm::scale(modelMatrixColliderFruta8, glm::vec3(1.2, 1.2, 1.2));
		modelMatrixColliderFruta8 = glm::translate(modelMatrixColliderFruta8, modelFruta.getSbb().c);
		fruta8Collider.c = glm::vec3(modelMatrixColliderFruta8[3]);
		fruta8Collider.ratio = modelFruta.getSbb().ratio * 1.2;
		addOrUpdateColliders(collidersSBB, "fruta8", fruta8Collider, modelMatrixFruta8);

		AbstractModel::SBB fruta9Collider;
		glm::mat4 modelMatrixColliderFruta9= glm::mat4(modelMatrixFruta9);
		modelMatrixColliderFruta9 = glm::scale(modelMatrixColliderFruta9, glm::vec3(1.2, 1.2, 1.2));
		modelMatrixColliderFruta9 = glm::translate(modelMatrixColliderFruta9, modelFruta.getSbb().c);
		fruta9Collider.c = glm::vec3(modelMatrixColliderFruta9[3]);
		fruta9Collider.ratio = modelFruta.getSbb().ratio * 1.2;
		addOrUpdateColliders(collidersSBB, "fruta9", fruta9Collider, modelMatrixFruta9);


		
		//Al centro de la escena
		//ZOMBI CORRIENDO
		AbstractModel::OBB zombiCollider;
		glm::mat4 modelMatrixColliderZombi = glm::mat4(modelMatrixZombi);
		modelMatrixColliderZombi = glm::rotate(modelMatrixColliderZombi,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		zombiCollider.u = glm::quat_cast(modelMatrixZombi);
		modelMatrixColliderZombi = glm::scale(modelMatrixColliderZombi, glm::vec3(1.0));
		modelMatrixColliderZombi = glm::translate(modelMatrixColliderZombi, 
								glm::vec3(zombiModelAnimate.getObb().c.x,
								zombiModelAnimate.getObb().c.y,
								zombiModelAnimate.getObb().c.z));
		zombiCollider.e = zombiModelAnimate.getObb().e * glm::vec3(1.0)*glm::vec3(0.5, 5.0, 0.3);
		zombiCollider.c = glm::vec3(modelMatrixColliderZombi[3]);
		addOrUpdateColliders(collidersOBB, "zombi-", zombiCollider, modelMatrixZombi);

		//zombi 1a
		AbstractModel::OBB zombi_1aCollider;
		glm::mat4 modelMatrixColliderZombi_1a = glm::mat4(modelMatrixZombi_1a);
		modelMatrixColliderZombi_1a = glm::rotate(modelMatrixColliderZombi_1a,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		zombi_1aCollider.u = glm::quat_cast(modelMatrixZombi_1a);
		modelMatrixColliderZombi_1a = glm::scale(modelMatrixColliderZombi_1a, glm::vec3(1.0));
		modelMatrixColliderZombi_1a = glm::translate(modelMatrixColliderZombi_1a, 
								glm::vec3(modelZombi_1a.getObb().c.x,
								modelZombi_1a.getObb().c.y,
								modelZombi_1a.getObb().c.z));
		zombi_1aCollider.e = modelZombi_1a.getObb().e * glm::vec3(1.0)*glm::vec3(0.5, 5.0, 0.3);
		zombi_1aCollider.c = glm::vec3(modelMatrixColliderZombi_1a[3]);
		addOrUpdateColliders(collidersOBB, "zombi_1a-", zombi_1aCollider, modelMatrixZombi_1a);

		//zombi 1b
		AbstractModel::OBB zombi_1bCollider;
		glm::mat4 modelMatrixColliderZombi_1b = glm::mat4(modelMatrixZombi_1b);
		//modelMatrixColliderZombi_1b = glm::rotate(modelMatrixColliderZombi_1b,
		//		glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		zombi_1bCollider.u = glm::quat_cast(modelMatrixZombi_1b);
		modelMatrixColliderZombi_1b = glm::scale(modelMatrixColliderZombi_1b, glm::vec3(1.0));
		modelMatrixColliderZombi_1b = glm::translate(modelMatrixColliderZombi_1b, 
								glm::vec3(modelZombi_1b.getObb().c.x,
								modelZombi_1b.getObb().c.y,
								modelZombi_1b.getObb().c.z));
		zombi_1bCollider.e = modelZombi_1b.getObb().e * glm::vec3(1.0)*glm::vec3(0.5, 5.0, 0.3);
		zombi_1bCollider.c = glm::vec3(modelMatrixColliderZombi_1b[3]);
		addOrUpdateColliders(collidersOBB, "zombi_1b-", zombi_1bCollider, modelMatrixZombi_1b);

		//zombi 1c
		AbstractModel::OBB zombi_1cCollider;
		glm::mat4 modelMatrixColliderZombi_1c = glm::mat4(modelMatrixZombi_1c);
		//modelMatrixColliderZombi_1c = glm::rotate(modelMatrixColliderZombi_1c,
		//		glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		zombi_1cCollider.u = glm::quat_cast(modelMatrixZombi_1c);
		modelMatrixColliderZombi_1c = glm::scale(modelMatrixColliderZombi_1c, glm::vec3(1.0));
		modelMatrixColliderZombi_1c = glm::translate(modelMatrixColliderZombi_1c, 
								glm::vec3(modelZombi_1c.getObb().c.x,
								modelZombi_1c.getObb().c.y,
								modelZombi_1c.getObb().c.z));
		zombi_1cCollider.e = modelZombi_1c.getObb().e * glm::vec3(1.0)*glm::vec3(0.5, 5.0, 0.3);
		zombi_1cCollider.c = glm::vec3(modelMatrixColliderZombi_1c[3]);
		addOrUpdateColliders(collidersOBB, "zombi_1c-", zombi_1cCollider, modelMatrixZombi_1c);


		//cubos colisionadores OBB

		//ZOMBI 1d
		AbstractModel::OBB zombi_1dCollider;
		glm::mat4 modelMatrixColliderZombi_1d = glm::mat4(modelMatrixZombi_1d);
		modelMatrixColliderZombi_1d = glm::rotate(modelMatrixColliderZombi_1d,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		
		// Set the orientation of collider before doing the scale
		zombi_1dCollider.u = glm::quat_cast(modelMatrixColliderZombi_1d);
		modelMatrixColliderZombi_1d = glm::scale(modelMatrixColliderZombi_1d, glm::vec3(1.0));
		modelMatrixColliderZombi_1d = glm::translate(modelMatrixColliderZombi_1d, 
								glm::vec3(modelZombi_1d.getObb().c.x,
								modelZombi_1d.getObb().c.y,
								modelZombi_1d.getObb().c.z));
		zombi_1dCollider.e = modelZombi_1d.getObb().e * glm::vec3(1.0) * glm::vec3(0.5, 5.0, 0.3);
		zombi_1dCollider.c = glm::vec(modelMatrixColliderZombi_1d[3]);
		addOrUpdateColliders(collidersOBB, "zombi_1d-", zombi_1dCollider, modelMatrixZombi_1d);


		//zombi 2a
		AbstractModel::OBB zombi_2aCollider;
		glm::mat4 modelMatrixColliderZombi_2a = glm::mat4(modelMatrixZombi_2a);
		modelMatrixColliderZombi_2a = glm::rotate(modelMatrixColliderZombi_2a,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		zombi_2aCollider.u = glm::quat_cast(modelMatrixZombi_2a);
		modelMatrixColliderZombi_2a = glm::scale(modelMatrixColliderZombi_2a, glm::vec3(1.0));
		modelMatrixColliderZombi_2a = glm::translate(modelMatrixColliderZombi_2a, 
								glm::vec3(modelZombi_2a.getObb().c.x,
								modelZombi_2a.getObb().c.y,
								modelZombi_2a.getObb().c.z));
		zombi_2aCollider.e = modelZombi_2a.getObb().e * glm::vec3(1.0)*glm::vec3(0.5, 5.0, 0.3);
		zombi_2aCollider.c = glm::vec3(modelMatrixColliderZombi_2a[3]);
		addOrUpdateColliders(collidersOBB, "zombi_2a-", zombi_2aCollider, modelMatrixZombi_2a);

		//zombi 2b
		AbstractModel::OBB zombi_2bCollider;
		glm::mat4 modelMatrixColliderZombi_2b = glm::mat4(modelMatrixZombi_2b);
		modelMatrixColliderZombi_2b = glm::rotate(modelMatrixColliderZombi_2b,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		zombi_2bCollider.u = glm::quat_cast(modelMatrixZombi_2b);
		modelMatrixColliderZombi_2b = glm::scale(modelMatrixColliderZombi_2b, glm::vec3(1.0));
		modelMatrixColliderZombi_2b = glm::translate(modelMatrixColliderZombi_2b, 
								glm::vec3(modelZombi_2b.getObb().c.x,
								modelZombi_2b.getObb().c.y,
								modelZombi_2b.getObb().c.z));
		zombi_2bCollider.e = modelZombi_2b.getObb().e * glm::vec3(1.0)*glm::vec3(0.5, 5.0, 0.3);
		zombi_2bCollider.c = glm::vec3(modelMatrixColliderZombi_2b[3]);
		addOrUpdateColliders(collidersOBB, "zombi_2b-", zombi_2bCollider, modelMatrixZombi_2b);

		//zombi 2c
		AbstractModel::OBB zombi_2cCollider;
		glm::mat4 modelMatrixColliderZombi_2c = glm::mat4(modelMatrixZombi_2c);
		modelMatrixColliderZombi_2c = glm::rotate(modelMatrixColliderZombi_2c,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		zombi_2cCollider.u = glm::quat_cast(modelMatrixZombi_2c);
		modelMatrixColliderZombi_2c = glm::scale(modelMatrixColliderZombi_2c, glm::vec3(1.0));
		modelMatrixColliderZombi_2c = glm::translate(modelMatrixColliderZombi_2c, 
								glm::vec3(modelZombi_2c.getObb().c.x,
								modelZombi_2c.getObb().c.y,
								modelZombi_2c.getObb().c.z));
		zombi_2cCollider.e = modelZombi_2c.getObb().e * glm::vec3(1.0)*glm::vec3(0.5, 5.0, 0.3);
		zombi_2cCollider.c = glm::vec3(modelMatrixColliderZombi_2c[3]);
		addOrUpdateColliders(collidersOBB, "zombi_2c-", zombi_2cCollider, modelMatrixZombi_2c);


		//zombi 3a
		AbstractModel::OBB zombi_3aCollider;
		glm::mat4 modelMatrixColliderZombi_3a = glm::mat4(modelMatrixZombi_3a);
		modelMatrixColliderZombi_3a = glm::rotate(modelMatrixColliderZombi_3a,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		zombi_3aCollider.u = glm::quat_cast(modelMatrixZombi_3a);
		modelMatrixColliderZombi_3a = glm::scale(modelMatrixColliderZombi_3a, glm::vec3(1.0));
		modelMatrixColliderZombi_3a = glm::translate(modelMatrixColliderZombi_3a, 
								glm::vec3(modelZombi_3a.getObb().c.x,
								modelZombi_3a.getObb().c.y,
								modelZombi_3a.getObb().c.z));
		zombi_3aCollider.e = modelZombi_3a.getObb().e * glm::vec3(1.0)*glm::vec3(0.5, 5.0, 0.3);
		zombi_3aCollider.c = glm::vec3(modelMatrixColliderZombi_3a[3]);
		addOrUpdateColliders(collidersOBB, "zombi_3a-", zombi_3aCollider, modelMatrixZombi_3a);

		//zombi 3b
		AbstractModel::OBB zombi_3bCollider;
		glm::mat4 modelMatrixColliderZombi_3b = glm::mat4(modelMatrixZombi_3b);
		modelMatrixColliderZombi_3b = glm::rotate(modelMatrixColliderZombi_3b,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		zombi_3bCollider.u = glm::quat_cast(modelMatrixZombi_3b);
		modelMatrixColliderZombi_3b = glm::scale(modelMatrixColliderZombi_3b, glm::vec3(1.0));
		modelMatrixColliderZombi_3b = glm::translate(modelMatrixColliderZombi_3b, 
								glm::vec3(modelZombi_3b.getObb().c.x,
								modelZombi_3b.getObb().c.y,
								modelZombi_3b.getObb().c.z));
		zombi_3bCollider.e = modelZombi_3b.getObb().e * glm::vec3(1.0)*glm::vec3(0.5, 5.0, 0.3);
		zombi_3bCollider.c = glm::vec3(modelMatrixColliderZombi_3b[3]);
		addOrUpdateColliders(collidersOBB, "zombi_3b-", zombi_3bCollider, modelMatrixZombi_3b);

		//zombi 3c
		AbstractModel::OBB zombi_3cCollider;
		glm::mat4 modelMatrixColliderZombi_3c = glm::mat4(modelMatrixZombi_3c);
		modelMatrixColliderZombi_3c = glm::rotate(modelMatrixColliderZombi_3c,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		zombi_3cCollider.u = glm::quat_cast(modelMatrixZombi_3c);
		modelMatrixColliderZombi_3c = glm::scale(modelMatrixColliderZombi_3c, glm::vec3(1.0));
		modelMatrixColliderZombi_3c = glm::translate(modelMatrixColliderZombi_3c, 
								glm::vec3(modelZombi_3c.getObb().c.x,
								modelZombi_3c.getObb().c.y,
								modelZombi_3c.getObb().c.z));
		zombi_3cCollider.e = modelZombi_3c.getObb().e * glm::vec3(1.0)*glm::vec3(0.5, 5.0, 0.3);
		zombi_3cCollider.c = glm::vec3(modelMatrixColliderZombi_3c[3]);
		addOrUpdateColliders(collidersOBB, "zombi_3c-", zombi_3cCollider, modelMatrixZombi_3c);


		// Lamps1 colliders
		for (int i = 0; i < lamp1Position.size(); i++){
			AbstractModel::OBB lampCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp1Position[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp1Orientation[i]),
					glm::vec3(0, 1, 0));
			
			// Set the orientation of collider before doing the scale
			lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(0.5));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLamp1.getObb().c);
			lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
			lampCollider.e = modelLamp1.getObb().e * glm::vec3(0.5);
			addOrUpdateColliders(collidersOBB, "lamp1-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);

			
		}

		//Lamp2 Colliders

		for (int i = 0; i < lamp2Position.size(); i++)
		{
			AbstractModel::OBB modelCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0f);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp2Position[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp,
									glm::radians(lamp2Orientation[i]), 
									glm::vec3(0, 1, 0));
			
			
			modelCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(1.0f));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLamp2Post.getObb().c);
			modelCollider.c = modelMatrixColliderLamp[3];
			modelCollider.e = modelLamp2Post.getObb().e * glm::vec3(1.0f);
			addOrUpdateColliders(collidersOBB, "lamp2-" + std::to_string(i),modelCollider,modelMatrixColliderLamp);
				
		}

		// Lamps3 colliders
		for (int i = 0; i < lamp3Position.size(); i++){
			AbstractModel::OBB lampCollider3;
			glm::mat4 modelMatrixColliderLamp3 = glm::mat4(1.0);
			modelMatrixColliderLamp3 = glm::translate(modelMatrixColliderLamp3, lamp3Position[i]);
			modelMatrixColliderLamp3 = glm::rotate(modelMatrixColliderLamp3, glm::radians(lamp3Orientation[i]),
					glm::vec3(0, 1, 0));
			
			// Set the orientation of collider before doing the scale
			lampCollider3.u = glm::quat_cast(modelMatrixColliderLamp3);
			modelMatrixColliderLamp3 = glm::scale(modelMatrixColliderLamp3, glm::vec3(0.05));
			modelMatrixColliderLamp3 = glm::translate(modelMatrixColliderLamp3, modelLamp3.getObb().c);
			lampCollider3.c = modelMatrixColliderLamp3[3];
			lampCollider3.e = modelLamp3.getObb().e * glm::vec3(0.05);
			addOrUpdateColliders(collidersOBB, "lamp3-" + std::to_string(i), lampCollider3, modelMatrixColliderLamp3);
			
		}

		// Lamps4 colliders
		for (int i = 0; i < lamp4Position.size(); i++){
			AbstractModel::OBB lampCollider4;
			glm::mat4 modelMatrixColliderLamp4 = glm::mat4(1.0);
			modelMatrixColliderLamp4 = glm::translate(modelMatrixColliderLamp4, lamp4Position[i]);
			modelMatrixColliderLamp4 = glm::rotate(modelMatrixColliderLamp4, glm::radians(lamp4Orientation[i]),
					glm::vec3(0, 1, 0));
			
			// Set the orientation of collider before doing the scale
			lampCollider4.u = glm::quat_cast(modelMatrixColliderLamp4);
			modelMatrixColliderLamp4 = glm::scale(modelMatrixColliderLamp4, glm::vec3(0.05));
			modelMatrixColliderLamp4 = glm::translate(modelMatrixColliderLamp4, modelLamp4.getObb().c);
			lampCollider4.c = modelMatrixColliderLamp4[3];
			lampCollider4.e = modelLamp4.getObb().e * glm::vec3(0.05);
			addOrUpdateColliders(collidersOBB, "lamp4-" + std::to_string(i), lampCollider4, modelMatrixColliderLamp4);
			
		}

		//Colisionador del Personaje  principal: LUFFY 
		// Collider de luffy
		AbstractModel::OBB luffyCollider;
		glm::mat4 modelmatrixColliderLuffy = glm::mat4(modelMatrixLuffy);
		modelmatrixColliderLuffy = glm::rotate(modelmatrixColliderLuffy,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		luffyCollider.u = glm::quat_cast(modelmatrixColliderLuffy);
		modelmatrixColliderLuffy = glm::scale(modelmatrixColliderLuffy, glm::vec3(1.0));
		modelmatrixColliderLuffy = glm::translate(modelmatrixColliderLuffy,
				glm::vec3(luffyModelAnimate.getObb().c.x,
						luffyModelAnimate.getObb().c.y,
						luffyModelAnimate.getObb().c.z));
		luffyCollider.e = luffyModelAnimate.getObb().e * glm::vec3(1.0) * glm::vec3(0.3, 0.6+golpe1*0.4+recogerFruta1*0.4, 0.9);
		luffyCollider.c = glm::vec3(modelmatrixColliderLuffy[3]);
		addOrUpdateColliders(collidersOBB, "luffy", luffyCollider, modelMatrixLuffy);

		//Colisionador del personaje: ENEMY
		
		AbstractModel::OBB enemyCollider;
		glm::mat4 modelmatrixColliderEnemy = glm::mat4(modelMatrixEnemy);
		modelmatrixColliderEnemy= glm::rotate(modelmatrixColliderEnemy,    //Se necesita esto para alinear la caja de Colisionador
			glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		enemyCollider.u = glm::quat_cast(modelmatrixColliderEnemy);
		modelmatrixColliderEnemy = glm::scale(modelmatrixColliderEnemy, glm::vec3(0.8,1.0,0.8));
		modelmatrixColliderEnemy = glm::translate(modelmatrixColliderEnemy,
				glm::vec3(enemyModelAnimate.getObb().c.x,
						enemyModelAnimate.getObb().c.y,
						enemyModelAnimate.getObb().c.z));
		enemyCollider.e = enemyModelAnimate.getObb().e * glm::vec3(0.8,1.0,0.8);
		enemyCollider.c = glm::vec3(modelmatrixColliderEnemy[3]);
		addOrUpdateColliders(collidersOBB, "enemy", enemyCollider, modelMatrixEnemy);


		/*******************************************
		 * Renderizado de los colisionadores. SE COMENTAN, SI NO SE QUIERE VISUALIZARLOS
		 * Render de colliders
		 *******************************************///<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		/*for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}*/

		/**********Render de transparencias***************/
		
		renderAlphaScene();
		

		/*********************Prueba de colisiones****************************/
		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt && testSphereSphereIntersection(
					std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay collision entre " << it->first <<
						" y el modelo " << jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isColision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt && 
					testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision entre " << it->first << " y el modelo" <<
						jt->first << std::endl;
						if(it->first == "zombi-" && isPunch == true){				 //***********PARA DESAPARECER LAS FRUTAS
							visibleZombi = false;
							puntuacion+=500;
							//sourcesPlay[7]=true;
						}
						else if(it->first == "zombi-" && isPunch == false)			 //***********PARA DESAPARECER LAS FRUTAS
							if (visibleZombi != false)
								sumaColisionOBB_OBB += 1;
								//sourcesPlay[6]=true;
						if(it->first == "zombi_1a-" && isPunch == true){
							visibleZombi_1a = false;
							puntuacion+=500;
							//sourcesPlay[7]=true;
						}
						else if(it->first == "zombi_1a-" && isPunch == false)
							if (visibleZombi != false)
								sumaColisionOBB_OBB += 1;
								//sourcesPlay[6]=true;
						if(it->first == "zombi_1b-" && isPunch == true){
							visibleZombi_1b = false ;
							puntuacion+=500;
							//sourcesPlay[7]=true;
						}
						else if(it->first == "zombi_1b-" && isPunch == false)
							if (visibleZombi != false)
								sumaColisionOBB_OBB += 1;
								//sourcesPlay[6]=true;
						if(it->first == "zombi_1c-" && isPunch == true){
							visibleZombi_1c = false;
							puntuacion+=500;
							//sourcesPlay[7]=true;
						}
						else if(it->first == "zombi_1c-" && isPunch == false)
							if (visibleZombi != false)
								sumaColisionOBB_OBB += 1;
								//sourcesPlay[6]=true;
						if(it->first == "zombi_1d-" && isPunch == true){
							visibleZombi_1d = false;
							puntuacion+=500;
							//sourcesPlay[7]=true;
						}
						else if(it->first == "zombi_1d-" && isPunch == false)
							if (visibleZombi != false)
								sumaColisionOBB_OBB += 1;
								//sourcesPlay[6]=true;
						if(it->first == "zombi_2a-" && isPunch == true){
							visibleZombi_2a = false;
							puntuacion+=500;
							//sourcesPlay[7]=true;
						}
						else if(it->first == "zombi_2a-" && isPunch == false)
							if (visibleZombi != false)
								sumaColisionOBB_OBB += 1;
								//sourcesPlay[6]=true;
						if(it->first == "zombi_2b-" && isPunch == true){
							visibleZombi_2b = false;
							puntuacion+=500;
							//sourcesPlay[7]=true;
						}
						else if(it->first == "zombi_2b-" && isPunch == false)
							if (visibleZombi != false)
								sumaColisionOBB_OBB += 1;
								//sourcesPlay[6]=true;
						if(it->first == "zombi_2c-" && isPunch == true){
							visibleZombi_2c = false;
							puntuacion+=500;
							//sourcesPlay[7]=true;
						}
						else if(it->first == "zombi_2c-" && isPunch == false)
							if (visibleZombi != false)
								sumaColisionOBB_OBB += 1;
								//sourcesPlay[6]=true;
						if(it->first == "zombi_3a-" && isPunch == true){
							visibleZombi_3a = false;
							puntuacion+=500;
							//sourcesPlay[7]=true;
						}
						else if(it->first == "zombi_3a-" && isPunch == false)
							if (visibleZombi != false)
								sumaColisionOBB_OBB += 1;
								//sourcesPlay[6]=true;
						if(it->first == "zombi_3b-" && isPunch == true){
							visibleZombi_3b = false;
							puntuacion+=500;
							//sourcesPlay[7]=true;
						}
						else if(it->first == "zombi_3b-" && isPunch == false)
							if (visibleZombi != false)
								sumaColisionOBB_OBB += 1;
								//sourcesPlay[6]=true;
						if(it->first == "zombi_3c-" && isPunch == true){
							visibleZombi_3c = false;
							puntuacion+=500;
							//sourcesPlay[7]=true;
							
						}
						else if(it->first == "zombi_3c-" && isPunch == false)
							if (visibleZombi != false)
								sumaColisionOBB_OBB += 1;
								//sourcesPlay[6]=true;
					
					//sumaColisionOBB_OBB += 1;
					
					
					isColision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isColision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
								
				if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision del " << it->first << " y el modelo" <<
						jt->first << std::endl;           
						if(it->first == "fruta1" && opcionPresionarBoton1 == true){ //***********PARA DESAPARECER LAS FRUTAS
							visibleFruta1 = false;
							puntuacion+=250;
						}
						if(it->first == "fruta2" && opcionPresionarBoton1 == true){
							visibleFruta2 = false;
							puntuacion+=250;
						}
						if(it->first == "fruta3" && opcionPresionarBoton1 == true){
							visibleFruta3 = false;
							puntuacion+=250;
						}
						if(it->first == "fruta4" && opcionPresionarBoton1 == true){
							visibleFruta4 = false;
							puntuacion+=250;
						}
						if(it->first == "fruta5" && opcionPresionarBoton1 == true){
							visibleFruta5 = false;
							puntuacion+=250;
						}
						if(it->first == "fruta6" && opcionPresionarBoton1 == true){
							visibleFruta6 = false;
							puntuacion+=250;
						}
						if(it->first == "fruta7" && opcionPresionarBoton1 == true){
							visibleFruta7 = false;
							puntuacion+=250;
						}
						if(it->first == "fruta8" && opcionPresionarBoton1 == true){
							visibleFruta8 = false;
							puntuacion+=250;
						}
						if(it->first == "fruta9" && opcionPresionarBoton1 == true){
							visibleFruta9 = false;
							puntuacion+=250;
						}
						


					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, true);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator itCollision;
		for (itCollision = collisionDetection.begin(); 
			itCollision != collisionDetection.end(); itCollision++) {
			std::map<std::string, std::tuple<AbstractModel::SBB, 
				glm::mat4, glm::mat4>>::iterator sbbBuscado = 
				collidersSBB.find(itCollision->first);
			std::map<std::string, std::tuple<AbstractModel::OBB,
				glm::mat4, glm::mat4>>::iterator obbBuscado =
				collidersOBB.find(itCollision->first);
			if (sbbBuscado != collidersSBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersSBB, itCollision->first);
			}
			if (obbBuscado != collidersOBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersOBB, itCollision->first);
				else {
					if (itCollision->first.compare("luffy") == 0)
						modelMatrixLuffy = std::get<1>(obbBuscado->second);
					if (itCollision->first.compare("dart") == 0)
						modelMatrixDart = std::get<1>(obbBuscado->second);
				}
			}
		}

		// El rayo es un elemento opcional. 

		//ELEMENTO RAYO en la parte de abajo
		//**************************************************
		/*glm::mat4 modelMatrixRayMay = glm::mat4(modelMatrixLuffy);
		modelMatrixRayMay = glm::translate(modelMatrixRayMay, glm::vec3(0, 1, 0));
		float maxDistanceRay = 10.0;
		glm::vec3 rayDirection = modelMatrixRayMay[2];
		glm::vec3 ori = modelMatrixRayMay[3];
		glm::vec3 rmd = ori + rayDirection * (maxDistanceRay / 2.0f);
		glm::vec3 targetRay = ori + rayDirection * maxDistanceRay;
		modelMatrixRayMay[3] = glm::vec4(rmd, 1.0);
		modelMatrixRayMay = glm::rotate(modelMatrixRayMay, glm::radians(90.0f), 
			glm::vec3(1, 0, 0));
		modelMatrixRayMay = glm::scale(modelMatrixRayMay, 
			glm::vec3(0.05, maxDistanceRay, 0.05));
		rayModel.render(modelMatrixRayMay);

		std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::
			iterator itSBB;
		for (itSBB = collidersSBB.begin(); itSBB != collidersSBB.end(); itSBB++) {
			float tRint;
			if (raySphereIntersect(ori, targetRay, rayDirection,
				std::get<0>(itSBB->second), tRint)) {
				std::cout << "Collision del rayo con el modelo " << itSBB->first 
				<< std::endl;
			}
		}*/
		/*std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::
			iterator itOBB;
		for (itOBB = collidersOBB.begin(); itOBB != collidersOBB.end(); itOBB++) {
			if (testRayOBB(ori, targetRay, std::get<0>(itOBB->second))) {
				std::cout << "Collision del rayo con el modelo " << itOBB->first
					<< std::endl;
			}
		}*/
		//*******************************************************************

		// Esto es para ilustrar la transformacion inversa de los coliders
		/*glm::vec3 cinv = glm::inverse(luffyCollider.u) * glm::vec4(rockCollider.c, 1.0);
		glm::mat4 invColliderS = glm::mat4(1.0);
		invColliderS = glm::translate(invColliderS, cinv);
		invColliderS =  invColliderS * glm::mat4(luffyCollider.u);
		invColliderS = glm::scale(invColliderS, glm::vec3(rockCollider.ratio * 2.0, rockCollider.ratio * 2.0, rockCollider.ratio * 2.0));
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphereCollider.enableWireMode();
		sphereCollider.render(invColliderS);
		glm::vec3 cinv2 = glm::inverse(luffyCollider.u) * glm::vec4(luffyCollider.c, 1.0);
		glm::mat4 invColliderB = glm::mat4(1.0);
		invColliderB = glm::translate(invColliderB, cinv2);
		invColliderB = glm::scale(invColliderB, luffyCollider.e * 2.0f);
		boxCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		boxCollider.enableWireMode();
		boxCollider.render(invColliderB);
		// Se regresa el color blanco
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));*/
		
		// ***************ANIMACION POR KEYFRAMES
		// Animaciones por keyframes dart Vader
		// Para salvar los keyframes
		if(record && modelSelected == 1){
			matrixDartJoints.push_back(rotDartHead);
			matrixDartJoints.push_back(rotDartLeftArm);
			matrixDartJoints.push_back(rotDartLeftHand);
			matrixDartJoints.push_back(rotDartRightArm);
			matrixDartJoints.push_back(rotDartRightHand);
			matrixDartJoints.push_back(rotDartLeftLeg);
			matrixDartJoints.push_back(rotDartRightLeg);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixDartJoints);
			}
		}
		else if(keyFramesDartJoints.size() > 0){
			// Para reproducir el frame
			interpolationDartJoints = numPasosDartJoints / (float) maxNumPasosDartJoints;
			numPasosDartJoints++;
			if(interpolationDartJoints > 1.0){
				interpolationDartJoints = 0;
				numPasosDartJoints = 0;
				indexFrameDartJoints = indexFrameDartJointsNext;
				indexFrameDartJointsNext++;
			}
			if(indexFrameDartJointsNext > keyFramesDartJoints.size() -1)
				indexFrameDartJointsNext = 0;
			rotDartHead = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 0, interpolationDartJoints);
			rotDartLeftArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 1, interpolationDartJoints);
			rotDartLeftHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 2, interpolationDartJoints);
			rotDartRightArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 3, interpolationDartJoints);
			rotDartRightHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 4, interpolationDartJoints);
			rotDartLeftLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 5, interpolationDartJoints);
			rotDartRightLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 6, interpolationDartJoints);
		}
		if(record && modelSelected == 2){
			matrixDart.push_back(modelMatrixDart);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixDart);
			}
		}
		else if(keyFramesDart.size() > 0){
			interpolationDart = numPasosDart / (float) maxNumPasosDart;
			numPasosDart++;
			if(interpolationDart > 1.0){
				numPasosDart = 0;
				interpolationDart = 0;
				indexFrameDart = indexFrameDartNext;
				indexFrameDartNext++;
			}
			if(indexFrameDartNext > keyFramesDart.size() - 1)
				indexFrameDartNext = 0;
			modelMatrixDart = interpolate(keyFramesDart, indexFrameDart, indexFrameDartNext, 0, interpolationDart);
		}
		// Animaciones por keyframes buzz
		// Para salvar los keyframes
		if(record && modelSelected == 3){
			matrixBuzzJoints.push_back(rotBuzzHead);
			matrixBuzzJoints.push_back(rotBuzzLeftarm);
			matrixBuzzJoints.push_back(rotBuzzLeftForeArm);
			matrixBuzzJoints.push_back(rotBuzzLeftHand);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixBuzzJoints);
			}
		}
		else if(keyFramesBuzzJoints.size() > 0){
			// Para reproducir el frame
			interpolationBuzzJoints = numPasosBuzzJoints / (float) maxNumPasosBuzzJoints;
			numPasosBuzzJoints++;
			if(interpolationBuzzJoints > 1.0){
				interpolationBuzzJoints = 0;
				numPasosBuzzJoints = 0;
				indexFrameBuzzJoints = indexFrameBuzzJointsNext;
				indexFrameBuzzJointsNext++;
			}
			if(indexFrameBuzzJointsNext > keyFramesBuzzJoints.size() -1)
				indexFrameBuzzJointsNext = 0;
			rotBuzzHead = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 0, interpolationBuzzJoints);
			rotBuzzLeftarm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 1, interpolationBuzzJoints);
			rotBuzzLeftForeArm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 2, interpolationBuzzJoints);
			rotBuzzLeftHand = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 3, interpolationBuzzJoints);
		}
		if(record && modelSelected == 4){
			matrixBuzz.push_back(modelMatrixBuzz);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixBuzz);
			}
		}
		else if(keyFramesBuzz.size() > 0){
			interpolationBuzz = numPasosBuzz / (float) maxNumPasosBuzz;
			numPasosBuzz++;
			if(interpolationBuzz > 1.0){
				numPasosBuzz = 0;
				interpolationBuzz = 0;
				indexFrameBuzz = indexFrameBuzzNext;
				indexFrameBuzzNext++;
			}
			if(indexFrameBuzzNext > keyFramesBuzz.size() - 1)
				indexFrameBuzzNext = 0;
			modelMatrixBuzz = interpolate(keyFramesBuzz, indexFrameBuzz, indexFrameBuzzNext, 0, interpolationBuzz);
		}
		
		/**********MAQUINAS DE ESTADO*************/
		// Maquina de estados para el CARRO ECLIPSE
		switch (state){
		case 0:
			if(numberAdvance == 0)
				maxAdvance = 65.0;
			else if(numberAdvance == 1)
				maxAdvance = 49.0;
			else if(numberAdvance == 2)
				maxAdvance = 44.5;
			else if(numberAdvance == 3)
				maxAdvance = 49.0;
			else if(numberAdvance == 4)
				maxAdvance = 44.5;
			state = 1;
			break;
		case 1:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, avance));
			advanceCount += avance;
			rotWheelsX += 0.05;
			rotWheelsY -= 0.02;
			if(rotWheelsY < 0)
				rotWheelsY = 0;
			if(advanceCount > maxAdvance){
				advanceCount = 0;
				numberAdvance++;
				state = 2;
			}
			break;
		case 2:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0, 0.0, 0.025f));
			modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(giroEclipse), glm::vec3(0, 1, 0));
			rotCount += giroEclipse;
			rotWheelsX += 0.05;
			rotWheelsY += 0.02;
			if(rotWheelsY > 0.25)
				rotWheelsY = 0.25;
			if(rotCount >= 90.0f){
				rotCount = 0;
				state = 0;
				if(numberAdvance > 4)
					numberAdvance = 1;
			}
			break;
		
		default:
			break;
		}

		// MAQUINA DE ESTADO DE LAS PUERTAS DEL LAMBO
		switch (stateDoor)
		{
		case 0:
			dorRotCount += 0.5;
			if(dorRotCount > 75)
				stateDoor = 1;
			break;
		case 1:
			dorRotCount -= 0.5;
			if(dorRotCount < 0){
				dorRotCount = 0.0;
				stateDoor = 0;
			}
		
		default:
			break;
		}

		// Constantes de animaciones
		rotHelHelY += 0.5;
		rotHelHelBack += 0.5;
		animationLuffyIndex = 1;
		x1 -=0.05f;
		z1 -=0.05f;
		energiaVida -=0.01;
		
		/*if(abs(10.0-x1)<=1.0f)
		{
		  x1 = 0.0f;
		  limite_acercar = true;

		}*/

		glfwSwapBuffers(window);

		/****************************+
		 * Para el SONIDO
		 * Open AL sound data
		 */
		source0Pos[0] = modelMatrixFountain[3].x;
		source0Pos[1] = modelMatrixFountain[3].y;
		source0Pos[2] = modelMatrixFountain[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);


		// Listener for the Thris person camera
		listenerPos[0] = modelMatrixLuffy[3].x;
		listenerPos[1] = modelMatrixLuffy[3].y;
		listenerPos[2] = modelMatrixLuffy[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixLuffy[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixLuffy[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		// Listener for the First person camera
		// listenerPos[0] = camera->getPosition().x;
		// listenerPos[1] = camera->getPosition().y;
		// listenerPos[2] = camera->getPosition().z;
		// alListenerfv(AL_POSITION, listenerPos);
		// listenerOri[0] = camera->getFront().x;
		// listenerOri[1] = camera->getFront().y;
		// listenerOri[2] = camera->getFront().z;
		// listenerOri[3] = camera->getUp().x;
		// listenerOri[4] = camera->getUp().y;
		// listenerOri[5] = camera->getUp().z;
		alListenerfv(AL_ORIENTATION, listenerOri);

		for(unsigned int i = 0; i < sourcesPlay.size(); i++){
			if(sourcesPlay[i]){
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
			}
		}
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}