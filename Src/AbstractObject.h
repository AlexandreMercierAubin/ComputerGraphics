#pragma once
#include <glm\glm.hpp>
#include "Model.h"
#include<cmath>

class AbstractObject
{
public:
	virtual void Create(GLuint &Program) { program = Program; }
	virtual void Draw(glm::mat4 &perspective, glm::mat4 &view) {}
	AbstractObject();
	virtual ~AbstractObject() {};

	virtual void MatRotation(const glm::vec3 &r);
	virtual void MatRotationDegree(const glm::vec3 &r);
	virtual void MatRotationQuaternion(const glm::quat &r);
	virtual void MatTranslation(const glm::vec3 &position);
	virtual void MatScale(const glm::vec3 &scale);

	virtual glm::vec3 getRotation();
	virtual glm::vec3 getRotationDegree();
	virtual glm::quat getRotationQuaternion();
	virtual glm::vec3 getTranslation();
	virtual glm::vec3 getScale();

	bool isSelected();
	void setSelected(bool value);
	string getName();

protected: 

	string name = "Abstract";
	GLuint program;
	glm::vec3 rotation;
	glm::vec3 translation;
	glm::vec3 scale;

	virtual void uniformLight(GLuint &program, glm::vec3 &color, glm::vec3 &direction, float ambientIntensity, float diffuseIntensity);
	virtual void uniformColor(GLuint &program, glm::vec4 &uniformColor);

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
