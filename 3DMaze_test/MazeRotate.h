#ifndef MAZEROTATE_H
#define MAZEROTATE_H

#include<iostream>
#include <glm\glm.hpp>     //GLSL的数学库
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include<GL\glew.h>

#define M_PI 3.14

class MazeRotate
{
public:
	//旋转迷宫的属性值
	GLfloat lastX, lastY;
	GLfloat curX, curY;
	GLfloat angleX, angleY;
	bool trackLock;

	MazeRotate();

	void setRotation(double x, double y, GLuint width, GLuint height, GLfloat deltaTime){
		curX = x;
		curY = y;
		angleY += (curX - lastX) / width*180.0f*deltaTime*2.0f;
		angleX += (lastY-curY) / height*180.0f*deltaTime*2.0f;
	}



	/*
	glm::vec3 getPointVec( GLfloat pos_x, GLfloat pos_y){
		GLfloat radius = heigh / 2;

		if ((pos_x*pos_x + pos_y*pos_y) > (radius*radius))    //如果鼠标移出以radius为半径的平面圆
			pos_y = sqrtf(radius*radius - pos_x*pos_x);

		GLfloat x,y, z;
		x = pos_x;
		z = pos_y;
		y = sqrt(radius*radius - x*x);

		glm::vec3 center = glm::vec3(width / 2, heigh / 2, 0.0f);
		glm::vec3 point = glm::vec3(x,y,z);

		glm::vec3 pointVec = point - center;

		return pointVec;
	
	}
	*/

	/*void MakeMazeRotate(GLfloat pos_x, GLfloat pos_y){
		glm::vec3 firstPoint = glm::normalize(getPointVec(lastX, lastY));
		glm::vec3 secondPoint = glm::normalize(getPointVec(pos_x, pos_y));
		glm::vec3 n= glm::cross(firstPoint, secondPoint);
		                   
		angle = sqrt(glm::dot(n, n));
		rotateDir = glm::normalize(n);
	    
		lastX = pos_x;
		lastY = pos_y;
	}*/
	
	/*
	void trackball(int x, int y, glm::vec3 v){
		float d;
		v.x = (2.0f*x - width) / width;
		v.y = (heigh - 2.0f*y) / heigh;

		d = sqrt(x*x + y*y);
		v.z = cos((M_PI / 2.0f)*((d < 1.0f) ? d : 1.0f));

		v = glm::normalize(v);

	}

	void mouseMotion(int  x, int y){
		GLfloat dx, dy, dz;
		glm::vec3 curPos;
		trackball(x, y, curPos);

		if (trackingMouse == true){
			dx = curPos.x - lastPos.x;
			dy = curPos.y - lastPos.y;
			dz = curPos.z - lastPos.z;

			if (dx || dy || dz){
				angle = 90.0f*sqrt(dx*dx + dy*dy + dz*dz);
				rotateDir = glm::cross(curPos, lastPos);
				lastPos = curPos;

			}
		}
	}
	void startMotion(int x, int y){
		trackingMouse = true;
		startX = x;
		startY = y;
		curx = x;
		cury = y;

		trackball(x, y, lastPos);
		trackballMove = true;
	}

	void stopMotion(int x, int y){
		trackingMouse = false;
		if (startX == x&&startY == y){
			angle = 0.0f;
			trackballMove = false;
		}
	}
	*/


	~MazeRotate();
   
private:

};

MazeRotate::MazeRotate()
{
	lastX = 0.0f;
	lastY = 0.0f;
	angleX = 0.0f;
	angleY = 0.0f;
	trackLock = false;
}

MazeRotate::~MazeRotate()
{
}



#endif