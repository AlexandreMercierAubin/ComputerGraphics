#pragma once
#include "AbstractObject.h"

class SBPyramidObject : public AbstractObject
{
private:
	GLuint VertexArray;
	glm::vec3 vertices[8];

	void createVerticesPyramid(GLfloat width, GLfloat height, GLfloat depth);
	void createPyramid(GLfloat width, GLfloat height, GLfloat depth);

	const unsigned int indices[18] = {
		0,1,2,//top
		0,2,3,
		0,3,4,
		0,4,1,
		1,2,4,
		2,4,3
	};

	std::vector<char*> faces;

public:
	virtual void Create(GLuint &Program);
	virtual void Draw(glm::mat4 &perspective, glm::mat4 &view);
	~SBPyramidObject();
	//virtual void SetColor();
};
