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
	//顶点着色器代码源
	std::string vertexCode;
	//片段着色器代码源
	std::string fragmentCode;

	GLuint vertex, fragment;

	//着色器程序ID
	GLuint Program;

	void loadShaderCode(const GLchar* vertexPath, const GLchar* fragmentPath);
	void compileShader();
	void linkToProgram();
	//使用程序
	void Use();


	 Shader();

	~Shader();
private:

};

//从文件路径中获取顶点/片段着色器代码
void Shader::loadShaderCode(const GLchar* vertexPath, const GLchar* fragmentPath){
	
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try{
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		//读取文件的缓冲内容到流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//关闭文件
		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e){
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULL_READ" << std::endl;
	}

}

//编译着色器
void Shader::compileShader(){

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLint success;
	GLchar infoLog[512];

	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//打印编译错误
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//打印编译错误
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

}
//将编译好的着色器载入程序
void Shader::linkToProgram(){

	GLint success;
	GLchar infoLog[512];
	//着色器程序
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	//打印连接错误
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//删除着色器，它们已经连接到我们的程序中了，已经不再需要
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