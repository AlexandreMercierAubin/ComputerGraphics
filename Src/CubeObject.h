#pragma once
#include "AbstractObject.h"

class CubeObject : public AbstractObject
{
private:
	GLuint VertexArray;
	glm::vec3 vertices[8];

	void createVertices(GLfloat width, GLfloat height, GLfloat depth);
	void createForm(GLfloat width, GLfloat height, GLfloat depth);

	const unsigned int indices[36] = {
		0,3,1,//left
		0,2,3,
		5,7,6,//right
		5,6,4,
		0,1,5,//face
		0,5,4,
		2,7,3,//back
		2,6,7,
		1,3,7,//top
		1,7,5,
		0,6,2,//bottom
		0,4,6
	};


public:
	virtual void Create(GLuint &Program);
	virtual void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights);
	~CubeObject();
};
