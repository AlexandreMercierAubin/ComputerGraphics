#pragma once
#include <glm\glm.hpp>
#include "Model.h"
#include<cmath>

class AbstractObject
{
public:
	struct Light
	{
		glm::vec3 color;
		float ambientIntensity;
		float diffuseIntensity;
		glm::vec3 direction;
		glm::vec3 position;
	};

	virtual void Create(GLuint &Program) {}
	virtual void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos) {}
	virtual ~AbstractObject() {};
	AbstractObject();
	virtual Light getLight();
	virtual void setLight(glm::vec3 color, glm::vec3 direction, glm::vec3 position, float ambientIntensity, float diffuseIntensity);
	virtual void setPosition(glm::vec3 pos);
	virtual void addPosition(const glm::vec3 &pos);
	virtual glm::vec3 getPosition();
	virtual void setRotationDegree(glm::vec3 rot);
	virtual void addRotationDegree(const glm::vec3 &rot);
	virtual  glm::vec3 getRotationDegree();
	virtual void setRotationQuaternion(glm::quat quat);
	virtual void addRotationQuaternion(const glm::quat &quat);
	virtual  glm::quat getRotationQuaternion();
	virtual void setScale(glm::vec3 scl);
	virtual void addScale(const glm::vec3 &scl);
	virtual  glm::vec3 getScale();
	virtual void setColor(glm::vec4 Color);
	virtual glm::vec4 getColor();
	bool isSelected();
	void setSelected(bool value);
	string getName();

protected:

	string name = "Abstract";

	virtual void uniformLight(GLuint &program, const AbstractObject::Light &inLight);
	virtual void uniformColor(GLuint &program, glm::vec4 &uniformColor);
	virtual void uniformCameraPosition(GLuint &program, glm::vec3 &uniformCameraPos);
	virtual void MatRotation(const GLuint &program, glm::mat4 &rotat, const glm::vec3 &r);
	virtual void MatRotationDegree(const GLuint &program, glm::mat4 &rotat, const glm::vec3 &r);
	virtual void MatRotationQuaternion(const GLuint &program, glm::mat4 &rotat, const glm::quat &r);
	virtual void MatTranslation(GLuint program, glm::mat4 &trans, const glm::vec3 &position);
	virtual void MatScale(GLuint program, glm::mat4 &scl, const glm::vec3 &scale);

	glm::vec4 color;
	glm::vec3 rotationDegree;
	glm::quat rotationQuat;
	glm::vec3 position;
	glm::vec3 scale;

	GLuint program;


	Light light;

private:
	bool selected = false;



};
