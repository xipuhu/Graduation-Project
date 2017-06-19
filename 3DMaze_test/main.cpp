
#pragma warning(disable : 4305)
#pragma warning(disable : 4244)



#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include <time.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <SOIL\SOIL.h>    //用于载入图片的库文件
#include <glm\glm.hpp>     //GLSL的数学库
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>



#include "Shader.h"      //着色器头文件
#include "Texture.h"     //纹理头文件
#include "Camera.h"      //摄像机头文件
#include "GameManage.h"
#include "MazeRotate.h"
#include "Font.h"
#include "MazeDepthFirst.h"
#include "MazePrim.h"
#include "Astar.h"
#include "SkyBox.h"

//字体大小
#define SMALLFONT 0.4
#define BIGFONT 0.7


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void mouseButton_callback(GLFWwindow* window, int key, int action, int mode);

GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
void CameraMove();        //摄像机移动函数


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

//迷宫大小
int mazeSize = 10;
int gameLevel = 1;         //关卡
string gameLevel_str;
bool resetMaze = true;

//摄像机
Camera myCamera(glm::vec3(mazeSize - 1.0f, 0.0f, mazeSize - 1.0f));
bool firstMouse = true;   //用于判断是否第一次获取鼠标位置
GLfloat lastX = 400, lastY = 300; //鼠标初始坐标值
GLfloat yaw = 0.0f, pitch = 0.0f;


GLfloat deltaTime = 0.0f;  //当前帧遇上一帧的时间差
GLfloat lastFram = 0.0f;   //上一帧的时间
bool keys[4];             //用于储存键盘方向键按键状态    


//迷宫类声明
Maze_DepthFirst* pMaze1 = new Maze_DepthFirst();
Maze_Prim* pMaze2 = new Maze_Prim();


//todo cube碰撞
bool cubeIsCrashed[100];
glm::vec3 cubePosition[100];
GLfloat cubeSpeed = 1.50f;
int cubeMoveDir[100];
int lastCubeMoveDir[100];
GLfloat moveValue[100];         //距离差值
int cubeNumber = 10;

//todo A*寻路算法1
Astar astar;
bool _cubeIsCrashed;
glm::vec3 _cubePosition = glm::vec3(-mazeSize + 1.0f, 0.0f, -mazeSize + 1.0f);
int _cubeMoveDir;
int _lastCubeMoveDir;
GLfloat _moveValue;
glm::vec3 lastCubePosition = _cubePosition;


//游戏管理类声明
GameManage myGameManage;
bool isCrashed = false; //用于判断是否发生碰撞


//迷宫旋转类
MazeRotate myMazeRotate;
bool mouseLeftDown = false;
bool firstMouseDown = true;

//文字类声明
Font MyFont;


//自定义鼠标光标显示
GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
GLFWimage image;
int w, h;
unsigned char* pixels = SOIL_load_image("path/to/textures/hand.png", &w, &h, 0, SOIL_LOAD_RGBA);


//显示UI字体属性
GLfloat fontSize[19];
glm::vec3 fontColor[19];

//天空盒
SkyBox mySkybox_Normal, mySkybox_Easy;

//todo 游戏信息显示
int minute =3, second = 0;       //游戏计时
double timer = 0.0f;
std::string minute_str, second_str;
bool firstReduce = true;   

//todo 控制音效播放的相关变量
int lastSecond;
bool countDown = true;
double lastTimer;
bool playWinSound = false;
bool playBgMusic = true;
bool playOverSound = false;
//控制旋转迷宫的音效
GLfloat lastAngleX, lastAngleY;
bool startRotate = true;
bool playRotateSound = true;
//控制鼠标划过声音播放
bool startPlay = false;
bool moveEnter = true;
//玩家受到攻击的音效开关
bool playHurtSound = true;

int lifes=5;                        //玩家生命次数
string lifes_str;
bool isHurted = false;
int index;                         //用来标识撞上玩家的cube

//手电筒开关
bool openFlashlight = true;

//返回游戏开关
bool returnGame = false;
//判断是否切换到第三人称视角状态
bool thridPersonPerspective = false;

//用于显示玩家死亡时的模糊界面
bool dieState = false;

bool winState = false;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Maze_3D", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetMouseButtonCallback(window, mouseButton_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);


	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	//打开深度测试功能
	glEnable(GL_DEPTH_TEST);

	//初始化字体属性
	for (int i = 0; i < 6; ++i){
		fontSize[i] = SMALLFONT;
		if (i<3)
		 fontColor[i] = glm::vec3(0.0f, 0.7f, 0.0f);
		else 
			fontColor[i] = glm::vec3(0.7f, 0.7f, 0.7f);
	}
	for (int i = 6; i < 13; ++i){
		fontSize[i] = SMALLFONT;
		if (i % 2 == 0 && i<12)
			fontColor[i] = glm::vec3(0.7f, 0.0f, 0.0f);
		else
			fontColor[i] = glm::vec3(0.0f, 0.7f, 0.0f);
	}
	for (int i = 13; i < 19; ++i){
		fontSize[i] = SMALLFONT;
		fontColor[i] = glm::vec3(0.7f, 0.7f, 0.7f);
	}

	//todo 渲染文字初始化
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	MyFont.loadFontShader();
	MyFont.InitText(WIDTH, HEIGHT);


	//todo 构建着色器
	Shader lampShader, levelEasyShader, levelNormalShader, levelDifficultShader, screenShader;

	lampShader.loadShaderCode("path/to/shaders/lamp.vs", "path/to/shaders/lamp.frag");
	lampShader.compileShader();
	lampShader.linkToProgram();

	levelEasyShader.loadShaderCode("path/to/shaders/easy.vs", "path/to/shaders/easy.frag");
	levelEasyShader.compileShader();
	levelEasyShader.linkToProgram();

	levelNormalShader.loadShaderCode("path/to/shaders/normal.vs", "path/to/shaders/normal.frag");
	levelNormalShader.compileShader();
	levelNormalShader.linkToProgram();

	levelDifficultShader.loadShaderCode("path/to/shaders/difficult.vs", "path/to/shaders/difficult.frag");
	levelDifficultShader.compileShader();
	levelDifficultShader.linkToProgram();

	//用于渲染帧缓存内容的着色器
	screenShader.loadShaderCode("path/to/shaders/screen.vs", "path/to/shaders/screen.frag");
	screenShader.compileShader();
	screenShader.linkToProgram();

	//todo 天空盒1
	mySkybox_Normal.LoadSkyboxShader("path/to/shaders/skybox.vs","path/to/shaders/skybox.frag");
	mySkybox_Easy.LoadSkyboxShader("path/to/shaders/skybox.vs","path/to/shaders/skybox.frag");


	//todo 读取游戏纪录
	const GLchar* scorePath[3] = {
		"path/to/highScore/easy.txt",
		"path/to/highScore/normal.txt",
		"path/to/highScore/difficult.txt"
	};
	for (int i = 0; i < 3; ++i){
		myGameManage.reedHighScore(scorePath[i], i);
	}


	//顶点数据
	GLfloat vertices[] = {
		// Positions           // Normals      // Texture Coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	    -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

	    -0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, 0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	    -0.5f, 0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	    -0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f
	};

	//todo frameBuffer_01
	GLfloat quadVertices[] = {   
		// Positions   // TexCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	//todo 天空盒2
	//mySkybox_Normal_Difficult
	mySkybox_Normal.SetupSkyboxVAO();
	GLchar* cubemapPaths[6] = {
		"path/to/textures/Nightsky/south.bmp",
		"path/to/textures/Nightsky/north.bmp",
		"path/to/textures/Nightsky/up.bmp",
		"path/to/textures/Nightsky/down.bmp",
		"path/to/textures/Nightsky/east.bmp",
		"path/to/textures/Nightsky/west.bmp"
	};
	mySkybox_Normal.LoadSkyboxTextures(cubemapPaths);

	//mySkybox_Easy
	mySkybox_Easy.SetupSkyboxVAO();
	GLchar* cubemapPaths2[6] = { 
		"path/to/textures/RedSky/south.bmp",
		"path/to/textures/RedSky/north.bmp",
		"path/to/textures/RedSky/up.bmp",
		"path/to/textures/RedSky/down.bmp",
		"path/to/textures/RedSky/east.bmp",
		"path/to/textures/RedSky/west.bmp" };
	mySkybox_Easy.LoadSkyboxTextures(cubemapPaths2);


	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	//配置位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//配置法向量属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//配置纹理坐标属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	//解绑VAO对象
	glBindVertexArray(0);

	GLuint floorVBO, floorVAO;
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(floorVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//用于绘制玩家与目标位置
	GLuint CubePosVAO;
	glGenVertexArrays(1, &CubePosVAO);
	glBindVertexArray(CubePosVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//todo frameBuffer_02
	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

	//todo 载入纹理数据
	Texture MyTextures[7];
	GLchar* paths[6] = { 
		"path/to/textures/container2.png",
		"path/to/textures/container2_specular.png",
		"path/to/textures/top.png",
		"path/to/textures/wall.png",
		"path/to/textures/floor.jpg",
		"path/to/textures/awesomeface.png"
						  };

	for (int i = 0; i < 6; ++i){
		MyTextures[i].loadTexture_frame(paths[i]);
	}

	//todo frameBuffer_03
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	GLuint textureColorbuffer = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT); 
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//摄像机移动
		//计算游戏帧率
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFram;
		lastFram = currentFrame;

	
		CameraMove();

		//todo 设置鼠标显示
		if (myGameManage.game_state == PLAYING&&myGameManage.gameUI_state==UIDEFAULT&&myCamera.moveLock==false){
			glfwDestroyCursor(cursor);                        //删除鼠标对象（避免上次鼠标对象带来的影响）

			cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
			glfwSetCursor(window, cursor);

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			if (myGameManage.gameUI_state == HIGHSCORE_UI){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				myCamera.moveLock = true;
			}
			else
			{
				switch (myGameManage.cursorStyle)
				{
				case NORMAL:
					cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
					glfwSetCursor(window, cursor);
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);    //将鼠标显示出来
					break;
				case HAND:
					cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
					glfwSetCursor(window, cursor);
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);    //将鼠标显示出来
					break;
				case CUSTOM:

					//自定义鼠标
					image.pixels = pixels;
					image.width = w;
					image.height = h;

					cursor = glfwCreateCursor(&image, 0, 0);
					glfwSetCursor(window, cursor);
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);    //将鼠标显示出来
					break;
				default:
					break;
				}
			}
			

		}

		//声音播放控制
		if (startPlay && myGameManage.playSound == true){
			myGameManage.cursorMovetoSound.play();
			startPlay = false;
			moveEnter = false;

		}
		//播放背景音乐
		if (playBgMusic==true && myGameManage.game_state == PLAYING && myGameManage.gameUI_state==UIDEFAULT && myGameManage.playMusic == true)
			myGameManage.bgMusic.startPlay();
		else
			myGameManage.bgMusic.pausePlay();


	

		// Render
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//清除颜色缓冲区和深度缓冲区
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);



#pragma region 渲染第一页UI信息
		if (myGameManage.game_state == FIRST_UI){

			//重置cube数量
			cubeNumber = 10;
			//重置时间
			minute = 3;

			//重置倒计时音效播放
			countDown = true;

			glEnable(GL_CULL_FACE);
			MyFont.RenderText("Welcome to the maze,just enjoy yourselve!", 120.0f, 500.0f, 0.55f, glm::vec3(0.0f, 0.7f, 0.7f));
			MyFont.RenderText("Make Choice:", 280.0f, 400.0f, 0.50f, glm::vec3(1.0f, 0.7f, 0.0f));
			MyFont.RenderText("Easy", 360.0f, 345.0f, fontSize[0], fontColor[0]);
			MyFont.RenderText("Normal", 360.0f, 295.0f, fontSize[1], fontColor[1]);
			MyFont.RenderText("Difficult", 360.0f, 245.0f, fontSize[2], fontColor[2]);
			MyFont.RenderText("Start", 260.0f, 175.0f, fontSize[3], fontColor[3]);
			MyFont.RenderText("Setting", 360.0f, 175.0f, fontSize[4], fontColor[4]);
			MyFont.RenderText("Exit", 480.0f, 175.0f, fontSize[5], fontColor[5]);

			glDisable(GL_CULL_FACE);

		}
#pragma endregion

#pragma region 渲染第二页UI信息
		if (myGameManage.game_state == SECOND_UI){
			
			glEnable(GL_CULL_FACE);
			MyFont.RenderText("Setting", 350.0f, 500.0f, 0.55f, glm::vec3(0.0f, 0.7f, 0.7f));

			MyFont.RenderText("Background Music:", 190.0f, 395.0f, 0.40f, glm::vec3(1.0f, 0.7f, 0.0f));
			MyFont.RenderText("on", 390.0f, 395.0f, fontSize[6], fontColor[6]);
			MyFont.RenderText("off", 440.0f, 395.0f, fontSize[7], fontColor[7]);

			MyFont.RenderText("Sound:", 308.0f, 355.0f, 0.40f, glm::vec3(1.0f, 0.7f, 0.0f));
			MyFont.RenderText("on", 390.0f, 355.0f, fontSize[8], fontColor[8]);
			MyFont.RenderText("off", 440.0f, 355.0f, fontSize[9], fontColor[9]);

			MyFont.RenderText("Volume:", 297.0f, 315.0f, 0.40f, glm::vec3(1.0f, 0.7f, 0.0f));
			MyFont.RenderText("low", 390.0f, 315.0f, fontSize[11], fontColor[11]);
			MyFont.RenderText("normal", 460.0f, 315.0f, fontSize[10], fontColor[10]);
			MyFont.RenderText("high", 580.0f, 315.0f, fontSize[12], fontColor[12]);
			if (myGameManage.lastFirstUI_choice == DEFAULT){
				MyFont.RenderText("Back", 380.0f, 205.0f, fontSize[13], fontColor[13]);
			}
			else
			{
				MyFont.RenderText("ReturnGame", 370.0f, 205.0f, fontSize[14], fontColor[14]);
			}
			glDisable(GL_CULL_FACE);
		}
#pragma endregion

#pragma region 游戏场景渲染
		if (myGameManage.game_state == PLAYING){
			glDisable(GL_CULL_FACE);

			glm::mat4 model, view, projection;
			view = myCamera.GetViewMatrix();
			projection = glm::perspective(myCamera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.01f, 100.0f);

	
#pragma region level_easy Render
			if (myGameManage.firstUI_choice == EASY){
		      
				//todo frameBuffer_04
				if (myGameManage.gameUI_state!=UIDEFAULT||dieState==true)
				  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

				// Clear all attached buffers        
				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
				glEnable(GL_DEPTH_TEST);

				//todo 天空盒3
				mySkybox_Easy.RenderSkybox(myCamera, WIDTH, HEIGHT);

				//todo 更新游戏easy模式的纪录
				if (gameLevel > myGameManage.highScore[0]){
					myGameManage.writeHighScore(scorePath[0], gameLevel,0);
				}

				//todo 重置关卡数据
				if (resetMaze==true&&returnGame==false){

					cout << "easy重置！" << endl;
					pMaze1->initArrayMaze(mazeSize);
					pMaze1->generateMaze();
				
					//将迷宫数据载入
					myGameManage.setArrMazePointer(mazeSize, pMaze1, NULL);
					myGameManage.setCollisionBoxs();
					myGameManage.setCollisionBoxs_cube();


					//todo 随机生成cubePosition
					int posX, posZ;
					for (int i = 0; i < cubeNumber; ++i){
						do
						{
							posX = myGameManage.random(1, 2 * mazeSize - 1);
							posZ = myGameManage.random(1, 2 * mazeSize - 1);
						} while (pMaze1->arr_mazeRow[posX][posZ].state != 0);
						cubePosition[i] = glm::vec3((GLfloat)(posZ - mazeSize), 0.0f, (GLfloat)(posX - mazeSize));

					}

					//重置玩家位置
					myCamera.Position = glm::vec3(mazeSize - 1.0f, 0.0f, mazeSize - 1.0f);
					myCamera.LastPosition = myCamera.Position;
					//重置旋转角度
					myMazeRotate.angleX = 0.0f;
					myMazeRotate.angleY = 0.0f;

					//重置计时器
					timer = 0.0f;
					//重置玩家生命值
					lifes = 5;

					srand((int)time(NULL));      //重置随机数种子
					rand();

					//关闭重置关卡
					resetMaze = false;
				}//if
		
			//todo cube碰撞检测
			for (int i = 0; i < cubeNumber; ++i)
				cubeIsCrashed[i] = myGameManage.CheckCollision_Cube_AABB(cubePosition[i]);

			for (int i = 0; i < cubeNumber; ++i){
					if (cubeIsCrashed[i]==true){
						lastCubeMoveDir[i] = cubeMoveDir[i];
						switch (cubeMoveDir[i])
						{
						case 0:
							cubePosition[i].x -= moveValue[i];
							break;
						case 1:
							cubePosition[i].x += moveValue[i];
							break;
						case 2:
							cubePosition[i].z -= moveValue[i];
							break;
						case 3:
							cubePosition[i].z += moveValue[i];
							break;
						default:
							break;
						}

						do
						{
							cubeMoveDir[i] = myGameManage.random(0, 4);

						} while (cubeMoveDir[i] == lastCubeMoveDir[i]);
					}
				}//for


				//摄像机碰撞检测
				isCrashed = myGameManage.CheckCollision_Point_AABB(myCamera.Position);

				if (isCrashed)
				{
					myCamera.Position = myCamera.LastPosition;
					//std::cout << "发生碰撞！" << std::endl;
				}

				levelEasyShader.Use();
				GLuint modelLoc = glGetUniformLocation(levelEasyShader.Program, "model");
				GLuint viewLoc = glGetUniformLocation(levelEasyShader.Program, "view");
				GLuint projectionLoc = glGetUniformLocation(levelEasyShader.Program, "projection");

				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

				
				glBindVertexArray(VAO);                           //绑定VAO对象
				glBindTexture(GL_TEXTURE_2D, MyTextures[3].ID);    //绑定纹理对象
	
				//迷宫部分2
				for (int i = 0; i < 2 * mazeSize + 1; ++i)
				{
					for (int j = 0; j < 2 * mazeSize + 1; ++j){
						glm::mat4 model;
						if (pMaze1->arr_mazeRow[i][j].state == 1){
							if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
								model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
								model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
							}
							model = glm::translate(model, glm::vec3((GLfloat)pMaze1->arr_mazeRow[i][j].y - mazeSize, 0.0f, (GLfloat)(pMaze1->arr_mazeRow[i][j].x - mazeSize)));
							GLuint modelLoc = glGetUniformLocation(levelEasyShader.Program, "model");
							glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

							glDrawArrays(GL_TRIANGLES, 0, 36);
						}

					}

				}
				glBindVertexArray(0);

				//迷宫底部
				glBindVertexArray(floorVAO);
				glBindTexture(GL_TEXTURE_2D, MyTextures[4].ID);
				for (int i = 0; i < 2 * mazeSize + 1; ++i)
				{
					for (int j = 0; j < 2 * mazeSize + 1; ++j){
						glm::mat4 model;
						if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
							model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
							model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
						}
						model = glm::translate(model, glm::vec3((GLfloat)pMaze1->arr_mazeRow[i][j].y - mazeSize, -1.0f, (GLfloat)pMaze1->arr_mazeRow[i][j].x - mazeSize));
						GLuint modelLoc = glGetUniformLocation(levelEasyShader.Program, "model");
						glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

						glDrawArrays(GL_TRIANGLES, 0, 36);
					}

				}//for
				glBindVertexArray(0);


				//todo cube移动
				for (int i = 0; i < cubeNumber; ++i){

					if (myGameManage.gameUI_state == UIDEFAULT&&myCamera.moveLock==false){
						moveValue[i] = deltaTime*cubeSpeed;
						switch (cubeMoveDir[i])
						{
						case 0:
							cubePosition[i].x += moveValue[i];
							break;
						case 1:
							cubePosition[i].x -= moveValue[i];
							break;
						case 2:
							cubePosition[i].z += moveValue[i];
							break;
						case 3:
							cubePosition[i].z -= moveValue[i];
							break;
						default:
							break;
						}
						//todo 当cube撞到玩家时
						if (isHurted == false && abs(cubePosition[i].x - myCamera.Position.x) <= 0.5f&&abs(cubePosition[i].z - myCamera.Position.z) <= 0.5f){
							//在播放失败/胜利声音时停止碰撞检测
							if (playWinSound == false && playOverSound == false){
								--lifes;
								isHurted = true;
								index = i;

								cout << "hurted!" << endl;
								if (playHurtSound == true){
									myGameManage.hurtSound.play();      //播放受到攻击音效
									playHurtSound = false;
								}
							}

						}
						if (isHurted == true && abs(cubePosition[index].x - myCamera.Position.x) > 0.5f&&abs(cubePosition[index].z - myCamera.Position.z) > 0.5f){
							isHurted = false;

							playHurtSound = true;                      //恢复攻击音效的播放
						}

					}

					

					lampShader.Use();
					glm::mat4 model;
					GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
					GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
					GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
					glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
					glUniform3f(glGetUniformLocation(lampShader.Program, "myColor"), 1.0f, 1.0f, 0.0f);

					glBindVertexArray(CubePosVAO);
					if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
						model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
						model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
					}

					model = glm::translate(model, cubePosition[i]);
					model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

					modelLoc = glGetUniformLocation(lampShader.Program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

					glDrawArrays(GL_TRIANGLES, 0, 36);
					glBindVertexArray(0);

				}//for


				
				//todo frameBuffer_05
				if (myGameManage.gameUI_state != UIDEFAULT||dieState==true){
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					// Clear all relevant buffers
					glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT);
					glDisable(GL_DEPTH_TEST);

					// Draw Screen
					screenShader.Use();
					glBindVertexArray(quadVAO);
					if (dieState == true || myGameManage.gameUI_state == SECOND_UI||myGameManage.gameUI_state==HIGHSCORE_UI){
						glUniform1i(glGetUniformLocation(screenShader.Program, "state"), 1);
					}
					else if (myGameManage.gameUI_state==OVER_UI)
					{
						glUniform1i(glGetUniformLocation(screenShader.Program, "state"), 2);
					}
					else
					{
						glUniform1i(glGetUniformLocation(screenShader.Program, "state"), 0);
					}
					glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glBindVertexArray(0);
				
				}


			}//easy

#pragma endregion

#pragma region level_Normal Render
			if (myGameManage.firstUI_choice == _NORMAL){

				//todo frameBuffer_04
				if (myGameManage.gameUI_state != UIDEFAULT||dieState==true){
					glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
				}
				// Clear all attached buffers        
				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);


				//todo 天空盒3
				mySkybox_Normal.RenderSkybox(myCamera, WIDTH, HEIGHT);

				//todo 更新游戏nomal模式的纪录
				if (gameLevel > myGameManage.highScore[1]){
					myGameManage.writeHighScore(scorePath[1], gameLevel, 1);
				}

				int dirNum;             //A*算法产生的方向变化次数
				//todo 重置关卡数据
				if (resetMaze == true&&returnGame==false){

					cout << "normal重置！" << endl;
					//载入迷宫数据
					pMaze2->initArrayMaze(mazeSize);
					pMaze2->generateMaze();

					//随机生成cubePosition
					int posX, posZ;
					for (int i = 0; i < cubeNumber; ++i){
						do
						{
							posX = myGameManage.random(1, 2 * mazeSize - 2);
							posZ = myGameManage.random(1, 2 * mazeSize - 2);
						} while (pMaze2->arr_mazeRow[posX][posZ].state != 0);
						cubePosition[i] = glm::vec3((GLfloat)(posZ - mazeSize), 0.0f, (GLfloat)(posX - mazeSize));

					}

					//载入迷宫数据并设置相应的碰撞器
					myGameManage.setArrMazePointer(mazeSize, NULL, pMaze2);
					myGameManage.setCollisionBoxs();
					myGameManage.setCollisionBoxs_cube();

					//todo A*寻路算法2
					astar.FindPath(*pMaze2);
					dirNum = astar.dirNum - 2;    //获取方向改变的次数
					_cubeMoveDir = astar.moveDir[dirNum];

					
					//重置玩家位置
					myCamera.Position = glm::vec3(mazeSize - 1.0f, 0.0f, mazeSize - 1.0f);
					myCamera.LastPosition = myCamera.Position;

					//重置路径方块位置
					_cubePosition = glm::vec3(-mazeSize + 1.0f, 0.0f, -mazeSize + 1.0f);
					lastCubePosition = _cubePosition;
				    //重置旋转角度
					myMazeRotate.angleX = 0.0f;
					myMazeRotate.angleY = 0.0f;

					//重置玩家生命值
					lifes = 5;

					//重置时间属性
					timer = 0.0f;

					//关闭关卡重置功能
					resetMaze = false;

				}//if

				//cube碰撞检测
				for (int i = 0; i < cubeNumber; ++i)
					cubeIsCrashed[i] = myGameManage.CheckCollision_Cube_AABB(cubePosition[i]);

				srand((int)time(NULL));      //重置随机数种子
				rand();
				for (int i = 0; i < cubeNumber; ++i){
					if (cubeIsCrashed[i]){

						lastCubeMoveDir[i] = cubeMoveDir[i];

						int oppositeDir;
						switch (cubeMoveDir[i])
						{
						case 0:
							oppositeDir = 1;
							break;
						case 1:
							oppositeDir = 0;
							break;
						case 2:
							oppositeDir = 3;
							break;
						case 3:
							oppositeDir = 2;
							break;
						default:
							break;
						}

						switch (cubeMoveDir[i])
						{
						case 0:
							cubePosition[i].x -= moveValue[i];
							break;
						case 1:
							cubePosition[i].x += moveValue[i];
							break;
						case 2:
							cubePosition[i].z -= moveValue[i];
							break;
						case 3:
							cubePosition[i].z += moveValue[i];
							break;
						default:
							break;
						}

						do
						{
							cubeMoveDir[i] = myGameManage.random(0, 4);

						} while (cubeMoveDir[i] == lastCubeMoveDir[i]);
					}
				}//for


				//todo A*寻路算法3
				if (dirNum >= 0){
					switch (_cubeMoveDir)
					{
					case 0:
						if (abs(_cubePosition.x - lastCubePosition.x) >= 1.0f){
							lastCubePosition = _cubePosition;
							_cubeMoveDir = astar.moveDir[--dirNum];
						}
						break;
					case 1:
						if (abs(_cubePosition.x - lastCubePosition.x) >= 1.0f){
							lastCubePosition = _cubePosition;
							_cubeMoveDir = astar.moveDir[--dirNum];
						}
						break;
					case 2:
						if (abs(_cubePosition.z - lastCubePosition.z) >= 1.0f){
							lastCubePosition = _cubePosition;
							_cubeMoveDir = astar.moveDir[--dirNum];
						}
						break;
					case 3:
						if (abs(_cubePosition.z - lastCubePosition.z) >= 1.0f){
							lastCubePosition = _cubePosition;
							_cubeMoveDir = astar.moveDir[--dirNum];
						}
						break;
					default:
						break;
					}
				}

				//摄像机碰撞检测
				isCrashed = myGameManage.CheckCollision_Point_AABB(myCamera.Position);
				if (isCrashed)
				{
					myCamera.Position = myCamera.LastPosition;
					//std::cout << "发生碰撞！" << std::endl;
				}



				if (myCamera.moveLock == false){

					levelNormalShader.Use();
					GLint viewPosLoc = glGetUniformLocation(levelNormalShader.Program, "viewPos");
					glUniform3f(viewPosLoc, myCamera.Position.x, myCamera.Position.y, myCamera.Position.z);
					//设置材质属性
					glUniform1f(glGetUniformLocation(levelNormalShader.Program, "material.shininess"), 32.0f);

					//关闭聚光灯
					glUniform1i(glGetUniformLocation(levelNormalShader.Program, "openSpotLight"), 0);

					//打开跟随玩家的点光源
					glUniform3f(glGetUniformLocation(levelNormalShader.Program, "pointLight.position"), myCamera.Position.x, myCamera.Position.y, myCamera.Position.z);
					glUniform3f(glGetUniformLocation(levelNormalShader.Program, "pointLight.ambient"), 0.1f, 0.1f, 0.1f);
					glUniform3f(glGetUniformLocation(levelNormalShader.Program, "pointLight.diffuse"), 0.5f, 0.5f, 0.5f);
					glUniform3f(glGetUniformLocation(levelNormalShader.Program, "pointLight.specular"), 1.0f, 1.0f, 1.0f);
					glUniform1f(glGetUniformLocation(levelNormalShader.Program, "pointLight.constant"), 1.0f);
					glUniform1f(glGetUniformLocation(levelNormalShader.Program, "pointLight.linear"), 0.14f);
					glUniform1f(glGetUniformLocation(levelNormalShader.Program, "pointLight.quadratic"), 0.07f);
				   


					// Bind diffuse map
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, MyTextures[0].ID);
					glUniform1i(glGetUniformLocation(levelNormalShader.Program, "material.diffuse"), 0);

					// Bind specular map
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, MyTextures[1].ID);
					glUniform1i(glGetUniformLocation(levelNormalShader.Program, "material.specular"), 1);

				}
				else
				{
					levelNormalShader.Use();
					GLint viewPosLoc = glGetUniformLocation(levelNormalShader.Program, "viewPos");
					glUniform3f(viewPosLoc, myCamera.Position.x, myCamera.Position.y, myCamera.Position.z);
					//设置材质属性
					glUniform1f(glGetUniformLocation(levelNormalShader.Program, "material.shininess"), 32.0f);

					//开启聚光灯
					glUniform1i(glGetUniformLocation(levelNormalShader.Program, "openSpotLight"), 1);

					// SpotLight
					glUniform3f(glGetUniformLocation(levelNormalShader.Program, "spotLight.position"), 0.0f, 10.0f, -30.0f);
					glUniform3f(glGetUniformLocation(levelNormalShader.Program, "spotLight.direction"), -0.017f, -0.167f, 0.596f);
					glUniform1f(glGetUniformLocation(levelNormalShader.Program, "spotLight.linear"), 0.014);
					glUniform1f(glGetUniformLocation(levelNormalShader.Program, "spotLight.quadratic"), 0.0007);
					glUniform1f(glGetUniformLocation(levelNormalShader.Program, "spotLight.inerCutOff"), glm::cos(glm::radians(55.5f)));
					glUniform1f(glGetUniformLocation(levelNormalShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(60.0f)));

					glUniform3f(glGetUniformLocation(levelNormalShader.Program, "spotLight.ambient"), 0.1f, 0.1f, 0.1f);
					glUniform3f(glGetUniformLocation(levelNormalShader.Program, "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
					glUniform3f(glGetUniformLocation(levelNormalShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
					glUniform1f(glGetUniformLocation(levelNormalShader.Program, "spotLight.constant"), 1.0f);
					


					// Bind diffuse map
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, MyTextures[0].ID);
					glUniform1i(glGetUniformLocation(levelNormalShader.Program, "material.diffuse"), 0);

					//Bind specular map
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, MyTextures[1].ID);
					glUniform1i(glGetUniformLocation(levelNormalShader.Program, "material.specular"), 1);

				}

				GLint modelLoc_light = glGetUniformLocation(levelNormalShader.Program, "model");
				GLint viewLoc_light = glGetUniformLocation(levelNormalShader.Program, "view");
				GLint projLoc_light = glGetUniformLocation(levelNormalShader.Program, "projection");
				// Pass the matrices to the shader
				glUniformMatrix4fv(viewLoc_light, 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(projLoc_light, 1, GL_FALSE, glm::value_ptr(projection));


#pragma region 迷宫渲染
				//绑定VAO对象
				glBindVertexArray(VAO);
				//迷宫部分1
				for (int i = 0; i < 2 * mazeSize + 1; ++i)
				{
					for (int j = 0; j < 2 * mazeSize + 1; ++j){
						glm::mat4 model;
						GLuint modelLoc;
						if (pMaze2->arr_mazeRow[i][j].state == 1){

							if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
								model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
								model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
							}
							model = glm::translate(model, glm::vec3((GLfloat)pMaze2->arr_mazeRow[i][j].y - mazeSize, 1.0f, (GLfloat)pMaze2->arr_mazeRow[i][j].x - mazeSize));
							modelLoc = glGetUniformLocation(levelNormalShader.Program, "model");
							glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
							glDrawArrays(GL_TRIANGLES, 0, 36);
						}

					}

				}
				//迷宫部分2
				for (int i = 0; i < 2 * mazeSize + 1; ++i)
				{
					for (int j = 0; j < 2 * mazeSize + 1; ++j){
						glm::mat4 model;
						GLuint modelLoc;
						if (pMaze2->arr_mazeRow[i][j].state == 1){
							if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
								model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
								model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
							}
							model = glm::translate(model, glm::vec3((GLfloat)pMaze2->arr_mazeRow[i][j].y - mazeSize , 0.0f, (GLfloat)pMaze2->arr_mazeRow[i][j].x - mazeSize));
							modelLoc = glGetUniformLocation(levelNormalShader.Program, "model");
							glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
							glDrawArrays(GL_TRIANGLES, 0, 36);
						}

					}

				}

				//将迷宫底部的镜面反射图换成箱子
				if (myCamera.moveLock == true){
					// Bind specular map
					glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D, MyTextures[0].ID);
					glUniform1i(glGetUniformLocation(levelNormalShader.Program, "material.specular"), 2);

				}

				//迷宫底部
				for (int i = 0; i < 2 * mazeSize + 1; ++i)
				{
					for (int j = 0; j < 2 * mazeSize + 1; ++j){
						glm::mat4 model;
						GLuint modelLoc;
						if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
							model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
							model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
						}
						model = glm::translate(model, glm::vec3((GLfloat)pMaze2->arr_mazeRow[i][j].y - mazeSize, -1.0f, (GLfloat)pMaze2->arr_mazeRow[i][j].x - mazeSize));
						modelLoc = glGetUniformLocation(levelNormalShader.Program, "model");
						glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

						glDrawArrays(GL_TRIANGLES, 0, 36);
					}

				}
#pragma endregion
				//todo cube移动
				for (int i = 0; i < cubeNumber; ++i){

					glm::mat4 model;
					if (myGameManage.gameUI_state == UIDEFAULT&&myCamera.moveLock == false){
						
						moveValue[i] = deltaTime*cubeSpeed;
						switch (cubeMoveDir[i])
						{
						case 0:
							cubePosition[i].x += moveValue[i];
							break;
						case 1:
							cubePosition[i].x -= moveValue[i];
							break;
						case 2:
							cubePosition[i].z += moveValue[i];
							break;
						case 3:
							cubePosition[i].z -= moveValue[i];
							break;
						default:
							break;
						}
					}

						//todo 当cube撞到玩家时
						if (isHurted == false && abs(cubePosition[i].x - myCamera.Position.x) <= 0.5f&&abs(cubePosition[i].z - myCamera.Position.z) <= 0.5f){
							--lifes;
							isHurted = true;
							index = i;
							cout << "hurted!" << endl;
							if (playHurtSound == true){
								myGameManage.hurtSound.play();
								playHurtSound = false;
							}

						
						if (isHurted == true && abs(cubePosition[index].x - myCamera.Position.x) > 0.5f&&abs(cubePosition[index].z - myCamera.Position.z) > 0.5f){
							isHurted = false;

							playHurtSound = true;       //恢复被攻击音效的播放设置
						}
					}
		

					lampShader.Use();
					GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
					GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
					GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
					glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
					glUniform3f(glGetUniformLocation(lampShader.Program, "myColor"), 1.0f, 1.0f, 0.0f);

					glBindVertexArray(CubePosVAO);
					if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
						model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
						model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
					}
					
					model = glm::translate(model, cubePosition[i]);
					model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			
					modelLoc = glGetUniformLocation(lampShader.Program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

					glDrawArrays(GL_TRIANGLES, 0, 36);


				}//for

				//todo A*算法4
					if (_cubePosition.x < (mazeSize - 1.0f) || _cubePosition.z < (mazeSize - 1.0f)){

						if (myGameManage.gameUI_state == UIDEFAULT){
							_moveValue = deltaTime*cubeSpeed;
							switch (_cubeMoveDir)
							{
							case 0:
								_cubePosition.x += _moveValue;
								break;
							case 1:
								_cubePosition.x -= _moveValue;
								break;
							case 2:
								_cubePosition.z += _moveValue;
								break;
							case 3:
								_cubePosition.z -= _moveValue;
								break;
							default:
								break;
							}

						}

					lampShader.Use();
					GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
					GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
					GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
					glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
					glUniform3f(glGetUniformLocation(lampShader.Program, "myColor"), 0.0f, 1.0f, 0.0f);

					glBindVertexArray(CubePosVAO);
					if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
						model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
						model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
					}

					model = glm::translate(model, _cubePosition);
					model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

					modelLoc = glGetUniformLocation(lampShader.Program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

					glDrawArrays(GL_TRIANGLES, 0, 36);
					
				}
				else
				{
					_cubePosition = glm::vec3(-mazeSize + 1.0f, 0.0f, -mazeSize + 1.0f);
					dirNum = astar.dirNum - 2;
					_cubeMoveDir = astar.moveDir[dirNum];
					lastCubePosition = _cubePosition;
				}

				//解绑VAO对象
				glBindVertexArray(0);

				//todo frameBuffer_05
				if (myGameManage.gameUI_state != UIDEFAULT||dieState==true){
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					// Clear all relevant buffers
					glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT);
					glDisable(GL_DEPTH_TEST);

					// Draw Screen
					screenShader.Use();
					glBindVertexArray(quadVAO);

					if (dieState == true || myGameManage.gameUI_state == SECOND_UI){
						glUniform1i(glGetUniformLocation(screenShader.Program, "state"), 1);
					}
					else if (myGameManage.gameUI_state == OVER_UI)
					{
						glUniform1i(glGetUniformLocation(screenShader.Program, "state"), 2);
					}
					else
					{
						glUniform1i(glGetUniformLocation(screenShader.Program, "state"), 0);
					}

					glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glBindVertexArray(0);

				}

			}//noraml

#pragma endregion	

#pragma region level_Difficult Render
			if (myGameManage.firstUI_choice == DIFFICULT){

				//todo frameBuffer_04
				if (myGameManage.gameUI_state != UIDEFAULT || dieState == true){
					glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
				}
				// Clear all attached buffers        
				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);

				//todo 天空盒3
				mySkybox_Normal.RenderSkybox(myCamera, WIDTH, HEIGHT);

				//todo 更新游戏difficlut模式的纪录
				if (gameLevel > myGameManage.highScore[2]){
					myGameManage.writeHighScore(scorePath[2], gameLevel, 2);
				}

				int dirNum;             //A*算法产生的方向变化次数
				//todo 重置关卡数据
				if (resetMaze == true && returnGame == false){

					//载入迷宫数据
					pMaze2->initArrayMaze(mazeSize);
					pMaze2->generateMaze();

					//随机生成cubePosition
					int posX, posZ;
					for (int i = 0; i < cubeNumber; ++i){
						do
						{
							posX = myGameManage.random(1, 2 * mazeSize - 2);
							posZ = myGameManage.random(1, 2 * mazeSize - 2);
						} while (pMaze2->arr_mazeRow[posX][posZ].state != 0);
						cubePosition[i] = glm::vec3((GLfloat)(posZ - mazeSize), 0.0f, (GLfloat)(posX - mazeSize));

					}

					//载入迷宫数据并设置相应的碰撞器
					myGameManage.setArrMazePointer(mazeSize, NULL, pMaze2);
					myGameManage.setCollisionBoxs();
					myGameManage.setCollisionBoxs_cube();

					//todo A*寻路算法2
					astar.FindPath(*pMaze2);
					dirNum = astar.dirNum - 2;    //获取方向改变的次数
					_cubeMoveDir = astar.moveDir[dirNum];

					std::cout << astar.dirNum << std::endl;
					//重置玩家位置
					myCamera.Position = glm::vec3(mazeSize - 1.0f, 0.0f, mazeSize - 1.0f);
					myCamera.LastPosition = myCamera.Position;

					//重置路径方块位置
					_cubePosition = glm::vec3(-mazeSize + 1.0f, 0.0f, -mazeSize + 1.0f);
					lastCubePosition = _cubePosition;
					//重置旋转角度
					myMazeRotate.angleX = 0.0f;
					myMazeRotate.angleY = 0.0f;

					//重置玩家生命值
					lifes = 5;

					//重置时间属性
					timer = 0.0f;

					//关闭关卡重置功能
					resetMaze = false;

				}//if

				//cube碰撞检测
				for (int i = 0; i < cubeNumber; ++i)
					cubeIsCrashed[i] = myGameManage.CheckCollision_Cube_AABB(cubePosition[i]);

				srand((int)time(NULL));      //重置随机数种子
				rand();
				for (int i = 0; i < cubeNumber; ++i){
					if (cubeIsCrashed[i]){

						lastCubeMoveDir[i] = cubeMoveDir[i];
						int oppositeDir;
						switch (cubeMoveDir[i])
						{
						case 0:
							oppositeDir = 1;
							break;
						case 1:
							oppositeDir = 0;
							break;
						case 2:
							oppositeDir = 3;
							break;
						case 3:
							oppositeDir = 2;
							break;
						default:
							break;
						}

						switch (cubeMoveDir[i])
						{
						case 0:
							cubePosition[i].x -= moveValue[i];
							break;
						case 1:
							cubePosition[i].x += moveValue[i];
							break;
						case 2:
							cubePosition[i].z -= moveValue[i];
							break;
						case 3:
							cubePosition[i].z += moveValue[i];
							break;
						default:
							break;
						}

						do
						{
							cubeMoveDir[i] = myGameManage.random(0, 4);

						} while (cubeMoveDir[i] == lastCubeMoveDir[i]);
					}
				}//for


				//todo A*寻路算法3
				if (dirNum >= 0){
					switch (_cubeMoveDir)
					{
					case 0:
						if (abs(_cubePosition.x - lastCubePosition.x) >= 1.0f){
							lastCubePosition = _cubePosition;
							_cubeMoveDir = astar.moveDir[--dirNum];
						}
						break;
					case 1:
						if (abs(_cubePosition.x - lastCubePosition.x) >= 1.0f){
							lastCubePosition = _cubePosition;
							_cubeMoveDir = astar.moveDir[--dirNum];
						}
						break;
					case 2:
						if (abs(_cubePosition.z - lastCubePosition.z) >= 1.0f){
							lastCubePosition = _cubePosition;
							_cubeMoveDir = astar.moveDir[--dirNum];
						}
						break;
					case 3:
						if (abs(_cubePosition.z - lastCubePosition.z) >= 1.0f){
							lastCubePosition = _cubePosition;
							_cubeMoveDir = astar.moveDir[--dirNum];
						}
						break;
					default:
						break;
					}
				}

				//摄像机碰撞检测
				isCrashed = myGameManage.CheckCollision_Point_AABB(myCamera.Position);
				if (isCrashed)
				{
					myCamera.Position = myCamera.LastPosition;
					//std::cout << "发生碰撞！" << std::endl;
				}

				levelDifficultShader.Use();

				GLint viewPosLoc = glGetUniformLocation(levelDifficultShader.Program, "viewPos");
				glUniform3f(viewPosLoc, myCamera.Position.x, myCamera.Position.y, myCamera.Position.z);

				//设置材质属性
				glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "material.shininess"), 32.0f);

				if (openFlashlight == true){
					//todo 玩家手电筒的模拟
					if (myCamera.moveLock == false){
						glUniform3f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.position"), myCamera.Position.x, myCamera.Position.y, myCamera.Position.z);
						glUniform3f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.direction"), myCamera.spotLightDir.x, myCamera.spotLightDir.y, myCamera.spotLightDir.z);
						glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.linear"), 0.09);
						glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.quadratic"), 0.032);
						glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.inerCutOff"), glm::cos(glm::radians(20.5f)));
						glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(25.0f)));

					}
					else
					{
						glUniform3f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.position"), 0.0f, 10.0f, -30.0f);
						glUniform3f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.direction"), -0.017f, -0.167f, 0.596f);
						glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.linear"), 0.014);
						glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.quadratic"), 0.0007);
						glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.inerCutOff"), glm::cos(glm::radians(55.5f)));
						glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(60.0f)));

					}
				}
				else
				{
					glUniform3f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.position"), 0.0f, 10.0f, 30.0f);
					glUniform3f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.direction"), -0.017f, -0.167f, 0.596f);
					glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.linear"), 0.014);
					glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.quadratic"), 0.0007);
					glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.inerCutOff"), glm::cos(glm::radians(55.5f)));
					glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(60.0f)));
				}
				glUniform3f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.ambient"), 0.1f, 0.1f, 0.1f);
				glUniform3f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
				glUniform3f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
				glUniform1f(glGetUniformLocation(levelDifficultShader.Program, "spotLight.constant"), 1.0f);
	
				GLint modelLoc_light = glGetUniformLocation(levelDifficultShader.Program, "model");
				GLint viewLoc_light = glGetUniformLocation(levelDifficultShader.Program, "view");
				GLint projLoc_light = glGetUniformLocation(levelDifficultShader.Program, "projection");
				// Pass the matrices to the shader
				glUniformMatrix4fv(viewLoc_light, 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(projLoc_light, 1, GL_FALSE, glm::value_ptr(projection));

				// Bind diffuse map
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, MyTextures[0].ID);
				glUniform1i(glGetUniformLocation(levelDifficultShader.Program, "material.diffuse"), 0);

				// Bind specular map
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, MyTextures[1].ID);
				glUniform1i(glGetUniformLocation(levelDifficultShader.Program, "material.specular"), 1);

#pragma region 迷宫渲染
				//绑定VAO对象
				glBindVertexArray(VAO);
				//迷宫部分1
				for (int i = 0; i < 2 * mazeSize + 1; ++i)
				{
					for (int j = 0; j < 2 * mazeSize + 1; ++j){
						glm::mat4 model;
						GLuint modelLoc;
						if (pMaze2->arr_mazeRow[i][j].state == 1){

							if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
								model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
								model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
							}
							model = glm::translate(model, glm::vec3((GLfloat)pMaze2->arr_mazeRow[i][j].y - mazeSize, 1.0f, (GLfloat)pMaze2->arr_mazeRow[i][j].x - mazeSize));
							modelLoc = glGetUniformLocation(levelDifficultShader.Program, "model");
							glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
							glDrawArrays(GL_TRIANGLES, 0, 36);
						}

					}

				}
				//迷宫部分2
				for (int i = 0; i < 2 * mazeSize + 1; ++i)
				{
					for (int j = 0; j < 2 * mazeSize + 1; ++j){
						glm::mat4 model;
						GLuint modelLoc;
						if (pMaze2->arr_mazeRow[i][j].state == 1){
							if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
								model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
								model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
							}
							model = glm::translate(model, glm::vec3((GLfloat)pMaze2->arr_mazeRow[i][j].y - mazeSize, 0.0f, (GLfloat)pMaze2->arr_mazeRow[i][j].x - mazeSize));
							modelLoc = glGetUniformLocation(levelDifficultShader.Program, "model");
							glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
							glDrawArrays(GL_TRIANGLES, 0, 36);
						}

					}

				}

				//迷宫底部
				for (int i = 0; i < 2 * mazeSize + 1; ++i)
				{
					for (int j = 0; j < 2 * mazeSize + 1; ++j){
						glm::mat4 model;
						GLuint modelLoc;
						if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
							model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
							model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
						}
						model = glm::translate(model, glm::vec3((GLfloat)pMaze2->arr_mazeRow[i][j].y - mazeSize, -1.0f, (GLfloat)pMaze2->arr_mazeRow[i][j].x - mazeSize));
						modelLoc = glGetUniformLocation(levelDifficultShader.Program, "model");
						glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

						glDrawArrays(GL_TRIANGLES, 0, 36);
					}

				}
#pragma endregion
				//todo cube移动
				for (int i = 0; i < cubeNumber; ++i){
					glm::mat4 model;
					if (myGameManage.gameUI_state == UIDEFAULT&&myCamera.moveLock == false){
						moveValue[i] = deltaTime*cubeSpeed;
						switch (cubeMoveDir[i])
						{
						case 0:
							cubePosition[i].x += moveValue[i];
							break;
						case 1:
							cubePosition[i].x -= moveValue[i];
							break;
						case 2:
							cubePosition[i].z += moveValue[i];
							break;
						case 3:
							cubePosition[i].z -= moveValue[i];
							break;
						default:
							break;
						}
					}


					//todo 当cube撞到玩家时
					if (isHurted == false && openFlashlight == true && abs(cubePosition[i].x - myCamera.Position.x) <= 0.5f&&abs(cubePosition[i].z - myCamera.Position.z) <= 0.5f){
						--lifes;
						isHurted = true;
						index = i;
						cout << "hurted!" << endl;
						if (playHurtSound == true){
							myGameManage.hurtSound.play();
							playHurtSound = false;
						}

					}
					if (isHurted == true && abs(cubePosition[index].x - myCamera.Position.x) > 0.5f&&abs(cubePosition[index].z - myCamera.Position.z) > 0.5f){
						isHurted = false;

						playHurtSound = true;       //恢复被攻击音效的播放设置
					}

					lampShader.Use();
					GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
					GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
					GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
					glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
					glUniform3f(glGetUniformLocation(lampShader.Program, "myColor"), 1.0f, 1.0f, 0.0f);

					glBindVertexArray(CubePosVAO);
					if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
						model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
						model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
					}

					model = glm::translate(model, cubePosition[i]);
					model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

					modelLoc = glGetUniformLocation(lampShader.Program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

					glDrawArrays(GL_TRIANGLES, 0, 36);


				}//for

				//todo A*算法4
				if (_cubePosition.x < (mazeSize - 1.0f) || _cubePosition.z < (mazeSize - 1.0f)){
					
					if (myGameManage.gameUI_state == UIDEFAULT){
						_moveValue = deltaTime*cubeSpeed;
						switch (_cubeMoveDir)
						{
						case 0:
							_cubePosition.x += _moveValue;
							break;
						case 1:
							_cubePosition.x -= _moveValue;
							break;
						case 2:
							_cubePosition.z += _moveValue;
							break;
						case 3:
							_cubePosition.z -= _moveValue;
							break;
						default:
							break;
						}
					}
	
					lampShader.Use();
					GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
					GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
					GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
					glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
					glUniform3f(glGetUniformLocation(lampShader.Program, "myColor"), 0.0f, 1.0f, 0.0f);

					glBindVertexArray(CubePosVAO);
					if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
						model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
						model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
					}

					model = glm::translate(model, _cubePosition);
					model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

					modelLoc = glGetUniformLocation(lampShader.Program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

					glDrawArrays(GL_TRIANGLES, 0, 36);

				}
				else
				{
					_cubePosition = glm::vec3(-mazeSize + 1.0f, 0.0f, -mazeSize + 1.0f);
					dirNum = astar.dirNum - 2;
					_cubeMoveDir = astar.moveDir[dirNum];
					lastCubePosition = _cubePosition;
				}

				//解绑VAO对象
				glBindVertexArray(0);

				//todo frameBuffer_05
				if (myGameManage.gameUI_state != UIDEFAULT||dieState==true){
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					// Clear all relevant buffers
					glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT);
					glDisable(GL_DEPTH_TEST);

					// Draw Screen
					screenShader.Use();
					glBindVertexArray(quadVAO);
					if (dieState == true || myGameManage.gameUI_state == SECOND_UI){
						glUniform1i(glGetUniformLocation(screenShader.Program, "state"), 1);
					}
					else if (myGameManage.gameUI_state == OVER_UI)
					{
						glUniform1i(glGetUniformLocation(screenShader.Program, "state"), 2);
					}
					else
					{
						glUniform1i(glGetUniformLocation(screenShader.Program, "state"), 0);
					}
					glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glBindVertexArray(0);

				}


			}//difficult

#pragma endregion

#pragma endregion

			if (myGameManage.gameUI_state == SECOND_UI){


				glEnable(GL_CULL_FACE);
				MyFont.RenderText("Setting", 350.0f, 500.0f, 0.55f, glm::vec3(0.0f, 0.7f, 0.7f));

				MyFont.RenderText("Background Music:", 190.0f, 395.0f, 0.40f, glm::vec3(1.0f, 0.7f, 0.0f));
				MyFont.RenderText("on", 390.0f, 395.0f, fontSize[6], fontColor[6]);
				MyFont.RenderText("off", 440.0f, 395.0f, fontSize[7], fontColor[7]);

				MyFont.RenderText("Sound:", 308.0f, 355.0f, 0.40f, glm::vec3(1.0f, 0.7f, 0.0f));
				MyFont.RenderText("on", 390.0f, 355.0f, fontSize[8], fontColor[8]);
				MyFont.RenderText("off", 440.0f, 355.0f, fontSize[9], fontColor[9]);

				MyFont.RenderText("Volume:", 297.0f, 315.0f, 0.40f, glm::vec3(1.0f, 0.7f, 0.0f));
				MyFont.RenderText("low", 390.0f, 315.0f, fontSize[11], fontColor[11]);
				MyFont.RenderText("normal", 460.0f, 315.0f, fontSize[10], fontColor[10]);
				MyFont.RenderText("high", 580.0f, 315.0f, fontSize[12], fontColor[12]);
				if (myGameManage.lastFirstUI_choice == DEFAULT){
					MyFont.RenderText("Back", 380.0f, 205.0f, fontSize[13], fontColor[13]);
				}
				else
				{
					MyFont.RenderText("ReturnGame", 370.0f, 205.0f, fontSize[14], fontColor[14]);
				}
				glDisable(GL_CULL_FACE);

			}
	
			else if (myGameManage.gameUI_state == HIGHSCORE_UI){

				glEnable(GL_CULL_FACE);
				MyFont.RenderText("HighScore", 350.0f, 500.0f, 0.55f, glm::vec3(0.0f, 0.7f, 0.7f));
				MyFont.RenderText("Easy_Level:", 275.0f, 395.0f, 0.40f, glm::vec3(1.0f, 0.7f, 0.0f));
				MyFont.RenderText("Normal_Level:", 250.0f, 335.0f, 0.40f, glm::vec3(1.0f, 0.7f, 0.0f));
				MyFont.RenderText("Difficult_Level:", 250.0f, 275.0f, 0.40f, glm::vec3(1.0f, 0.7f, 0.0f));

				MyFont.RenderText(std::to_string(myGameManage.highScore[0]), 420.0f, 395.0f, 0.40f, glm::vec3(0.7f, 0.0f, 0.0f));

				MyFont.RenderText(std::to_string(myGameManage.highScore[1]), 420.0f, 335.0f, 0.40f, glm::vec3(0.7f, 0.0f, 0.0f));

				MyFont.RenderText(std::to_string(myGameManage.highScore[2]), 420.0f, 275.0f, 0.40f, glm::vec3(0.7f, 0.0f, 0.0f));
				glDisable(GL_CULL_FACE);

			}
			else if (myGameManage.gameUI_state == OVER_UI){
				//重置时间
				minute = 3;
				//重置倒计时音效播放
				countDown = true;

				glEnable(GL_CULL_FACE);
				MyFont.RenderText("Game Over!", 320.0f, 500.0f, 0.55f, glm::vec3(0.7f, 0.0f, 0.0f));
				MyFont.RenderText("Try again", 280.0f, 355.0f, fontSize[15], fontColor[15]);
				MyFont.RenderText("Return home", 450.0f, 355.0f, fontSize[16], fontColor[16]);
				glDisable(GL_CULL_FACE);

				myCamera.moveLock = true;
			}
			else if (myGameManage.gameUI_state == WIN_UI){
			
				//重置倒计时音效播放
				countDown = true;

				glEnable(GL_CULL_FACE);
				MyFont.RenderText("Congratulate,mission accomplished!", 170.0f, 500.0f, 0.55f, glm::vec3(0.7f, 0.0f, 0.0f));
				MyFont.RenderText("Play again", 280.0f, 355.0f, fontSize[17], fontColor[17]);
				MyFont.RenderText("Next mission", 450.0f, 355.0f, fontSize[18], fontColor[18]);
				glDisable(GL_CULL_FACE);

				myCamera.moveLock = true;
			}
			else
			{
				//todo 显示游戏时间
				timer += deltaTime;
				if (playOverSound == false && playWinSound == false)
					second = 59 - int(timer) % 60;

				minute_str = std::to_string(minute);
				second_str = std::to_string(second);
				if (minute >= 10) MyFont.RenderText(minute_str + ":", 305.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.7f, 0.0f));
				else MyFont.RenderText("0" + minute_str + ":", 305.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.7f, 0.0f));

				if (second < 10) {

					if (minute == 0){
						MyFont.RenderText("0" + second_str, 345.0f, 530.0f, 0.6f, glm::vec3(0.0f, 0.0f, 0.7f));
						if (countDown == true){

							lastSecond = second;
							myGameManage.countDownSound.play();
							countDown = false;

						}

						if ((lastSecond - second) == 1)
							countDown = true;

					}
					else
					{
						MyFont.RenderText("0" + second_str, 345.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.7f, 0.0f));
						//播放倒计时声音
						if (countDown == true){
							lastSecond = second;
							myGameManage.timeSound.play();
							countDown = false;
						}

						if ((lastSecond - second) == 1)
							countDown = true;
					}

				}
				else{
					MyFont.RenderText(second_str, 345.0f, 530.0f, 0.5f, glm::vec3(0.0f, 0.7f, 0.0f));
					if (countDown == true){
						lastSecond = second;
						myGameManage.timeSound.play();
						countDown = false;

					}
					if ((lastSecond - second) == 1)
						countDown = true;
				}


				//显示关卡信息
				gameLevel_str = std::to_string(gameLevel);
				MyFont.RenderText("level:" + gameLevel_str, 355.0f, 570.0f, 0.5f, glm::vec3(0.7f, 0.7f, 0.0f));

				//todo 显示玩家生命次数
				lifes_str = std::to_string(lifes);
				MyFont.RenderText("Life:" + lifes_str, 405.0f, 530.0f, 0.5f, glm::vec3(0.7f, 0.0f, 0.0f));

				//当游戏限定时间消耗完或玩家剩余生命为0时
				if (playBgMusic == true && ((minute == 0 && second == 0) || lifes == 0)){

					lastTimer = timer;
					myGameManage.bgMusic.pausePlay();
					myGameManage.overSound.play();
					playOverSound = true;
					playBgMusic = false;
					
					myCamera.moveLock = true;

					dieState = true;
				}
				//todo 当玩家走到出口
				else if (playBgMusic == true && myCamera.Position.x < (-mazeSize + 1.5f) && myCamera.Position.x >(-mazeSize + 0.5f) && myCamera.Position.z < (-mazeSize + 1.5f) && myCamera.Position.z >(-mazeSize + 0.5f)){
					lastTimer = timer;
					myGameManage.bgMusic.pausePlay();
					myGameManage.winSound.play();
					playWinSound = true;
					playBgMusic = false;

					myCamera.moveLock = true;
					winState = true;

				}
				//播放9秒的胜利音效
				if (playWinSound == true)
				{
					if ((timer - lastTimer) > 9.0f){
						myGameManage.gameUI_state = WIN_UI;
						myGameManage.lastFirstUI_choice = myGameManage.firstUI_choice;

						myCamera.moveLock = true; 
						returnGame = false;
						playWinSound = false;
						winState = false;
					}


				}
				//播放5秒的失败音效
				if (playOverSound == true){
					if ((timer - lastTimer) > 6.0f){
						myGameManage.gameUI_state = OVER_UI;
						myGameManage.lastFirstUI_choice = myGameManage.firstUI_choice;

						if (thridPersonPerspective==false)
						    myCamera.moveLock = true;
						returnGame = false;
						playOverSound = false;
			
						dieState = false;
					}
				}

				if (second == 19) firstReduce = true;
				if (minute > 0 && second == 0 && firstReduce){
					timer = 0.0f;
					--minute;
					firstReduce = false;
				}

				//todo 绘制迷宫出口标志
				if (winState == false){
					glm::mat4 _model;
					lampShader.Use();
					GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
					GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
					GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
					glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
					glUniform3f(glGetUniformLocation(lampShader.Program, "myColor"), 1.0f, 0.0f, 0.0f);

					glBindVertexArray(CubePosVAO);
					if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
						_model = glm::rotate(_model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
						_model = glm::rotate(_model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
					}

					_model = glm::translate(_model, glm::vec3(-mazeSize + 1.0f, 0.0f, -mazeSize + 1.0f));
					_model = glm::rotate(_model, (GLfloat)glfwGetTime()*100.0f, glm::vec3(0.0f, 1.0f, 0.0f));
					_model = glm::scale(_model, glm::vec3(0.4f, 0.4f, 0.4f));
					modelLoc = glGetUniformLocation(lampShader.Program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(_model));

					glDrawArrays(GL_TRIANGLES, 0, 36);
					glBindVertexArray(0);

				}

				//todo 绘制玩家位置物体
				if (myCamera.moveLock == true&&thridPersonPerspective==true){
					glm::mat4 model;

					lampShader.Use();
					GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
					GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
					GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
					glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
					glUniform3f(glGetUniformLocation(lampShader.Program, "myColor"), 0.0f, 1.0f, 0.0f);
					glBindVertexArray(CubePosVAO);

					if (myCamera.moveLock == true && myMazeRotate.trackLock == false){
						model = glm::rotate(model, myMazeRotate.angleX, glm::vec3(1.0f, 0.0f, 0.0f));
						model = glm::rotate(model, myMazeRotate.angleY, glm::vec3(0.0f, 1.0f, 0.0f));
					}

					model = glm::translate(model, glm::vec3(myCamera.LastPosition.x, 0.0f, myCamera.LastPosition.z));
					model = glm::rotate(model, (GLfloat)glfwGetTime()*100.0f, glm::vec3(0.0f, 1.0f, 0.0f));
					model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
					modelLoc = glGetUniformLocation(lampShader.Program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

					glDrawArrays(GL_TRIANGLES, 0, 36);
					glBindVertexArray(0);

				}

			}
		
		}//if

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}
//产生一个纹理给帧缓存
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;


	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, WIDTH, HEIGHT, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}
//todo 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	
	if (key == GLFW_KEY_W&&action == GLFW_PRESS){
		keys[0] = true;
		myGameManage.footSound.startPlay();
	}
		
	else if (action == GLFW_RELEASE){
		keys[0] = false;
		myGameManage.footSound.pausePlay();
	}
		
	if (key == GLFW_KEY_S&&action == GLFW_PRESS)
		keys[1] = true;
	else if (action == GLFW_RELEASE)
		keys[1] = false;
	if (key == GLFW_KEY_A&&action == GLFW_PRESS)
		keys[2] = true;
	else if (action == GLFW_RELEASE)
		keys[2] = false;
	if (key == GLFW_KEY_D&&action == GLFW_PRESS)
		keys[3] = true;
	else if (action == GLFW_RELEASE)
		keys[3] = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		myCamera.moveLock = true;
		myGameManage.game_state = FIRST_UI;
		myGameManage.lastFirstUI_choice = DEFAULT;
		myGameManage.gameUI_state = UIDEFAULT;

		returnGame = false;               //将返回游戏的开关关闭
	}
	if (key == GLFW_KEY_F1&&action == GLFW_PRESS){
		myGameManage.gameUI_state = SECOND_UI;
		myGameManage.lastFirstUI_choice = myGameManage.firstUI_choice;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);    //将鼠标显示出来


		if (thridPersonPerspective==false)
		   myCamera.moveLock = true;
	}
	if (key==GLFW_KEY_F3&&action==GLFW_PRESS){
		//增加关卡内容
		gameLevel++;
		mazeSize += 3;
		cubeNumber += 5;
		minute += 2;

		resetMaze = true;
		returnGame = false;

		myCamera.moveLock = false;
		myCamera.Position = glm::vec3(mazeSize - 1.0f, 0.0f, mazeSize - 1.0f);
		myCamera.Lock_Y = true;
	}

	if (key == GLFW_KEY_SPACE&&action == GLFW_PRESS){

		//播放手电筒开关音效
		myGameManage.flashLightSound.play();
		if (openFlashlight == true)
			openFlashlight = false;
		else
		{
			openFlashlight = true;
		}
	}
	if (key == GLFW_KEY_F2&&action == GLFW_PRESS){
		myGameManage.gameUI_state = HIGHSCORE_UI;
		myCamera.moveLock = true;
	
	}
	if (key == GLFW_KEY_F2&&action == GLFW_RELEASE){
		myGameManage.gameUI_state = UIDEFAULT;
		if (thridPersonPerspective == false)
			myCamera.moveLock = false;
	}
	if (key == GLFW_KEY_F4&&action == GLFW_PRESS){
		playWinSound = true;
	}
}

//todo 鼠标按钮回调函数
void mouseButton_callback(GLFWwindow* window, int button, int action, int mode){

	if (action == GLFW_PRESS){
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			mouseLeftDown = true;
			//开始界面属性设置
			if (myGameManage.game_state == FIRST_UI){
				switch (myGameManage.firstUI_list)
				{
				case EASY:
					myGameManage.firstUI_choice = EASY;
					for (int i = 0; i < 3; ++i){
						if (i == 0) fontColor[i] = glm::vec3(0.7f, 0.0f, 0.0f);
						else fontColor[i] = glm::vec3(0.0f, 0.7f, 0.0f);
					}
					break;
				case _NORMAL:
					myGameManage.firstUI_choice = _NORMAL;
					for (int i = 0; i < 3; ++i){
						if (i == 1) fontColor[i] = glm::vec3(0.7f, 0.0f, 0.0f);
						else fontColor[i] = glm::vec3(0.0f, 0.7f, 0.0f);
					}
					break;
				case DIFFICULT:
					myGameManage.firstUI_choice = DIFFICULT;
					for (int i = 0; i < 3; ++i){
						if (i == 2) fontColor[i] = glm::vec3(0.7f, 0.0f, 0.0f);
						else fontColor[i] = glm::vec3(0.0f, 0.7f, 0.0f);
					}
					break;
				default:
					break;
				}

			
			}//if

			//游戏设置界面的相关属性设置
			else if (myGameManage.game_state == SECOND_UI||myGameManage.gameUI_state==SECOND_UI){
				switch (myGameManage.secondUI_list)
				{
				  case MUSIC_ON:
					  myGameManage.musicChoice = MUSIC_ON;
					  break;
				  case MUSIC_OFF:
					  myGameManage.musicChoice = MUSIC_OFF;
					  break;
				  case SOUND_ON:
					  myGameManage.soundChoice = SOUND_ON;
					  break;
				  case SOUND_OFF:
					  myGameManage.soundChoice = SOUND_OFF;
					  break;
				  case LOW:
					  myGameManage.volumeChoice = LOW;
					  myGameManage.bgMusic.setVolume(0.2f);
					  break;
				  case __NORMAL:
					  myGameManage.volumeChoice = __NORMAL;
					  myGameManage.bgMusic.setVolume(0.5f);
					  break;
				  case HIGH:
					  myGameManage.volumeChoice = HIGH;
					  myGameManage.bgMusic.setVolume(1.0f);
					  break;
				  default:
					break;
				}
				

			}//if

			else if (myGameManage.gameUI_state == OVER_UI){
				if (myGameManage.gameOverUI_list == TRY_AGAIN){
					myGameManage.gameOverUI_choice = TRY_AGAIN;
				}
				else if (myGameManage.gameOverUI_list == RETURN){
					myGameManage.gameOverUI_choice = RETURN;
				}

			}
			else if (myGameManage.gameUI_state == WIN_UI){
				if (myGameManage.gameWinUI_list == PLAY_AGAIN){
					myGameManage.gameWinUI_choice = PLAY_AGAIN;
				}
				else if (myGameManage.gameWinUI_list == NEXTLEVEL){
					myGameManage.gameWinUI_choice = NEXTLEVEL;
				}

			}
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			break;
		default:
			break;
		}
		                      
	}
	else if (action == GLFW_RELEASE){
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			mouseLeftDown = false;
			firstMouseDown = true;

			if (myGameManage.game_state == FIRST_UI){
				if (myGameManage.firstUI_choice == EASY&&myGameManage.firstUI_list == START)
				{
					myGameManage.game_state = PLAYING;
					myCamera.moveLock = false;                 //解锁玩家移动视角
					myCamera.Lock_Y = true;                   //限制玩家只能在y=0的平面上移动
			
                    //重置关卡内容
					resetMaze = true;                 //开启游戏关卡重置功能
					returnGame = false;
					thridPersonPerspective = false;

					gameLevel = 1;      //关卡
					mazeSize = 10;      //迷宫大小

					//将字体属性还原
					fontSize[3] = SMALLFONT;                            
					fontColor[0] = glm::vec3(0.0f, 0.7f, 0.0f);
					myGameManage.mouseButtonDownSound.play();           //播放按键声音

				}
				if (myGameManage.firstUI_choice == DEFAULT&&myGameManage.firstUI_list == START)
				{
					myGameManage.errorWaring.play();            //播放错误提示声音
				}
				if (myGameManage.firstUI_choice == _NORMAL&&myGameManage.firstUI_list == START)
				{
					myGameManage.game_state = PLAYING;
					myCamera.moveLock = false;
					myCamera.Lock_Y = true;

				
					//重置关卡内容
					resetMaze = true;                 //开启游戏关卡重置功能
					returnGame = false;
					thridPersonPerspective = false;

					gameLevel = 1;      //关卡
					mazeSize = 10;      //迷宫大小

					//将字体属性还原
					fontSize[3] = SMALLFONT;  
					fontColor[1] = glm::vec3(0.0f, 0.7f, 0.0f);
					myGameManage.mouseButtonDownSound.play();           //播放按键声音

				}
				if (myGameManage.firstUI_choice == DIFFICULT&&myGameManage.firstUI_list == START)
				{
					myGameManage.game_state = PLAYING;
					myCamera.moveLock = false;
					myCamera.Lock_Y = true;
					thridPersonPerspective = false;

					//重置关卡内容
					resetMaze = true;                 //开启游戏关卡重置功能
					returnGame = false;

					gameLevel = 1;      //关卡
					mazeSize = 10;      //迷宫大小

					//将字体属性还原
					fontSize[3] = SMALLFONT;                            
					fontColor[2] = glm::vec3(0.0f, 0.7f, 0.0f);
					myGameManage.mouseButtonDownSound.play();           //播放按键声音

				}
				if (myGameManage.firstUI_list == EXIT)                  //退出游戏
				{
					myGameManage.mouseButtonDownSound.play();           //播放按键声音
					glfwSetWindowShouldClose(window, GL_TRUE);
				}
					
				if (myGameManage.firstUI_list == SETTING){

					fontSize[4] = SMALLFONT;                            //将字体大小还原
					myGameManage.mouseButtonDownSound.play();           //播放按键声音
					myGameManage.game_state = SECOND_UI;               //跳转到游戏设置界面

				}
			
			}
			else if (myGameManage.game_state == SECOND_UI||myGameManage.gameUI_state==SECOND_UI){
				if (myGameManage.secondUI_list == BACK){
					myGameManage.secondUI_list = __DEFAULT;

					fontSize[13] = SMALLFONT;                            //将字体大小还原
					myGameManage.mouseButtonDownSound.play();      //播放按键声音

					myGameManage.game_state = FIRST_UI;                //返回游戏开始界面
				}
				if (myGameManage.secondUI_list == RETURN_GAME)         //返回游戏
				{
					returnGame = true;
					myGameManage.game_state = PLAYING;
					myGameManage.gameUI_state = UIDEFAULT;

					if (thridPersonPerspective==true)
					   myCamera.moveLock = true;
					else{
						myCamera.moveLock = false;
					}
					fontSize[14] = SMALLFONT;                            //将字体大小还原
					myGameManage.mouseButtonDownSound.play();            //播放按键声音
				}

				//改变选项字体颜色
				if (myGameManage.musicChoice == MUSIC_ON){
					fontColor[6] = glm::vec3(0.7f, 0.0f, 0.0f);
					fontColor[7] = glm::vec3(0.0f, 0.7f, 0.0f);

					myGameManage.playMusic = true;                   //播放背景音乐
				}
				else
				{
					fontColor[6] = glm::vec3(0.0f, 0.7f, 0.0f);
					fontColor[7] = glm::vec3(0.7f, 0.0f, 0.0f);

					myGameManage.playMusic = false;                  //停止播放背景音乐
				}

				if (myGameManage.soundChoice == SOUND_ON){
					fontColor[8] = glm::vec3(0.7f, 0.0f, 0.0f);
					fontColor[9] = glm::vec3(0.0f, 0.7f, 0.0f);

					myGameManage.playSound = true;                   //打开音效
				}
				else
				{
					fontColor[8] = glm::vec3(0.0f, 0.7f, 0.0f);
					fontColor[9] = glm::vec3(0.7f, 0.0f, 0.0f);

					myGameManage.playSound = false;                   //关闭音效
				}
				if (myGameManage.volumeChoice == LOW){
					fontColor[11] = glm::vec3(0.7f, 0.0f, 0.0f);
					fontColor[10] = glm::vec3(0.0f, 0.7f, 0.0f);
					fontColor[12] = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else if (myGameManage.volumeChoice == __NORMAL){
					fontColor[11] = glm::vec3(0.0f, 0.7f, 0.0f);
					fontColor[10] = glm::vec3(0.7f, 0.0f, 0.0f);
					fontColor[12] = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else
				{
					fontColor[11] = glm::vec3(0.0f, 0.7f, 0.0f);
					fontColor[10] = glm::vec3(0.0f, 0.7f, 0.0f);
					fontColor[12] = glm::vec3(0.7f, 0.0f, 0.0f);
				}

			}//else if

			else if (myGameManage.gameUI_state == OVER_UI){
				if (myGameManage.gameOverUI_choice == TRY_AGAIN){

					myGameManage.game_state = PLAYING;
					myGameManage.firstUI_choice = myGameManage.lastFirstUI_choice;
					myGameManage.gameUI_state = UIDEFAULT;

					myCamera.moveLock = false;
					myCamera.Lock_Y = true;
					resetMaze = true;

					fontSize[15] = SMALLFONT;                            //将字体大小还原
					myGameManage.mouseButtonDownSound.play();            //播放按键声音

					playBgMusic = true;                                   //重新开始播放背景音乐
				}
				else if (myGameManage.gameOverUI_choice == RETURN){

					fontSize[16] = SMALLFONT;                            //将字体大小还原
					myGameManage.mouseButtonDownSound.play();            //播放按键声音
					myGameManage.game_state = FIRST_UI;                  //返回游戏开始界面
					myGameManage.lastFirstUI_choice = DEFAULT;
					myGameManage.gameUI_state = UIDEFAULT;

					//重新开始播放背景音乐
					playBgMusic = true;                                  
				}
			}
			else if (myGameManage.gameUI_state == WIN_UI){
				if (myGameManage.gameWinUI_choice == PLAY_AGAIN){

					myGameManage.game_state = PLAYING;
					myGameManage.firstUI_choice = myGameManage.lastFirstUI_choice;
					myGameManage.gameUI_state = UIDEFAULT;

					myCamera.moveLock = false;
					myCamera.Lock_Y = true;
					resetMaze = true;

					fontSize[17] = SMALLFONT;                            //将字体大小还原
					myGameManage.mouseButtonDownSound.play();            //播放按键声音

					playBgMusic = true;                                   //重新开始播放背景音乐

				}
				else if (myGameManage.gameWinUI_choice == NEXTLEVEL){


					//增加关卡内容
					gameLevel++;
					mazeSize += 3;
					cubeNumber += 5;
					minute += 2;

					resetMaze = true;
					myGameManage.game_state = PLAYING;
					myGameManage.firstUI_choice = myGameManage.lastFirstUI_choice;
					myGameManage.gameUI_state = UIDEFAULT;

					myCamera.moveLock = false;
					myCamera.Position = glm::vec3(mazeSize - 1.0f, 0.0f, mazeSize - 1.0f);
					myCamera.Lock_Y = true;

					fontSize[18] = SMALLFONT;                            //将字体大小还原
					myGameManage.mouseButtonDownSound.play();            //播放按键声音

					playBgMusic = true;                                   //重新开始播放背景音乐
				}
			}
			      
			std::cout << "release left" << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			if (myGameManage.game_state == PLAYING){
				if (myCamera.moveLock == true){
					//返回正常视角
					myMazeRotate.angleX = 0.0f;
					myMazeRotate.angleY = 0.0f;
					myCamera.moveLock = false;                                     //解锁视角
					myCamera.Lock_Y = true;                                       //解除视角对Y轴的锁定

					thridPersonPerspective = false;                                //标识是否切换到第三人称视角

					//将视角重新移入迷宫
					myCamera.Position = myCamera.LastPosition;
					myCamera.Front = myCamera.LastFront;
					myCamera.Up = myCamera.LastUp;
					myCamera.Right = myCamera.LastRight;

					myCamera.Zoom = 45;

					std::cout << "Press right" << std::endl;
				}
				else
				{
					thridPersonPerspective = true;      //标识是否切换到第三人称视角
					myCamera.moveLock = true;           //锁定视角
					myCamera.Lock_Y = false;            //解除视角对Y轴的锁定

					//将视角移出迷宫
					myCamera.Position = glm::vec3(0.0f, 10.0f, -30.0f);
					myCamera.Front = glm::vec3(-0.017f, -0.167f, 0.596f);
					myCamera.Right = glm::vec3(1.0f, 0.0f, -0.008f);
					myCamera.Up = glm::vec3(0.0f, 1.0f, 0.0f);
				}
			}
			
			std::cout << "Release right" << std::endl;
			break;
		default:
			break;
		}
	}

}
void CameraMove(){

	if (keys[0]&&myCamera.moveLock==false)
		myCamera.ProcessKeyboard(FORWARD, deltaTime);

	if (keys[1]&&myCamera.moveLock == false)
		myCamera.ProcessKeyboard(BACKWARD, deltaTime);

	if (keys[2]&&myCamera.moveLock == false)
		myCamera.ProcessKeyboard(LEFT, deltaTime);

	if (keys[3]&&myCamera.moveLock == false)
		myCamera.ProcessKeyboard(RIGHT, deltaTime);
	

}
//todo 鼠标滑动回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos){

	
	if (myCamera.moveLock == false){
		if (firstMouse){
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		GLdouble xoffset = xpos - lastX;
		GLdouble yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		myCamera.ProcessMouseMovement(xoffset, yoffset);
		//保留移出视角之前的摄像机视角属性
		myCamera.LastFront = myCamera.Front;
		myCamera.LastRight = myCamera.Right;
		myCamera.LastUp = myCamera.Up;
	}
	else
	{
		myMazeRotate.trackLock = false;
	}


	//通过滑动鼠标来控制迷宫旋转
	if (mouseLeftDown){
		if (firstMouseDown==true){
			myMazeRotate.lastX = xpos;
			myMazeRotate.lastY = ypos;
			firstMouseDown = false;
			playRotateSound = true;
		}
		myMazeRotate.setRotation(xpos, ypos, WIDTH, HEIGHT, deltaTime);
		//获取起始角度
		if (startRotate == true){
			lastAngleX = myMazeRotate.angleX;
			lastAngleY = myMazeRotate.angleY;
			startRotate = false;
		}

		//todo 播放迷宫旋转音效
		if (playRotateSound==true && (myMazeRotate.angleX - lastAngleX > 10.0f || myMazeRotate.angleY - lastAngleY > 10.0f)){
			myGameManage.rotateSound.play();
			playRotateSound = false;
		}
		
	}
#pragma region 当游戏处于开始界面时
	if (myGameManage.game_state == FIRST_UI){

		//鼠标热区的设定
		if (xpos > 355.0f&&xpos<455.0f&&ypos > 230.0f&&ypos<260.0f) {
			myGameManage.firstUI_list = EASY;
		}
		else if (xpos > 355.0f&&xpos<435.0f&&ypos > 285.0f&&ypos<315.0f) {
			myGameManage.firstUI_list = _NORMAL;
		}
		else if (xpos > 355.0f&&xpos<435.0f&&ypos > 335.0f&&ypos<365.0f) {
			myGameManage.firstUI_list = DIFFICULT;
		}
		else if (xpos > 265.0f&&xpos<320.0f&&ypos > 395.0f&&ypos<435.0f) {
			myGameManage.firstUI_list = START;
		}
		else if (xpos > 355.0f&&xpos<433.0f&&ypos > 395.0f&&ypos<435.0f) {
			myGameManage.firstUI_list = SETTING;
		}
		else if (xpos > 466.0f&&xpos<540.0f&&ypos > 395.0f&&ypos<435.0f) {
			myGameManage.firstUI_list = EXIT;
		}
		else
		{
			myGameManage.cursorStyle = NORMAL;
			moveEnter = true;
			for (int i = 0; i < 6; ++i)
				fontSize[i] = SMALLFONT;
			myGameManage.firstUI_list = DEFAULT;
		}

		switch (myGameManage.firstUI_list)
		{
		  case EASY:
			  //设置鼠标成小手形状
			  myGameManage.cursorStyle = HAND;
			  //使音效执行一次
			  if (moveEnter == true)
				  startPlay = true;
			  fontSize[0] = BIGFONT;
			  break;
		  case _NORMAL:
			  myGameManage.cursorStyle = HAND;
			  if (moveEnter == true)
				  startPlay = true;
			  fontSize[1] = BIGFONT;
			  break;
		  case DIFFICULT:
			  myGameManage.cursorStyle = HAND;
			  if (moveEnter == true)
				  startPlay = true;
			  fontSize[2] = BIGFONT;
			  break;
		  case START:
			  myGameManage.cursorStyle = HAND;
			  if (moveEnter == true)
				  startPlay = true;
			  fontSize[3] = BIGFONT;
			  break;
		  case SETTING:
			  myGameManage.cursorStyle = HAND;
			  if (moveEnter == true)
				  startPlay = true;
			  fontSize[4] = BIGFONT;
			  break;
		  case EXIT:
			  myGameManage.cursorStyle = HAND;
			  if (moveEnter == true)
				  startPlay = true;
			  fontSize[5] = BIGFONT;
			  break;
		default:
			break;
		}
	}
#pragma endregion

#pragma region 当游戏处于设置界面时
	else if (myGameManage.gameUI_state == SECOND_UI || myGameManage.game_state == SECOND_UI){
		//鼠标热区的设定
		if (xpos > 390.0f&&xpos<412.0f&&ypos > 192.0f&&ypos<206.0f) {
			myGameManage.secondUI_list = MUSIC_ON;
		}
		else if (xpos > 440.0f&&xpos<465.0f&&ypos > 192.0f&&ypos<206.0f) {
			myGameManage.secondUI_list = MUSIC_OFF;
		}
		else if (xpos > 390.0f&&xpos<412.0f&&ypos > 231.0f&&ypos<245.0f) {
			myGameManage.secondUI_list = SOUND_ON;
		}
		else if (xpos > 440.0f&&xpos<465.0f&&ypos > 231.0f&&ypos<245.0f) {
			myGameManage.secondUI_list = SOUND_OFF;
		}
		else if (xpos > 390.0f&&xpos<422.0f&&ypos > 268.0f&&ypos<282.0f) {
			myGameManage.secondUI_list = LOW;
		}
		else if (xpos > 450.0f&&xpos<540.0f&&ypos > 268.0f&&ypos<282.0f) {
			myGameManage.secondUI_list = __NORMAL;
		}
		else if (xpos > 570.0f&&xpos<630.0f&&ypos > 268.0f&&ypos<282.0f) {
			myGameManage.secondUI_list = HIGH;
		}
		else if (myGameManage.lastFirstUI_choice==DEFAULT&&xpos > 365.0f&&xpos<440.0f&&ypos > 370.0f&&ypos<395.0f) {
			myGameManage.secondUI_list = BACK;
		}
		else if (myGameManage.lastFirstUI_choice != DEFAULT&&xpos > 360.0f&&xpos<490.0f&&ypos > 370.0f&&ypos<395.0f) {
			myGameManage.secondUI_list = RETURN_GAME;
		}
		else
		{
			myGameManage.cursorStyle = NORMAL;
			moveEnter = true;
			for (int i = 6; i < 15; ++i)
				fontSize[i] = SMALLFONT;
			myGameManage.secondUI_list = __DEFAULT;
		}

		switch (myGameManage.secondUI_list)
		{
		case MUSIC_ON:
			//设置鼠标成小手形状
			myGameManage.cursorStyle = HAND;
			//使音效执行一次
			if (moveEnter == true)
				startPlay = true;
			fontSize[6] = BIGFONT;
			break;
		case MUSIC_OFF:
			myGameManage.cursorStyle = HAND;
			if (moveEnter == true)
				startPlay = true;
			fontSize[7] = BIGFONT;
			break;
		case SOUND_ON:
			myGameManage.cursorStyle = HAND;
			if (moveEnter == true)
				startPlay = true;
			fontSize[8] = BIGFONT;
			break;
		case SOUND_OFF:
			myGameManage.cursorStyle = HAND;
			if (moveEnter == true)
				startPlay = true;
			fontSize[9] = BIGFONT;
			break;
		case LOW:
			myGameManage.cursorStyle = HAND;
			if (moveEnter == true)
				startPlay = true;
			fontSize[11] = BIGFONT;
			break;
		case __NORMAL:
			myGameManage.cursorStyle = HAND;
			if (moveEnter == true)
				startPlay = true;
			fontSize[10] = BIGFONT;
			break;
		case HIGH:
			myGameManage.cursorStyle = HAND;
			if (moveEnter == true)
				startPlay = true;
			fontSize[12] = BIGFONT;
			break;
		case BACK:
			if (myGameManage.lastFirstUI_choice == DEFAULT){
				myGameManage.cursorStyle = HAND;
				if (moveEnter == true)
					startPlay = true;
				fontSize[13] = BIGFONT;
				
			}
			break;
		case RETURN_GAME:
			if (myGameManage.lastFirstUI_choice != DEFAULT){
				myGameManage.cursorStyle = HAND;
				if (moveEnter == true)
					startPlay = true;
				fontSize[14] = BIGFONT;
			}
			break;
		default:
			break;
		}
	}//if
#pragma endregion

#pragma region 当游戏处于结束界面时
	else if (myGameManage.gameUI_state == OVER_UI){
		//鼠标热区的设定
		if (xpos > 280.0f&&xpos<370.0f&&ypos > 229.0f&&ypos<245.0f) {
			myGameManage.gameOverUI_list = TRY_AGAIN;
		}
		else if (xpos > 450.0f&&xpos<576.0f&&ypos > 229.0f&&ypos<245.0f) {
			myGameManage.gameOverUI_list = RETURN;
		}
		else
		{
			myGameManage.cursorStyle = NORMAL;
			moveEnter = true;
			fontSize[15] = SMALLFONT;
			fontSize[16] = SMALLFONT;
			myGameManage.gameOverUI_list = DEFAULT_;
		}

		switch (myGameManage.gameOverUI_list)
		{
		case TRY_AGAIN:
			myGameManage.cursorStyle = HAND;
			if (moveEnter == true)
				startPlay = true;
			fontSize[15] = BIGFONT;
			myGameManage.gameOverUI_choice = TRY_AGAIN;
			break;
		case RETURN:
			myGameManage.cursorStyle = HAND;
			if (moveEnter == true)
				startPlay = true;
			fontSize[16] = BIGFONT;
			break;
		default:
			break;
		}
	
	}

#pragma endregion

#pragma region 当游戏处于过关时
	else if (myGameManage.gameUI_state == WIN_UI){
		//鼠标热区的设定
		if (xpos > 280.0f&&xpos<370.0f&&ypos > 229.0f&&ypos<245.0f) {
			myGameManage.gameWinUI_list = PLAY_AGAIN;
		}
		else if (xpos > 450.0f&&xpos<576.0f&&ypos > 229.0f&&ypos<245.0f) {
			myGameManage.gameWinUI_list = NEXTLEVEL;
		}
		else
		{
			myGameManage.cursorStyle = NORMAL;
			moveEnter = true;
			fontSize[17] = SMALLFONT;
			fontSize[18] = SMALLFONT;
			myGameManage.gameWinUI_list = DEFAULT__;
		}

		switch (myGameManage.gameWinUI_list)
		{
		case PLAY_AGAIN:
			myGameManage.cursorStyle = HAND;
			if (moveEnter == true)
				startPlay = true;
			fontSize[17] = BIGFONT;
			break;
		case NEXTLEVEL:
			myGameManage.cursorStyle = HAND;
			if (moveEnter == true)
				startPlay = true;
			fontSize[18] = BIGFONT;
			break;
		default:
			break;
		}

	}
#pragma endregion

  if (myGameManage.game_state == PLAYING&&myGameManage.gameUI_state==UIDEFAULT)
		myGameManage.cursorStyle = CUSTOM;
	
}
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset){

	myCamera.ProcessMouseScroll(yOffset);
}
