#include "Maze.h"  
#include <stdio.h>  
#include <time.h>  
#include <stdlib.h>  

Maze::Maze() :
m_nCol(0),
m_nRow(0)
{
	m_arrRaw = NULL;
	m_arrMaze = NULL;

	srand((int)time(0));
	rand();
}

Maze::~Maze()
{
	reset();
}

void Maze::initMazeArray(int rownum, int colnum)
{
	//  raw data  
	m_nRawRow = rownum;
	m_nRawCol = colnum;
	m_arrRaw = new MazePoint*[m_nRawRow];
	for (int i = 0; i<m_nRawRow; i++)
	{
		m_arrRaw[i] = new MazePoint[m_nRawCol]();
		for (int j = 0; j<m_nRawCol; j++)
		{
			m_arrRaw[i][j].setCoord(j, i);
		}
	}

	//  generated data  
	m_nRow = 2 * rownum + 1;
	m_nCol = 2 * colnum + 1;
	m_arrMaze = new MazePoint*[m_nRow];        
	for (int i = 0; i<m_nRow; i++)
	{
		m_arrMaze[i] = new MazePoint[m_nCol]();
		for (int j = 0; j<m_nCol; j++)
		{
			m_arrMaze[i][j].setCoord(j, i);
		} 
	}
}

void Maze::printMazeArray()
{
	printf("Maze Map:\n");
	for (int i = 0; i < m_nRow; i++)
	{
		for (int j = 0; j < m_nCol; j++)
		{
			printf("%d", m_arrMaze[i][j]._state);
		}
		printf("\n");
	}
#ifdef  DEBUG_MAZE_GENERATE  
	printf("Raw data:\n");
	for (int i = 0; i < m_nRawRow; i++)
	{
		for (int j = 0; j < m_nRawCol; j++)
		{
			printf("%d", m_arrRaw[i][j]._state);
		}
		printf("\n");
	}
#endif  
}


void Maze::autoGenerate()
{
	int curx, cury;
	int nextx, nexty;
	curx = cury = 0;

	m_arrRaw[cury][curx].setVisited(true);
	m_ptStack.push_back(m_arrRaw[cury][curx]);
	while (isThereUnvisited())
	{
		nextx = curx;
		nexty = cury;

		if (getNeighbor(nextx, nexty))
		{
#ifdef  DEBUG_MAZE_GENERATE  
			//printf("nextx: %d,nexty: %d\n", nextx, nexty);
#endif    
			m_arrRaw[nexty][nextx].setVisited();
			m_ptStack.push_back(m_arrRaw[nexty][nextx]);

			//  modfiy arrMaze[][] ,mark it passable between (curx,cury) => (nextx,nexty)  
			m_arrMaze[2 * cury + 1][2 * curx + 1]._state = MazePoint::E_State_Path;
			m_arrMaze[2 * nexty + 1][2 * nextx + 1]._state = MazePoint::E_State_Path;
			m_arrMaze[(2 * cury + 1 + 2 * nexty + 1) / 2][(2 * curx + 1 + 2 * nextx + 1) / 2]._state = MazePoint::E_State_Path;


			curx = nextx;
			cury = nexty;
		}
		else if (m_ptStack.size() > 1)
		{
			m_ptStack.pop_back();
			curx = m_ptStack[m_ptStack.size() - 1]._x;
			cury = m_ptStack[m_ptStack.size() - 1]._y;
		}
		else if (m_ptStack.size() == 1)
		{
			curx = m_ptStack[m_ptStack.size() - 1]._x;
			cury = m_ptStack[m_ptStack.size() - 1]._y;
			m_ptStack.pop_back();
		}
		else
		{
			//  is it possible ?  
			printf("Stack is NULL\n");
		}
	}
}

bool Maze::isThereUnvisited()
{
	for (int i = 0; i < m_nRawRow; i++)
	{
		for (int j = 0; j < m_nRawCol; j++)
		{
			if (!m_arrRaw[i][j].IsVisited())
				return true;
		}
	}
	return false;
}


bool Maze::isThePointInBoundary(int x, int y)
{
	return x >= 0 && x < m_nRawCol &&  y >= 0 && y < m_nRawRow;
}

double Maze::random(double start, double end)
{
	return start + (end - start)*rand() / (RAND_MAX + 1.0);
}

bool Maze::getNeighbor(int &x, int &y)
{
	int neighborX[E_Dir_Max];
	int neighborY[E_Dir_Max]; 
	bool isValid[E_Dir_Max];

	neighborX[E_Dir_Right] = x + 1;
	neighborY[E_Dir_Right] = y;
	isValid[E_Dir_Right] = (isThePointInBoundary(neighborX[E_Dir_Right], neighborY[E_Dir_Right])
		&& !m_arrRaw[neighborY[E_Dir_Right]][neighborX[E_Dir_Right]].IsVisited());
	neighborX[E_Dir_Left] = x - 1;                                                        
	neighborY[E_Dir_Left] = y;
	isValid[E_Dir_Left] = (isThePointInBoundary(neighborX[E_Dir_Left], neighborY[E_Dir_Left])
		&& !m_arrRaw[neighborY[E_Dir_Left]][neighborX[E_Dir_Left]].IsVisited());
	neighborX[E_Dir_Down] = x;
	neighborY[E_Dir_Down] = y + 1;
	isValid[E_Dir_Down] = (isThePointInBoundary(neighborX[E_Dir_Down], neighborY[E_Dir_Down])
		&& !m_arrRaw[neighborY[E_Dir_Down]][neighborX[E_Dir_Down]].IsVisited());
	neighborX[E_Dir_Up] = x;
	neighborY[E_Dir_Up] = y - 1;
	isValid[E_Dir_Up] = (isThePointInBoundary(neighborX[E_Dir_Up], neighborY[E_Dir_Up])
		&& !m_arrRaw[neighborY[E_Dir_Up]][neighborX[E_Dir_Up]].IsVisited());

	//  there's no valid direction   
	bool isThereValidDir = false;
	for (int i = 0; i<E_Dir_Max; i++)
	{
		isThereValidDir = isThereValidDir || isValid[i];
	}
	if (!isThereValidDir)
	{
		return false;
	}

	//  there's valid direction ,get random one  
	double randnumtemp = random(0, 4);
	int randnum = int(randnumtemp);

	int finalDir = 0;
	for (int i = 0; i<E_Dir_Max; i++)
	{
		if (isValid[i])
		{
			finalDir = i;
			break;
		}
	}

	while (randnum)
	{
		randnum--;
		finalDir++;
		finalDir = finalDir == E_Dir_Max ? 0 : finalDir;

		while (!isValid[finalDir])
		{
			finalDir++;
			finalDir = finalDir == E_Dir_Max ? 0 : finalDir;
		}
	}
	x = neighborX[finalDir];
	y = neighborY[finalDir];
	return true;

}

void Maze::reset()
{
	//  delete generated data  
	if (m_arrMaze != NULL)
	{
		for (int i = 0; i<m_nRow; i++)
		{
			delete[](m_arrMaze[i]);
			m_arrMaze[i] = NULL;
		}
		delete[] m_arrMaze;
		m_arrMaze = NULL;
	}

	//  delete raw data  
	if (m_arrMaze != NULL)
	{
		for (int i = 0; i<m_nRawRow; i++)
		{
			delete[](m_arrRaw[i]);
			m_arrRaw[i] = NULL;
		}
		delete[] m_arrRaw;
		m_arrRaw = NULL;
	}
}