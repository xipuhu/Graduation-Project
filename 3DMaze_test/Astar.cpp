
#include "Astar.h"

void Astar::removeGirdFromList(vector<Grid> List, Grid grid){
	bool findGrid = false;

	for (int i = 0; i < List.size() - 1; ++i){
		if (List[i].x == grid.x&&List[i].y == grid.y)
			findGrid = true;
		if (findGrid)
			List[i] = List[i + 1];      //ʹList�е�Ԫ����ǰ�ƶ�
	}
	if (findGrid){
		List.pop_back();     //ɾ��Ԫ��
	} 

}
bool Astar::existInList(vector<Grid> List, Grid grid){
	bool exist = false;
	for (auto it : List){
		if (grid.x == it.x&&grid.y == it.y)
		{
			exist = true;
			break;
		}
			
	}
	return exist;
}

void Astar::FindPath(Maze_Prim Maze){

	Maze_Prim *pMaze = new Maze_Prim();
	(*pMaze) = Maze;

	Grid map[70][70];

	//�����㷨����
	while (!openList.empty())
		openList.pop();
	while (!tempOpenList.empty())
		tempOpenList.pop_back();
	while (!closeList.empty())
		closeList.pop_back();

	if (pMaze->mazeRow >= 70) {
		std::cout << "Astar:�Թ���С������Ԥ��ֵ��" << std::endl;
		return;
	}



	for (int i = 0; i < pMaze->mazeRow; ++i){
		for (int j = 0; j < pMaze->mazeRow; ++j){
			map[i][j].x = i;
			map[i][j].y = j;
			if (pMaze->arr_mazeRow[i][j].state == 1) map[i][j].state = 1;
			else
			{
				map[i][j].state = 0;
			}
		}//for
	}//for

	Grid startGrid = map[1][1];
	Grid endGrid = map[pMaze->mazeRow - 2][pMaze->mazeRow - 2];


	int dir[4][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

	openList.push(startGrid);
	tempOpenList.push_back(startGrid);

	while (!tempOpenList.empty()){

		Grid currGrid = openList.top();

		openList.pop();
		closeList.push_back(currGrid);
		removeGirdFromList(tempOpenList, currGrid);
	

		//�����Ŀ�������˹ر��б��У������·���Ѿ��ҵ�
		if (existInList(closeList, endGrid))
			break;

		for (int i = 0; i < 4; ++i){

			Grid neighbourGrid = map[currGrid.x + dir[i][0]][currGrid.y + dir[i][1]];

			//�ڸ��Ƿ�����Ч��Χ
			if (neighbourGrid.x < 0 || neighbourGrid.x >= pMaze->mazeRow || neighbourGrid.y < 0 || neighbourGrid.y >= pMaze->mazeRow)
				continue;
			//���ڸ��Ƿ�Ϊǽ
			if (neighbourGrid.state == 1)
				continue;

			//���ڸ��Ѿ����ڹر��б���
			if (existInList(closeList, neighbourGrid))
				continue;

			//���ڸ��ڿ����б���
			if (!existInList(tempOpenList, neighbourGrid)){
					
				neighbourGrid.parent = new Grid();
				(*neighbourGrid.parent) = currGrid;

				int g = currGrid.G + 1;
				int h = Heuristic(neighbourGrid, endGrid);
				neighbourGrid.G = g;
				neighbourGrid.H = h;
				neighbourGrid.F = g + h;
				
				//��¼��·���Ϸ����ƶ��ķ���
				switch (i)
				{
				case 0:
					neighbourGrid.dir = 3;
					break;
				case 1:
					neighbourGrid.dir = 2;
					break;
				case 2:
					neighbourGrid.dir = 1;
					break;
				case 3:
					neighbourGrid.dir = 0;
					break;
				default:
					break;
				}

				openList.push(neighbourGrid);
				tempOpenList.push_back(neighbourGrid);

		      if (neighbourGrid.x == endGrid.x&&neighbourGrid.y == endGrid.y)
					break;

			}
		

		}//for

	}

	//���ƶ��ķ����¼����
	Grid *gridPath = &closeList[closeList.size()-1];
	dirNum = 0;         //���������·���Ϸ���仯�Ĵ���
	while (gridPath != NULL){
		moveDir[dirNum++] = gridPath->dir;
		gridPath = gridPath->parent;
	}

}
Astar::Astar()
{
}

Astar::~Astar()
{

}
