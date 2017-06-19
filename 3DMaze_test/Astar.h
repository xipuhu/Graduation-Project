#ifndef _ASTAR_H
#define _START_H

#include <iostream>
#include <vector>
#include <queue>
#include <glm\glm.hpp>     //GLSL����ѧ��
#include <functional>
#include "MazePrim.h"
using namespace std;

struct Grid
{
	int x, y;
	int F, G, H;
	int state;         //���ڱ�ע��ǽ����ͨ·
	Grid* parent;
	int dir;           //��¼�����ƶ��ڸ�ķ���

	Grid() :x(0), y(0), F(0), G(0), H(0),dir(-1),parent(nullptr)
	{
	   
	}
	//ʹ���ȶ��а�F��ֵ��С����������
	friend bool operator < (Grid a, Grid b){
		return a.F > b.F;
	}

};


class Astar
{
public:
	Astar();
	~Astar();

	//Manhattan(������)���ۺ���
	int Heuristic(const Grid currGrid, const Grid goalGrid){
		return (abs(goalGrid.x - currGrid.x) + abs(goalGrid.y - currGrid.y));
	}
	bool existInList(vector<Grid> List, Grid grid);
	void removeGirdFromList(vector<Grid> List, Grid grid);
	void FindPath(Maze_Prim pMaze);    //A*�㷨Ѱ��·��

	int moveDir[100];
	int dirNum;
private:
	priority_queue<Grid> openList; //�����ȶ�������ʾ�����б�
	vector<Grid> tempOpenList;  //�������������б�
	vector<Grid> closeList;    //�ر��б�
};


#endif