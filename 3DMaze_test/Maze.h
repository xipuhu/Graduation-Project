#ifndef _MAZE_H_  
#define _MAZE_H_  

#include <vector>  
#define DEBUG_MAZE_GENERATE  
struct MazePoint
{
	int _x, _y;
	bool _bIsVisisted;

	MazePoint()
	{
		_x = _y = 0;
		_bIsVisisted = false;
		_state = E_State_Wall;
	}

	MazePoint(int x, int y)
	{
		_x = x;
		_y = y;
		_bIsVisisted = false;
	}

	void setCoord(int x, int y)
	{
		_x = x;
		_y = y;
	}

	void operator = (const MazePoint& pt)
	{
		_x = pt._x;
		_y = pt._y;
		_bIsVisisted = pt._bIsVisisted;
	}

	bool IsVisited()
	{
		return _bIsVisisted;
	}

	void setVisited(bool visited = true)
	{
		_bIsVisisted = visited;
	}

	enum
	{
		E_State_Path,
		E_State_Wall,
		E_State_Entry,
		E_State_Dest,
		E_State_Max,
	};
	int _state;
};

class Maze
{
protected:
	int     m_nCol, m_nRow, m_nRawCol, m_nRawRow;

	//  generate helper  
	//MazePoint** m_arrMaze;
	MazePoint** m_arrRaw;
	MazePoint m_ptStart;
	MazePoint m_ptCur;
	std::vector<MazePoint> m_ptStack;
public:
	Maze();
	virtual ~Maze();

	
	void initMazeArray(int rownum, int colnum);
	void printMazeArray();
	//  generate helper  
	void autoGenerate();
	bool isThereUnvisited();
	bool isThePointInBoundary(int x, int y);
	void reset();

	////获得迷宫数据
	//MazePoint** getMaze(){
	//	return m_arrMaze;
	//}

	enum
	{
		E_Dir_Up,
		E_Dir_Down,
		E_Dir_Left,
		E_Dir_Right,
		E_Dir_Max,
	};
	double random(double start, double end);
	bool getNeighbor(int &x, int &y);

	MazePoint** m_arrMaze;
};
#endif  //_MAZE_H_  