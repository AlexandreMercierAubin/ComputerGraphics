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

	GLuint m_vertexArraySkybox;
	GLuint m_textureSkybox;
	GLuint m_vertexArraySol;
	GLuint m_textureSol;
	glm::vec3 m_sommetsCube[8];
	glm::vec2 m_textureCube[8];



public:
	Scene(void);
	~Scene(void);

	void uniformLight(GLuint &program, glm::vec3 &color, glm::vec3 &direction, float ambientIntensity, float diffuseIntensity);
	void uniformColor(GLuint &program, glm::vec4 &uniformColor);

	void deleteScene();

	void drawModel(GLuint &program, glm::mat4 &view, glm::mat4 &perspective, Model &model, glm::vec3 &matTranslation, glm::vec3 &matScale, glm::vec4 &color, float &lightIntensity, glm::vec3 &lightDirection);
};
