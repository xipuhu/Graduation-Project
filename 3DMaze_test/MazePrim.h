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
	Point_prim** arr_mazeRow;      //�Թ�����
	int mazeRow;              //���������Թ������ά��


	Maze_Prim();
	~Maze_Prim();

	void initArrayMaze(int rowNum);           //��ʼ���Թ�����
	void generateMaze();

	bool wallIsValid(int _x,int _y);          //�жϽ�������ǽ���Թ��������Ƿ���Ч

	void addWall(int _x, int _y);              //��ӽ���ǽ

	void delWallFromVec(int index);                     //��ǽ���б���ɾ��
	double random(double start, double end);  //���������
	void printMazeArray();


private:
	int nodeRow;              //����������ά��
	

	Point_prim** arr_nodeRow;       //�������


	std::vector<Point_prim> mWall;  //�洢�ѷ��ʹ��Ľ�����Թ��е�ǽ
};


#endif