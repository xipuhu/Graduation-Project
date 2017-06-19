#include"MazePrim.h"
#include <stdio.h>  
#include <time.h>  
#include <stdlib.h>  
#include <iostream>

#pragma warning(disable : 4305)
#pragma warning(disable : 4244)

Maze_Prim::Maze_Prim() :nodeRow(0), mazeRow(0){

	arr_nodeRow = NULL;
	arr_mazeRow = NULL;

	srand((int)time(0));
	rand();
}
void Maze_Prim::initArrayMaze(int rowNum){

	nodeRow = rowNum;
	mazeRow = 2 * rowNum + 1;

	arr_nodeRow = new Point_prim*[nodeRow];
	for (int i = 0; i < nodeRow; ++i){
		arr_nodeRow[i] = new Point_prim[nodeRow];
		for (int j = 0; j < nodeRow; ++j){
			arr_nodeRow[i][j].setCoords(i, j);
		}
	}

	arr_mazeRow = new Point_prim*[mazeRow];
	for (int i = 0; i < mazeRow; ++i){
		arr_mazeRow[i] = new Point_prim[mazeRow];
		for (int j = 0; j < mazeRow; ++j){
			arr_mazeRow[i][j].setCoords(i, j);
		}
	}

}

bool Maze_Prim::wallIsValid(int _x, int _y){
	return _x>0 && _x<mazeRow  && _y>0 && _y < mazeRow && arr_mazeRow[_x][_y].state==Point_prim::State_wall;
}

void Maze_Prim::addWall(int _x, int _y){
	int mazeX = 2 * _x + 1;
	int mazeY = 2 * _y + 1;

	//添加上墙壁
	if (wallIsValid(mazeX, mazeY - 1)){
		mWall.push_back(arr_mazeRow[mazeX][mazeY - 1]);
	}
	//添加下墙壁
	if (wallIsValid(mazeX, mazeY + 1)){
		mWall.push_back(arr_mazeRow[mazeX][mazeY + 1]);
	}
	//添加左墙壁
	if (wallIsValid(mazeX - 1, mazeY)){
		mWall.push_back(arr_mazeRow[mazeX - 1][mazeY]);
	}
	//添加右墙壁
	if (wallIsValid(mazeX + 1, mazeY)){
		mWall.push_back(arr_mazeRow[mazeX + 1][mazeY]);
	}

}
void Maze_Prim::delWallFromVec(int index){
	for (unsigned int i = index; i < mWall.size()-1; ++i)
		mWall[i] = mWall[i + 1];

	mWall.pop_back();
}
void Maze_Prim::generateMaze(){
	int cellX, cellY;          //结点坐标
	int randNum;
	//初始选择一个结点
	cellX = cellY = 0;
	Point_prim currWall;

	arr_mazeRow[2 * cellX + 1][2 * cellY + 1].state = Point_prim::State_path;
	arr_nodeRow[cellX][cellY].setVisited();

	addWall(cellX, cellY);

	while (!mWall.empty()){

		randNum = random(0, mWall.size() - 1);     //从列表中选出一个随机墙
		currWall = mWall[randNum];

		if (currWall.x > 0 && currWall.x < mazeRow - 1 && currWall.y > 0 && currWall.y < mazeRow - 1){
			//墙的上下方向
			if ((arr_nodeRow[(currWall.x - 1) / 2][(currWall.y - 1 - 1) / 2].isVisited && !arr_nodeRow[(currWall.x - 1) / 2][(currWall.y + 1 - 1) / 2].isVisited)){

				cellX = (currWall.x - 1) / 2;
				cellY = (currWall.y + 1 - 1) / 2;

				arr_mazeRow[2 * cellX + 1][2 * cellY + 1].state = Point_prim::State_path;
				arr_nodeRow[cellX][cellY].setVisited();

				arr_mazeRow[currWall.x][currWall.y].state = Point_prim::State_path;         //将墙拆除

				addWall(cellX, cellY);
			}

			else if ((!arr_nodeRow[(currWall.x - 1) / 2][(currWall.y - 1 - 1) / 2].isVisited && arr_nodeRow[(currWall.x - 1) / 2][(currWall.y + 1 - 1) / 2].isVisited)){

				cellX = (currWall.x - 1) / 2;
				cellY = (currWall.y - 1 - 1) / 2;

				arr_mazeRow[2 * cellX + 1][2 * cellY + 1].state = Point_prim::State_path;
				arr_nodeRow[cellX][cellY].setVisited();

				arr_mazeRow[currWall.x][currWall.y].state = Point_prim::State_path;         //将墙拆除

				addWall(cellX, cellY);
			}
			else if ((!arr_nodeRow[(currWall.x - 1) / 2][(currWall.y - 1 - 1) / 2].isVisited && !arr_nodeRow[(currWall.x - 1) / 2][(currWall.y + 1 - 1) / 2].isVisited)){

				cellX = (currWall.x - 1) / 2;
				cellY = (currWall.y - 1 - 1) / 2;

				arr_mazeRow[2 * cellX + 1][2 * cellY + 1].state = Point_prim::State_path;
				arr_nodeRow[cellX][cellY].setVisited();

				arr_mazeRow[currWall.x][currWall.y].state = Point_prim::State_path;         //将墙拆除

				addWall(cellX, cellY);
			}
			//墙的左右方向
			if ((arr_nodeRow[(currWall.x + 1 - 1) / 2][(currWall.y - 1) / 2].isVisited && !arr_nodeRow[(currWall.x - 1 - 1) / 2][(currWall.y - 1) / 2].isVisited)){

				cellX = (currWall.x - 1 - 1) / 2;
				cellY = (currWall.y - 1) / 2;

				arr_mazeRow[2 * cellX + 1][2 * cellY + 1].state = Point_prim::State_path;
				arr_nodeRow[cellX][cellY].setVisited();

				arr_mazeRow[currWall.x][currWall.y].state = Point_prim::State_path;         //将墙拆除

				addWall(cellX, cellY);
			}
			else if ((!arr_nodeRow[(currWall.x + 1 - 1) / 2][(currWall.y - 1) / 2].isVisited && arr_nodeRow[(currWall.x - 1 - 1) / 2][(currWall.y - 1) / 2].isVisited)){

				cellX = (currWall.x + 1 - 1) / 2;
				cellY = (currWall.y - 1) / 2;

				arr_mazeRow[2 * cellX + 1][2 * cellY + 1].state = Point_prim::State_path;
				arr_nodeRow[cellX][cellY].setVisited();

				arr_mazeRow[currWall.x][currWall.y].state = Point_prim::State_path;         //将墙拆除

				addWall(cellX, cellY);
			}
			else if ((!arr_nodeRow[(currWall.x + 1 - 1) / 2][(currWall.y - 1) / 2].isVisited && !arr_nodeRow[(currWall.x - 1 - 1) / 2][(currWall.y - 1) / 2].isVisited)){

				cellX = (currWall.x + 1 - 1) / 2;
				cellY = (currWall.y - 1) / 2;

				arr_mazeRow[2 * cellX + 1][2 * cellY + 1].state = Point_prim::State_path;
				arr_nodeRow[cellX][cellY].setVisited();

				arr_mazeRow[currWall.x][currWall.y].state = Point_prim::State_path;         //将墙拆除

				addWall(cellX, cellY);
			}

		}
	
		delWallFromVec(randNum);             //将墙移出列表
	}
 

}

double Maze_Prim::random(double start, double end){
	return start + (end - start)*rand() / (RAND_MAX + 1.0);
}
void Maze_Prim::printMazeArray(){
	for (int i = 0; i < nodeRow; ++i){
		for (int j = 0; j < nodeRow; ++j)
			std::cout << arr_nodeRow[i][j].state << " ";
		std::cout << std::endl;
	}
	for (int i = 0; i < mazeRow; ++i){
		for (int j = 0; j < mazeRow; ++j)
			std::cout << arr_mazeRow[i][j].state << " ";
		std::cout << std::endl;
	}
}
Maze_Prim::~Maze_Prim(){


}