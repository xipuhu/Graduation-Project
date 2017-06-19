#ifndef _PRIM_H
#define _PRIM_H

#include<stack>
#include<vector>
struct Point_prim
{
	int x;
	int y;
	bool isVisited;
	int state;


	Point_prim(){
		x = 0;
		y = 0;
		isVisited = false;
		state = State_wall;
	}

	Point_prim(int _x, int _y) :x(_x), y(_y){}

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
class Maze_Prim
{
public:
	Point_prim** arr_mazeRow;      //迷宫数组
	int mazeRow;              //储存整个迷宫矩阵的维数


	Maze_Prim();
	~Maze_Prim();

	void initArrayMaze(int rowNum);           //初始化迷宫数组
	void generateMaze();

	bool wallIsValid(int _x,int _y);          //判断结点包含的墙在迷宫数组内是否有效

	void addWall(int _x, int _y);              //添加结点的墙

	void delWallFromVec(int index);                     //将墙从列表中删除
	double random(double start, double end);  //产生随机数
	void printMazeArray();


private:
	int nodeRow;              //储存结点矩阵的维数
	

	Point_prim** arr_nodeRow;       //结点数组


	std::vector<Point_prim> mWall;  //存储已访问过的结点在迷宫中的墙
};


#endif