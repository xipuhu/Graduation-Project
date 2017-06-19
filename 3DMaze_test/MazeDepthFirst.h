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
	Point** arr_mazeRow;      //�Թ�����
	int mazeRow;              //���������Թ������ά��


	 Maze_DepthFirst();
	~Maze_DepthFirst();
	void initArrayMaze(int rowNum);           //��ʼ���Թ�����
	void generateMaze();

	bool existNotVisited();                   //�ж����н���Ƿ񶼱�������   
	bool getNeighbour(int &x,int &y);           //�ж��Ƿ���δ�����ʵ��ڽ��
	bool pointIsVaild(int x, int y);          //�жϽ���Ƿ����Թ�����ķ�Χ��
	double random(double start, double end);  //���������
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
	int nodeRow;              //����������ά��
	
	Point** arr_nodeRow;       //�������

	std::stack<Point> mPoint;  //�洢���ʹ��Ľ�㣬���ڻ���

};


#endif