#pragma once
#include <glm\glm.hpp>
#include "Model.h"
#include <iostream>
#include <array>
#include <stdio.h>
#include <vector>
#include "QuadObject.h"
#include "AbstractObject.h"
#include "SkyboxObject.h"
#include "CubeObject.h"
#include "ShaderLoader.h"
#include "ModelShader.h"
#include "SkyboxShader.h"
#include "ModelObject.h"

class Scene
{
	

private:
	struct objetMonde;
	float sensitivity=0.2f;
	float mouvementSpeed = 0.03f;

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
	
	CubeObject cube;
	Model g_requinModel;
	GLuint shaderID;
	GLuint skyboxID;
	GLuint bufferID;
	GLuint bufferColorID;
	//end of test
	std::vector<std::shared_ptr<AbstractObject>> vObject ={make_shared<SkyboxObject>(),make_shared<ModelObject>()};
public:
	Scene(void);
	~Scene(void);

	void setupScene();

	void deleteScene();

	void drawSkybox();
	void drawScene();
	

	void mouseMotion(const unsigned int & timestamp, const unsigned int & windowID, const unsigned int & state, const int & x, const int & y, const int & xRel, const int & yRel);

	void addObject(shared_ptr<AbstractObject> object);

	struct KeyFlags
	{
		bool flagUp, flagDown, flagLeft, flagRight;
	};
	void refreshScene(KeyFlags flags);
};
