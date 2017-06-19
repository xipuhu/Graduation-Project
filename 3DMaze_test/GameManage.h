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

//��Χ��
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
	UIDEFAULT   //Ĭ����
};

enum cursorStyles
{
	NORMAL,
	HAND,
	CUSTOM
};
enum firstUI
{
	DEFAULT,   //Ĭ����
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

	//��Ϸ״̬
	gameStates game_state;
	gameStates gameUI_state;

	//ÿ��ģʽ����Ϸ��¼
	int highScore[3];

	//��һʱ�̵���Ϸѡ��״̬
	firstUI lastFirstUI_choice;
    //�����
	cursorStyles cursorStyle;
	//��ʼ����
	firstUI firstUI_list;
	firstUI firstUI_choice;
	//���ý���
	secondUI secondUI_list;
	secondUI musicChoice, soundChoice, volumeChoice;
	//��Ϸʧ�ܽ���
	gameOverUI gameOverUI_list;
	gameOverUI gameOverUI_choice;
	//��Ϸ���ؽ���
	gameWinUI gameWinUI_list;
	gameWinUI gameWinUI_choice;

	//��Ϸ��Ч����
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

	//��ײ��⺯��
	bool  CheckCollision_Point_AABB(glm::vec3 cameraPosition);
	bool  CheckCollision_Cube_AABB(glm::vec3 cubePosition);

	//������ײ��
	void  setCollisionBoxs();
	void  setCollisionBoxs_cube();
	
	//�����Թ��������
	void  setArrMazePointer(GLint size, Maze_DepthFirst* p1, Maze_Prim* p2);

	double random(double start, double end);  //���������
    
	//������Ϸÿ��ģʽ�ļ�¼
	void reedHighScore(const GLchar* path, int mode);
	//д����Ϸ�¼�¼
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

	srand((int)time);      //��ʱ�������ͬ����������ӣ�ʹÿ��ʱ�̵������������ͬ
	rand();

	pMaze1 = NULL;
	pMaze2 = NULL;

	//������Ƶ
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
		std::cout << "�Թ���С������Ԥ��ֵ��" << std::endl;
		return;
	}

	if (p1 != NULL){
		pMaze1 = p1;
	}
	else if (p2 != NULL) pMaze2 = p2;

}

void GameManage::setCollisionBoxs(){


	if (pMaze1 != NULL){

		std::cout << "���ð�Χ��" << std::endl;
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

	//��ȡ���ݵ��㷨������ײ����
	if (pMaze1 != NULL){
		std::cout << "����cube��Χ��" << std::endl;
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
		//����prim�㷨������ײ����
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

	bool collisionX, collisionZ;  //���ڼ��X���Z����ཻ���
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

	bool collisionX, collisionZ;  //���ڼ��X���Z����ཻ���
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
			scoreStream >> highScore[mode];                //���ı��еļ�¼��ֵ��int���͵����ڼ�¼�ؿ���¼�ı���

			std::cout << highScore[mode] << std::endl;
			highScoreFile.close();
		}
		else
		{
			std::cout << "not open!" << std::endl;
		}
		
	}
	catch (std::ifstream::failure e){
		std::cout << "��ȡ�����Ϸ�ؿ���¼ʧ�ܣ�" << std::endl;
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
		std::cout << "��ȡ�����Ϸ�ؿ���¼ʧ�ܣ�" << std::endl;
	}

}

#endif