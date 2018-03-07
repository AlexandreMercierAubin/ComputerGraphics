#pragma once
#include "AbstractObject.h"
#include "Model.h"

class ModelObject: public AbstractObject
{
public:
	virtual void Create(GLuint &Program);
	virtual void Draw(glm::mat4 &perspective,glm::mat4 &view);
	//virtual void SetColor(glm::vec4 color);
private:
	GLuint program;
	glm::vec4 color;
};
