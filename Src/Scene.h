#pragma once
#include <glm\glm.hpp>
#include "Model.h"
#include <iostream>
#include <array>
#include <stdio.h>
#include <vector>
#include "QuadObject.h"
#include "SkyboxObject.h"
#include "CubeObject.h"
#include "ShaderLoader.h"
#include "ModelShader.h"
#include "SkyboxShader.h"

class Scene
{
	

private:

	struct objetMonde;

	glm::mat4 MatView(bool staticPos);
	glm::mat4 perspective;
	glm::mat4 view;

	glm::vec3 direction;
	glm::vec4 uniformCouleur;

	glm::vec3 position;
	glm::vec3 orientation;
	GLfloat yaw;
	GLfloat pitch;

	//temp test change for an array for scene graph
	QuadObject quad;
	SkyboxObject mySkybox;
	CubeObject cube;
	Model g_requinModel;
	GLuint shaderID;
	GLuint skyboxID;
	GLuint bufferID;
	GLuint bufferColorID;
	float g_angleLumiere = glm::radians(270.0f);
	float g_intensiteLumiere = 1;
	//end of test

public:
	Scene(void);
	~Scene(void);

	void setupScene();

	void deleteScene();

	void drawSkybox();
	void drawModel();

};
