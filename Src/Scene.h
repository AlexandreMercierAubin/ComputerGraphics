#pragma once
#include <glm\glm.hpp>
#include "Model.h"
#include <iostream>
#include <array>
#include <stdio.h>
#include <vector>




class Scene
{


private:

	struct Light
	{
		glm::vec3 color;
		float ambientIntensity;
		float diffuseIntensity;
		glm::vec3 direction;
	};

	struct objetMonde;

	GLuint vertexArraySkybox;
	GLuint textureSkybox;
	glm::vec3 verticesCube[8];
	glm::vec2 textureCube[8];

	GLuint loadCubemap(std::vector<char*> faces, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
	const unsigned int indicesCube[36] = {
		0,1,3,//left
		0,2,3,
		5,7,6,//right
		5,4,6,
		0,1,5,//face
		0,4,5,
		2,3,7,//back
		2,6,7,
		1,3,7,//top
		1,5,7,
		0,2,6,//bottom
		0,4,6
	};


public:
	Scene(void);
	~Scene(void);

	void uniformLight(GLuint &program, glm::vec3 &color, glm::vec3 &direction, float ambientIntensity, float diffuseIntensity);
	void uniformColor(GLuint &program, glm::vec4 &uniformColor);

	void deleteScene();

	void createSkybox(GLfloat width, GLfloat height);
	void createCube(GLfloat width, GLfloat height);
	void createTextureCube();
	void createVerticesCube(GLfloat largeur, GLfloat hauteur);

	void drawSkybox(glm::mat4 view, glm::mat4 perspective, GLuint program, glm::vec4 color);
	void drawModel(GLuint &program, glm::mat4 &view, glm::mat4 &perspective, Model &model, glm::vec3 &matTranslation, glm::vec3 &matScale, glm::vec4 &color, float &lightIntensity, glm::vec3 &lightDirection);
};
