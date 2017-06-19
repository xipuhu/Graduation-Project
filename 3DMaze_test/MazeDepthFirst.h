#ifndef _DEPTHFIRST_H
#define _DEPTHFIRST_H

#include<stack>

struct Point
{
	int x;
	int y;
	bool isVisited;
	int state;

	Point(){
		x = 0;
		y = 0;  
		isVisited = false;
		state = State_wall;
	}

	Point(int _x, int _y) :x(_x), y(_y){}

	void setCoords(int _x, int _y){
		x = _x;
		y = _y;
	}

	void setVisited(){
		isVisited = true;
	}
	void getCoords(int &_x, int &_y){
		_x = x;
		_y = y;
	}

	enum 
	{
		State_path,
		State_wall,
		State_max
	};
};

class Maze_DepthFirst
{

public:
	Point** arr_mazeRow;      //迷宫数组
	int mazeRow;              //储存整个迷宫矩阵的维数


	 Maze_DepthFirst();
	~Maze_DepthFirst();
	void initArrayMaze(int rowNum);           //初始化迷宫数组
	void generateMaze();

	bool existNotVisited();                   //判断所有结点是否都遍历完了   
	bool getNeighbour(int &x,int &y);           //判断是否含有未被访问的邻结点
	bool pointIsVaild(int x, int y);          //判断结点是否在迷宫数组的范围内
	double random(double start, double end);  //产生随机数
	void printMazeArray();


	enum 
	{
		Dir_Up,
		Dir_Down,
		Dir_Right,
		Dir_Left,
		Dir_Max
	};


private:
	int nodeRow;              //储存结点矩阵的维数
	
	Point** arr_nodeRow;       //结点数组

	std::stack<Point> mPoint;  //存储访问过的结点，用于回溯

};


#endif