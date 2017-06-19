#ifndef TEXTURE_H
#define TEXTURE_H

#include<string.h>
#include<GL\glew.h>
#include<SOIL\SOIL.h>
class Texture
{
public:

	//����ID
	GLuint ID;
	
	//��������
	void loadTexture(const GLchar* texturePath, GLuint id){
		glGenTextures(id, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGB);
	}


	//���û��Ʒ�ʽ
	void setWrapping(const GLchar* mays);

	//���ù��˷�ʽ
	void setFiltering(const GLchar* mays);

	void loadTexture_frame(GLchar* path, GLboolean alpha = false);

	//��������
	void getTexture();

	//�����������ͷ�ͼ������
	void releaseTexture();

	Texture();
	~Texture();

private:

	//ͼƬ����ֵ��߶�ֵ
	int width;
	int height;
	//��ȡͼƬ���ݱ���
	unsigned char* image;


};

void Texture::setWrapping(const GLchar* ways){
	if (ways == "REPET"){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	
}
void Texture::setFiltering(const GLchar* ways){
	if (ways == "NEAREST"){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	if (ways == "LINER"){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

}

void Texture::getTexture(){
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
}
void Texture::releaseTexture(){
	
	SOIL_free_image_data(image);   //�ͷ�ͼ������
	glBindTexture(GL_TEXTURE_2D, 0);	//����������
}
void Texture::loadTexture_frame(GLchar* path, GLboolean alpha){
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Use GL_MIRRORED_REPEAT to prevent white borders. Due to interpolation it takes value from next repeat 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	ID = textureID;
}
Texture::Texture(){

}
Texture::~Texture()
{
}
#endif