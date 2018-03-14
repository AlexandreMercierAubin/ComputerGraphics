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
#include "TexShader.h"
#include "ModelObject.h"
#include "GroupObject.h"

class Scene
{
	

private:
	struct objetMonde;
	const float sensitivity=0.2f;
	const float mouvementSpeed = 0.03f;
	float viewAngle = 30.0f;

	glm::mat4 MatView(bool staticPos);
	glm::mat4 MatPerspective(const float &angleOfView, const float &aspect, const float &near, const float &far);
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
	std::shared_ptr<GroupObject> objects;
	SkyboxObject skybox;

public:
	Scene(void);
	~Scene(void);

	void setupScene();
	void setPerspective(const float &angleOfView, const float &aspect, const float &near, const float &far);

	void drawSkybox();
	void drawScene();

	void mouseMotion(const unsigned int & timestamp, const unsigned int & windowID, const unsigned int & state, const int & x, const int & y, const int & xRel, const int & yRel);

	void addObject(shared_ptr<AbstractObject> object);

	void dollyZoom(float dolly, float zoom);

	struct KeyFlags
	{
		bool flagUp, flagDown, flagLeft, flagRight, flagLeftMouse, flagRightMouse;
	};
	void refreshScene(KeyFlags flags);

	std::shared_ptr<GroupObject> getObjects();
};
