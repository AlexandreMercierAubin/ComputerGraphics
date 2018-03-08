#pragma once
#include "AbstractObject.h"
#include "Model.h"

class ModelObject: public AbstractObject
{
public:
	virtual void Create(GLuint &Program);
	virtual void Draw(glm::mat4 &perspective,glm::mat4 &view);
	virtual void Delete();
	//virtual void SetColor(glm::vec4 color);
private:
	GLuint program;
	glm::vec4 color;
	glm::vec3 position;
	glm::vec3 scale;
	Model model;
	float diffuseIntensity;
	float ambientIntensity;
};
