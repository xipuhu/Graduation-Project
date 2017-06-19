#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

#include<GL\glew.h>

using std::cout;
class Shader
{
public:
	//������ɫ������Դ
	std::string vertexCode;
	//Ƭ����ɫ������Դ
	std::string fragmentCode;

	GLuint vertex, fragment;

	//��ɫ������ID
	GLuint Program;

	void loadShaderCode(const GLchar* vertexPath, const GLchar* fragmentPath);
	void compileShader();
	void linkToProgram();
	//ʹ�ó���
	void Use();


	 Shader();

	~Shader();
private:

};

//���ļ�·���л�ȡ����/Ƭ����ɫ������
void Shader::loadShaderCode(const GLchar* vertexPath, const GLchar* fragmentPath){
	
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try{
		//���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		//��ȡ�ļ��Ļ������ݵ�����
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//�ر��ļ�
		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e){
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULL_READ" << std::endl;
	}

}

//������ɫ��
void Shader::compileShader(){

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLint success;
	GLchar infoLog[512];

	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//��ӡ�������
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//��ӡ�������
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

}
//������õ���ɫ���������
void Shader::linkToProgram(){

	GLint success;
	GLchar infoLog[512];
	//��ɫ������
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	//��ӡ���Ӵ���
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}
Shader::Shader(){

}

void Shader::Use(){
	glUseProgram(this->Program);
}

Shader::~Shader()
{

}

#endif