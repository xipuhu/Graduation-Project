#ifndef _SKYBOX_H
#define _SKYBOX_H

// Std. Includes
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLFW
#include <GLFW/glfw3.h>
#include <SOIL\SOIL.h>
#include "Shader.h"
#include "Camera.h"

class SkyBox
{
public:

	void LoadSkyboxShader(GLchar* vertexPath,GLchar* fragPath){
		skyboxShader.loadShaderCode(vertexPath, fragPath);
		skyboxShader.compileShader();
		skyboxShader.linkToProgram();
	}
	void SetupSkyboxVAO();
	void LoadSkyboxTextures(GLchar* path[]);
	void RenderSkybox(Camera camera, GLuint screenWidth, GLuint screenHeight);

	SkyBox();
	~SkyBox();

private:
	vector<const GLchar*> faces;
	GLuint cubemapTextureID;
	GLuint skyboxVAO, skyboxVBO;
	Shader skyboxShader;

};

void SkyBox::SetupSkyboxVAO(){
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

}

void SkyBox::LoadSkyboxTextures(GLchar* path[]){
	for (int i = 0; i < 6; ++i){
		faces.push_back(path[i]);
	}
	GLuint TextureID;

	glGenTextures(1, &TextureID);
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &TextureID);
	int width, height;
	unsigned char* image;
	
	//立方体贴图及参数设置
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
	for (GLuint i = 0; i < faces.size(); ++i){
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	cubemapTextureID = TextureID;
}

void SkyBox::RenderSkybox(Camera camera, GLuint screenWidth, GLuint screenHeight){

	glDepthMask(GL_FALSE);    //将深度缓存读入开关关闭

	skyboxShader.Use();
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	

}
SkyBox::SkyBox()
{

}

SkyBox::~SkyBox()
{
}

#endif