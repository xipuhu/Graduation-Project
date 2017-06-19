#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include<vector>
#include<GL\glew.h>
#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include"Maze.h"
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	_DEFAULT   //Ĭ����
};

const GLfloat YAW        = -90.0f;
const GLfloat PITCH      = 0.0f;
const GLfloat SPEED      = 2.0f;
const GLfloat SENSITIVITY = 0.15f;
const GLfloat ZOOM       = 45.0f;

class Camera
{
public:
	//���������ֵ
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//�������һʱ�̵�����ֵ
	glm::vec3 LastPosition;
	glm::vec3 LastFront;
	glm::vec3 LastUp;
	glm::vec3 LastRight;

	//�۹�Ʒ���
	glm::vec3 spotLightDir;

	//�������һʱ�̵�λ��������ײ���
	bool Lock_Y;
	bool moveLock;

	//ƫ����
	GLfloat Yaw;
	//������
	GLfloat Pitch;  

	GLfloat MovementSpeed;          //�ӽ��ƶ��ٶ�
	GLfloat MouseSensitivity;       //�ӽ���ת��������
	GLfloat Zoom;                   //�ӽ�����ֵ

	Camera_Movement moveDir;       //������ƶ�����

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;

	

		this->UpdateCameraVectors();

		this->Lock_Y = true;        //�Ƿ��ӽ�Y��ֵ������y=0
		this->moveLock = true;     //�Ƿ�����ƶ��ӽ�
		this->LastPosition = position;

		this->moveDir = _DEFAULT;   //��ʼ��������ƶ�����
		this->spotLightDir = position + Front;  //�۹�Ʒ����ʼ��

		

	}

	glm::mat4 GetViewMatrix(){
		
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime){

		GLfloat velocity = this->MovementSpeed*deltaTime;

		//�����������һʱ�̵�λ��
		LastPosition = this->Position;

		if (direction == FORWARD) this->Position += this->Front*velocity;
		if (direction == BACKWARD) this->Position -= this->Front*velocity;
		if (direction == LEFT)     this->Position -= this->Right*velocity;
		if (direction == RIGHT)   this->Position += this->Right*velocity;

		//���ӽ�������xozƽ����
		if (Lock_Y)
		this->Position.y = 0.0f;

	}

	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true){
		xOffset *= this->MouseSensitivity;
		yOffset *= this->MouseSensitivity;

		this->Yaw   += xOffset;
		this->Pitch += yOffset;
	  
		if (constrainPitch){
			if (this->Pitch > 89.0f)  this->Pitch = 89.0f;
			if (this->Pitch < -89.0f) this->Pitch = -89.0f;
		}

		this->UpdateCameraVectors() ;
	}

	void ProcessMouseScroll(GLfloat yOffset){
	
		if (this->Zoom >= 1.0f&&this->Zoom <= 90.0f) this->Zoom -= yOffset;
		if (this->Zoom <= 1.0f)  this->Zoom = 1.0f;
		if (this->Zoom >= 90.0f) this->Zoom = 90.0f;
	}

	~Camera();

private:
	//�������������
	void UpdateCameraVectors(){
		glm::vec3 front;
		//�ο�������
		front.x = cos(glm::radians(this->Pitch))*cos(glm::radians(this->Yaw));
		front.y = sin(glm::radians(this->Pitch));
		front.z = cos(glm::radians(this->Pitch))*sin(glm::radians(this->Yaw));

		this->Front = glm::normalize(front);

		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));

		this->spotLightDir = this->Front;

	}
};


Camera::~Camera()
{

}
#endif