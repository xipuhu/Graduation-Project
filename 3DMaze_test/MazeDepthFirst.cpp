#include "MazeDepthFirst.h"
#include <stdio.h>  
#include <time.h>  
#include <stdlib.h>  
#include <iostream>

#pragma warning(disable : 4305)
#pragma warning(disable : 4244)

Maze_DepthFirst::Maze_DepthFirst() :nodeRow(0), mazeRow(0){

	arr_nodeRow = new Point* ();
	arr_mazeRow = new Point* ();
	 
	srand((int)time(0));
	rand();
	
}
void Maze_DepthFirst::initArrayMaze(int rowNum){
	
	nodeRow = rowNum;
	mazeRow = 2 * rowNum + 1;

	Point** temp1 = arr_nodeRow;
	Point** temp2 = arr_mazeRow;
	arr_nodeRow = new Point*[nodeRow];
	for (int i = 0; i < nodeRow; ++i){
		arr_nodeRow[i] = new Point[nodeRow];
		for (int j = 0; j < nodeRow; ++j){
			arr_nodeRow[i][j].setCoords(i, j);
		}
	}

	arr_mazeRow = new Point*[mazeRow];
	for (int i = 0; i < mazeRow; ++i){
		arr_mazeRow[i] = new Point[mazeRow];
		for (int j = 0; j < mazeRow; ++j){
			arr_mazeRow[i][j].setCoords(i, j);
		}
	}
	delete temp1;
	delete temp2;
}

//todo ��������Թ�����
void Maze_DepthFirst::generateMaze(){
	int curX, curY;
	int nextX, nextY;
	curX = curY = 0;

	arr_nodeRow[curX][curY].setVisited();
	//�����ʵĳ�ʼ�����뵽���ջ�У����ڻ���
	mPoint.push(arr_nodeRow[curX][curY]);

	while (existNotVisited()){
	    
		nextX = curX;
		nextY = curY;

		//���ѡ��һ��δ���ʵ��ڽ��
		if (getNeighbour(nextX, nextY)){
		    
		
			//����ǰ���ͻ�õ��ڽ�����Թ������е�ֵ��Ϊͨ·
			arr_mazeRow[2 * curX + 1][2 * curY + 1].state = Point::State_path;
			arr_mazeRow[2 * nextX + 1][2 * nextY + 1].state = Point::State_path;
	
			//����ǰ���ͻ�õ��ڽ��֮���ֵ��Ϊͨ·
			arr_mazeRow[(2 * curX + 1 + 2 * nextX + 1) / 2][(2 * curY + 1 + 2 * nextY + 1) / 2].state = Point::State_path;

			
			curX = nextX;
			curY = nextY;

			arr_nodeRow[curX][curY].setVisited();    //����ǰ���״̬����Ϊ�ѷ���
			//����ǰ�����ջ���ڻ���
			mPoint.push(arr_nodeRow[curX][curY]);

		
		}
		else if (mPoint.size()>1)
		{
			mPoint.pop();
			mPoint.top().getCoords(curX, curY);
		}
		else if (mPoint.size() == 1){
			mPoint.top().getCoords(curX, curY);
			mPoint.pop();
		}
		else
		{
			std::cout << "sdsd" << std::endl;
		}

	}

}
double Maze_DepthFirst::random(double start, double end){
	return start + (end - start)*rand() / (RAND_MAX + 1.0);
}

bool Maze_DepthFirst::existNotVisited(){
	for (int i = 0; i < nodeRow; ++i){
		for (int j = 0; j < nodeRow; ++j)
		if (arr_nodeRow[i][j].isVisited == false)
			  return true;
	}

	return false;
}

bool Maze_DepthFirst::getNeighbour(int &_x, int &_y){
	int neighbourX[Dir_Max];
	int neighbourY[Dir_Max];
	bool isValid[Dir_Max];

	//�Ͽ����ڽ��
	neighbourX[Dir_Up] = _x;
	neighbourY[Dir_Up] = _y - 1;
	if (_y-1>=0)
	isValid[Dir_Up] = (pointIsVaild(neighbourX[Dir_Up], neighbourY[Dir_Up])
		&& !arr_nodeRow[neighbourX[Dir_Up]][neighbourY[Dir_Up]].isVisited);
	else
	{
		isValid[Dir_Up] = false;
	}

	//�¿����ڽ��
	neighbourX[Dir_Down] = _x;
	neighbourY[Dir_Down] = _y + 1;
	if (_y+1<nodeRow)
	isValid[Dir_Down] = (pointIsVaild(neighbourX[Dir_Down], neighbourY[Dir_Down])
		&& !arr_nodeRow[neighbourX[Dir_Down]][neighbourY[Dir_Down]].isVisited);
	else
	{
		isValid[Dir_Down] = false;
	}

	//�ҿ����ڽ��
	neighbourX[Dir_Right] = _x + 1;
	neighbourY[Dir_Right] = _y;
	if (_x+1<nodeRow)
	isValid[Dir_Right] = (pointIsVaild(neighbourX[Dir_Right], neighbourY[Dir_Right])
		&& !arr_nodeRow[neighbourX[Dir_Right]][neighbourY[Dir_Right]].isVisited);
	else
	{
		isValid[Dir_Right] = false;
	}

	//������ڽ��
	neighbourX[Dir_Left] = _x - 1;
	neighbourY[Dir_Left] = _y;
	if (_x-1>=0)
	isValid[Dir_Left] = (pointIsVaild(neighbourX[Dir_Left], neighbourY[Dir_Left])
		&& !arr_nodeRow[neighbourX[Dir_Left]][neighbourY[Dir_Left]].isVisited);
	else
	{
		isValid[Dir_Left] = false;
	}


	bool existVaildDir = false;
	for (int i = 0; i < Dir_Max; ++i){
		if (isValid[i] == true) existVaildDir = true;           //���ڿ�ȡ�ڽ��
	}
	if (existVaildDir == false)  return false;                  

	int randDir = random(0, 4);
	while (!isValid[randDir])        //�����ȡ��Ч���ڽ��
	{
		randDir = random(0, 4);
	}

	_x = neighbourX[randDir];        
	_y = neighbourY[randDir];

	return true;
}

bool Maze_DepthFirst::pointIsVaild(int x, int y){
	x = 2 * x + 1;
	y = 2 * y + 1;
	return x > 0 && x <= mazeRow&&y > 0 && y <= mazeRow;
}


void Maze_DepthFirst::printMazeArray(){

	for (int i = 0; i < mazeRow; ++i){
		for (int j = 0; j < mazeRow; ++j)
			std::cout << arr_mazeRow[i][j].state << " ";
		std::cout << std::endl;
	}
}
Maze_DepthFirst::~Maze_DepthFirst(){
    
}