#ifndef _GAMEMANAGE_H_
#define _GAMEMANAGE_H_


#include<string>
#include<fstream>
#include<sstream>
#include<iostream>


#include<GL\glew.h>
#include<glm\glm.hpp>
#include"Maze.h"
#include"MazeDepthFirst.h"
#include"MazePrim.h"
#include"Audios.h"


#define MAXROW 100

//包围盒
struct AABB
{
	glm::vec3 MinPoint;
	glm::vec3 MaxPoint;
	int state;

	AABB(){
		MinPoint = glm::vec3(0.0f);
		MaxPoint = glm::vec3(0.0f);
		state = 0;
	}
};

enum gameStates
{
	FIRST_UI,
	SECOND_UI,
	HIGHSCORE_UI,
	OVER_UI,
	WIN_UI,
	PLAYING,
	UIDEFAULT   //默认项
};

enum cursorStyles
{
	NORMAL,
	HAND,
	CUSTOM
};
enum firstUI
{
	DEFAULT,   //默认项
	EASY,
	_NORMAL,
	DIFFICULT,
	START,
	SETTING,
	EXIT
	
};
enum secondUI
{
	MUSIC_ON,
	MUSIC_OFF,
	SOUND_ON,
	SOUND_OFF,
	LOW,
	__NORMAL,
	HIGH,
	BACK,
	RETURN_GAME,
	__DEFAULT
};
enum gameOverUI
{
	TRY_AGAIN,
	RETURN,
	DEFAULT_
};
enum gameWinUI
{
	PLAY_AGAIN,
	NEXTLEVEL,
	DEFAULT__
};

class GameManage
{
public:

	//游戏状态
	gameStates game_state;
	gameStates gameUI_state;

	//每个模式的游戏纪录
	int highScore[3];

	//上一时刻的游戏选择状态
	firstUI lastFirstUI_choice;
    //鼠标风格
	cursorStyles cursorStyle;
	//开始界面
	firstUI firstUI_list;
	firstUI firstUI_choice;
	//设置界面
	secondUI secondUI_list;
	secondUI musicChoice, soundChoice, volumeChoice;
	//游戏失败界面
	gameOverUI gameOverUI_list;
	gameOverUI gameOverUI_choice;
	//游戏过关界面
	gameWinUI gameWinUI_list;
	gameWinUI gameWinUI_choice;

	//游戏音效对象
	Audio cursorMovetoSound;
	Audio mouseButtonDownSound;
	Audio footSound;  
	Audio bgMusic;
	Audio countDownSound;
	Audio winSound;
	Audio overSound;
	Audio rotateSound;
	Audio errorWaring;
	Audio hurtSound;
	Audio timeSound;
	Audio flashLightSound;

	bool playMusic, playSound;

	GameManage();

	//碰撞检测函数
	bool  CheckCollision_Point_AABB(glm::vec3 cameraPosition);
	bool  CheckCollision_Cube_AABB(glm::vec3 cubePosition);

	//设置碰撞器
	void  setCollisionBoxs();
	void  setCollisionBoxs_cube();
	
	//配置迷宫相关属性
	void  setArrMazePointer(GLint size, Maze_DepthFirst* p1, Maze_Prim* p2);

	double random(double start, double end);  //产生随机数
    
	//读入游戏每个模式的纪录
	void reedHighScore(const GLchar* path, int mode);
	//写入游戏新记录
	void writeHighScore(const GLchar* path, int gameLevel,int mode);

	~GameManage();

private:
	AABB boxs[MAXROW][MAXROW];
	AABB cubeBoxs[MAXROW][MAXROW];
	GLint Row, row;

	Maze_DepthFirst* pMaze1;
	Maze_Prim* pMaze2;
};

GameManage::GameManage()
{
	game_state = FIRST_UI;
	gameUI_state = UIDEFAULT;


	cursorStyle = NORMAL;

	lastFirstUI_choice = DEFAULT;
	firstUI_list = DEFAULT;
	firstUI_choice = DEFAULT;

	secondUI_list = __DEFAULT;
	musicChoice = MUSIC_ON;
	soundChoice = SOUND_ON;
	volumeChoice = __NORMAL;

	gameOverUI_list = DEFAULT_;
	gameOverUI_choice = DEFAULT_;

	gameWinUI_list = DEFAULT__;
	gameWinUI_choice = DEFAULT__;

	playMusic = true;
	playSound = true;

	srand((int)time);      //随时间产生不同的随机数种子，使每个时刻的随机数都不相同
	rand();

	pMaze1 = NULL;
	pMaze2 = NULL;

	//载入音频
	cursorMovetoSound.loadAudio("path/to/audios/moveEnter.ogg",false);
	mouseButtonDownSound.loadAudio("path/to/audios/btn01.ogg",false);
	footSound.loadAudio("path/to/audios/foot.ogg",true);
	countDownSound.loadAudio("path/to/audios/countDown.ogg",false);
	winSound.loadAudio("path/to/audios/win.ogg",false);
	overSound.loadAudio("path/to/audios/over.ogg", false);
	rotateSound.loadAudio("path/to/audios/rotate.ogg", false);
	errorWaring.loadAudio("path/to/audios/error.wav", false);
	hurtSound.loadAudio("path/to/audios/hurt.wav", false);
	timeSound.loadAudio("path/to/audios/timeSound.wav", false);
	flashLightSound.loadAudio("path/to/audios/flashLight.wav", false);

	bgMusic.loadAudio("path/to/audios/bg02.ogg",true);
	bgMusic.setVolume(0.50f);
}

GameManage::~GameManage()
{
	bgMusic.deleteSound();
}

void GameManage::setArrMazePointer(GLint size, Maze_DepthFirst* p1, Maze_Prim* p2){

	pMaze1 = NULL;
	pMaze2 = NULL;
	row = size;
	Row = 2 * size + 1;
	if (Row > MAXROW) {
		std::cout << "迷宫大小超过了预设值！" << std::endl;
		return;
	}

	if (p1 != NULL){
		pMaze1 = p1;
	}
	else if (p2 != NULL) pMaze2 = p2;

}

void GameManage::setCollisionBoxs(){


	if (pMaze1 != NULL){

		std::cout << "设置包围盒" << std::endl;
		for (int i = 0; i <Row; ++i){
			for (int j = 0; j < Row; ++j){
				if (pMaze1->arr_mazeRow[i][j].state == 1){
					boxs[i][j].MinPoint.x = j - 0.55f - row;
					boxs[i][j].MinPoint.z = i - 0.55f - row;
					boxs[i][j].MaxPoint.x = j + 0.55f - row;
					boxs[i][j].MaxPoint.z = i + 0.55f - row;

					boxs[i][j].state = 1;
				}
				else
				{
					boxs[i][j].MinPoint.x = 0.0f;
					boxs[i][j].MinPoint.z = 0.0f;
					boxs[i][j].MaxPoint.x = 0.0f;
					boxs[i][j].MaxPoint.z = 0.0f;
				}
			}
		}//for


	}
	else if (pMaze2!=NULL)
	{
	
		for (int i = 0; i <Row; ++i){
			for (int j = 0; j < Row; ++j){
				if (pMaze2->arr_mazeRow[i][j].state == 1){
					boxs[i][j].MinPoint.x = j - 0.55f - row;
					boxs[i][j].MinPoint.z = i - 0.55f - row;
					boxs[i][j].MaxPoint.x = j + 0.55f - row;
					boxs[i][j].MaxPoint.z = i + 0.55f - row;

					boxs[i][j].state = 1;
				}
				else
				{
					boxs[i][j].MinPoint.x = 0.0f;
					boxs[i][j].MinPoint.z = 0.0f;
					boxs[i][j].MaxPoint.x = 0.0f;
					boxs[i][j].MaxPoint.z = 0.0f;
				}
			}
		}//for
	}

}
void GameManage::setCollisionBoxs_cube(){

	//采取回溯的算法生成碰撞盒子
	if (pMaze1 != NULL){
		std::cout << "设置cube包围盒" << std::endl;
		for (int i = 0; i < Row; ++i){
			for (int j = 0; j < Row; ++j){
				if (pMaze1->arr_mazeRow[i][j].state==1){

					cubeBoxs[i][j].MinPoint.x = j - 0.75f - row;
					cubeBoxs[i][j].MinPoint.z = i - 0.75f - row;

					cubeBoxs[i][j].MaxPoint.x = j + 0.75f - row;
					cubeBoxs[i][j].MaxPoint.z = i + 0.75f - row;

					cubeBoxs[i][j].state = 1;
				}
				else
				{
					cubeBoxs[i][j].MaxPoint.x = 0.0f;
					cubeBoxs[i][j].MinPoint.z = 0.0f;

					cubeBoxs[i][j].MaxPoint.x = 0.0f;
					cubeBoxs[i][j].MaxPoint.z = 0.0f;
				}
			}
			
		}//for
		
	}
	else if (pMaze2!=NULL)
	{
		//采用prim算法生成碰撞盒子
		for (int i = 0; i < Row; ++i){
			for (int j = 0; j < Row; ++j){
				if (pMaze2->arr_mazeRow[i][j].state==1){

					cubeBoxs[i][j].MinPoint.x = j - 0.75f - row;
					cubeBoxs[i][j].MinPoint.z = i - 0.75f - row;

					cubeBoxs[i][j].MaxPoint.x = j + 0.75f - row;
					cubeBoxs[i][j].MaxPoint.z = i + 0.75f - row;
					
					cubeBoxs[i][j].state = 1;
				}
				else
				{
					cubeBoxs[i][j].MaxPoint.x = 0.0f;
					cubeBoxs[i][j].MinPoint.z = 0.0f;

					cubeBoxs[i][j].MaxPoint.x = 0.0f;
					cubeBoxs[i][j].MaxPoint.z = 0.0f;
				}
			}
		}//for

	}

}

bool GameManage::CheckCollision_Cube_AABB(glm::vec3 cubePosition){

	bool collisionX, collisionZ;  //用于检测X轴和Z轴的相交情况
	int i, j;
	for (i = 0; i < Row; ++i){
		for (j = 0; j < Row; ++j){
			if (cubeBoxs[i][j].state == 1){
				collisionX = cubePosition.x > cubeBoxs[i][j].MinPoint.x&&cubePosition.x < cubeBoxs[i][j].MaxPoint.x;
				collisionZ = cubePosition.z > cubeBoxs[i][j].MinPoint.z&&cubePosition.z < cubeBoxs[i][j].MaxPoint.z;
				if (collisionX&&collisionZ) return true;
			}
		}
	}
	return false;
}
bool GameManage::CheckCollision_Point_AABB(glm::vec3 cameraPosition){

	bool collisionX, collisionZ;  //用于检测X轴和Z轴的相交情况
	int i, j;
	for (i = 0; i < Row; ++i){
		for (j = 0; j < Row; ++j){
			if (boxs[i][j].state == 1){
				collisionX = cameraPosition.x >= boxs[i][j].MinPoint.x&&cameraPosition.x <= boxs[i][j].MaxPoint.x;
				collisionZ = cameraPosition.z >= boxs[i][j].MinPoint.z&&cameraPosition.z <= boxs[i][j].MaxPoint.z;
				if (collisionX&&collisionZ) return true;
			}
			
		}
	}
	return false;
}


double GameManage::random(double start, double end){

	return start + (end - start)*rand() / (RAND_MAX + 1.0);
}

void GameManage::reedHighScore(const GLchar* path, int mode){

	std::ifstream highScoreFile;
	highScoreFile.exceptions(std::ifstream::badbit);
	try{
		highScoreFile.open(path);

		if (highScoreFile.is_open()){
			std::stringstream scoreStream;
			scoreStream << highScoreFile.rdbuf();
			scoreStream >> highScore[mode];                //将文本中的记录赋值给int类型的用于记录关卡纪录的变量

			std::cout << highScore[mode] << std::endl;
			highScoreFile.close();
		}
		else
		{
			std::cout << "not open!" << std::endl;
		}
		
	}
	catch (std::ifstream::failure e){
		std::cout << "获取最高游戏关卡记录失败！" << std::endl;
	}

}
void GameManage::writeHighScore(const GLchar* path, int gameLevel,int mode){

	std::ofstream highScoreFile;
	highScoreFile.exceptions(std::ofstream::badbit);
	try{
		highScoreFile.open(path);

		if (highScoreFile.is_open()){
			std::stringstream scoreStream;
			scoreStream << gameLevel;
			
			highScoreFile << scoreStream.str();
			highScore[mode] = gameLevel;

			highScoreFile.close();
		}
		else
		{
			std::cout << "not open!" << std::endl;
		}

	}
	catch (std::ifstream::failure e){
		std::cout << "获取最高游戏关卡记录失败！" << std::endl;
	}

}

#endif