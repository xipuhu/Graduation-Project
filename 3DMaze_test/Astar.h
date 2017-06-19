#ifndef _ASTAR_H
#define _START_H

#include <iostream>
#include <vector>
#include <queue>
#include <glm\glm.hpp>     //GLSL的数学库
#include <functional>
#include "MazePrim.h"
using namespace std;

struct Grid
{
	int x, y;
	int F, G, H;
	int state;         //用于标注是墙还是通路
	Grid* parent;
	int dir;           //记录方格移动邻格的方向

	Grid() :x(0), y(0), F(0), G(0), H(0),dir(-1),parent(nullptr)
	{
	   
	}
	//使优先队列按F的值由小到大来优先
	friend bool operator < (Grid a, Grid b){
		return a.F > b.F;
	}

};


class Astar
{
public:
	Astar();
	~Astar();

	//Manhattan(曼哈顿)估价函数
	int Heuristic(const Grid currGrid, const Grid goalGrid){
		return (abs(goalGrid.x - currGrid.x) + abs(goalGrid.y - currGrid.y));
	}
	bool existInList(vector<Grid> List, Grid grid);
	void removeGirdFromList(vector<Grid> List, Grid grid);
	void FindPath(Maze_Prim pMaze);    //A*算法寻找路径

	int moveDir[100];
	int dirNum;
private:
	priority_queue<Grid> openList; //用优先队列来表示开启列表
	vector<Grid> tempOpenList;  //用来遍历开启列表
	vector<Grid> closeList;    //关闭列表
};


#endif