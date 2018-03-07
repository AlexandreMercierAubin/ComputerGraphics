#pragma once
#include "AbstractObject.h"

class QuadObject: public AbstractObject
{
private:
	GLuint program;
	GLuint VertexArray;
	glm::vec3 vertices[4];
	const unsigned int indices[6] = {
		0,1,2,//face
		0,3,2
	};
public:
	virtual void Create(GLuint &Program);
	virtual void Draw(glm::mat4 &perspective, glm::mat4 &view);
	virtual void Delete();
	//virtual void SetColor();
};
