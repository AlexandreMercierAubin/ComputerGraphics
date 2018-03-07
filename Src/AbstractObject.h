#pragma once
#include <glm\glm.hpp>
#include "Model.h"

class AbstractObject
{
public:
	virtual void Create(GLuint &Program) {}
	virtual void Draw(glm::mat4 &perspective, glm::mat4 &view) {}
	virtual void Delete() {}
	//virtual void SetColor(glm::vec4 color);
	virtual ~AbstractObject() {};
protected: 

	virtual void uniformLight(GLuint &program, glm::vec3 &color, glm::vec3 &direction, float ambientIntensity, float diffuseIntensity);
	virtual void uniformColor(GLuint &program, glm::vec4 &uniformColor);
	virtual void MatRotation(GLuint program, glm::vec3 &r);
	virtual void MatTranslation(GLuint program, const glm::vec3 &position);
	virtual void MatScale(GLuint program, const glm::vec3 &scale);

	struct Light
	{
		glm::vec3 color;
		float ambientIntensity;
		float diffuseIntensity;
		glm::vec3 direction;
	};
	
};
