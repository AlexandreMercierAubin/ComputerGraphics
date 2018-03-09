#pragma once
#include <glm\glm.hpp>
#include "Model.h"
#include<cmath>

class AbstractObject
{
public:
	virtual void Create(GLuint &Program) {}
	virtual void Draw(glm::mat4 &perspective, glm::mat4 &view) {}
	virtual ~AbstractObject() {};

	bool isSelected();
	void setSelected(bool value);
	string getName();

protected: 

	string name = "Abstract";

	virtual void uniformLight(GLuint &program, glm::vec3 &color, glm::vec3 &direction, float ambientIntensity, float diffuseIntensity);
	virtual void uniformColor(GLuint &program, glm::vec4 &uniformColor);
	virtual void MatRotation(const GLuint &program, const glm::vec3 &r);
	virtual void MatRotationDegree(const GLuint &program, const glm::vec3 &r);
	virtual void MatRotationQuaternion(const GLuint &program, const glm::quat &r);
	virtual void MatTranslation(GLuint program, const glm::vec3 &position);
	virtual void MatScale(GLuint program, const glm::vec3 &scale);

	struct Light
	{
		glm::vec3 color;
		float ambientIntensity;
		float diffuseIntensity;
		glm::vec3 direction;
	};
	
private:
	bool selected = false;
};
