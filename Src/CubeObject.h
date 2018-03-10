#pragma once
#include "AbstractObject.h"

class CubeObject : public AbstractObject
{
private:
	GLuint VertexArray;
	glm::vec3 vertices[8];

	void createVerticesCube(GLfloat width, GLfloat height, GLfloat depth);
	void createCube(GLfloat width, GLfloat height, GLfloat depth);

	const unsigned int indices[36] = {
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

	std::vector<char*> faces;

public:
	virtual void Create(GLuint &Program);
	virtual void Draw(glm::mat4 &perspective, glm::mat4 &view);
	~CubeObject();
};
