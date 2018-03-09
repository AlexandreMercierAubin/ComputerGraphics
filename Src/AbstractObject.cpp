#pragma once
#include "AbstractObject.h"

AbstractObject::AbstractObject()
{
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	translation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void AbstractObject::uniformColor(GLuint &program, glm::vec4 &uniformColor)
{
	GLuint vecCouleur = glGetUniformLocation(program, "vColor");
	glUniform4fv(vecCouleur, 1, &uniformColor[0]);
}

void AbstractObject::uniformLight(GLuint &program, glm::vec3 &color, glm::vec3 &direction, float ambientIntensity, float diffuseIntensity)
{
	Light light;
	light.color = color;
	light.ambientIntensity = ambientIntensity;
	light.diffuseIntensity = diffuseIntensity;
	light.direction = direction;

	glUniform3fv(glGetUniformLocation(program,
		"structLight.color"), 1, &light.color[0]);
	glUniform1f(glGetUniformLocation(program,
		"structLight.ambientIntensity"),
		light.ambientIntensity);
	glUniform1f(glGetUniformLocation(program,
		"structLumiere. structLight.diffuseIntensity"),
		light.diffuseIntensity);
	glUniform3fv(glGetUniformLocation(program,
		"structLight.direction"), 1,
		&light.direction[0]);
}

void AbstractObject::MatRotation(const glm::vec3 &r) // matrice de rotation
{
	rotation = r;

	//x:pitch y:yaw z:roll
	GLuint matRotation = glGetUniformLocation(program, "matRotation");

	glm::mat4 rotat;// rotation Y

	rotat[0][0] = cosf(r.z)*cosf(r.y)-sinf(r.z)*sinf(r.x)*sinf(r.y);  rotat[0][1] = -sinf(r.z)*cosf(r.x);		rotat[0][2] = cosf(r.z)*sinf(r.y)+sinf(r.z)*sinf(r.x)*cosf(r.y);	rotat[0][3] = 0.0f;
	rotat[1][0] = sinf(r.z)*cosf(r.y)+cosf(r.z)*sinf(r.x)*sinf(r.y);  rotat[1][1] = cosf(r.z)*cosf(r.x);		rotat[1][2] = sinf(r.z)*sinf(r.y)-cosf(r.z)*sinf(r.x)*cosf(r.y);	rotat[1][3] = 0.0f;
	rotat[2][0] = -cosf(r.x)*sinf(r.y);								  rotat[2][1] = sinf(r.x);					rotat[2][2] = cosf(r.x)*cosf(r.y);									rotat[2][3] = 0.0f;
	rotat[3][0] = 0.0f;												  rotat[3][1] = 0.0f;						rotat[3][2] = 0.0f;													rotat[3][3] = 1.0f;

	glUniformMatrix4fv(matRotation, 1, GL_FALSE, &rotat[0][0]);
}

void AbstractObject::MatRotationDegree(const glm::vec3 &r)
{
	rotation = glm::vec3(glm::radians(r.x), glm::radians(r.y), glm::radians(r.z));

	MatRotation(rotation);
}

void AbstractObject::MatRotationQuaternion(const glm::quat &r)
{
	rotation = glm::eulerAngles(r);

	//x:pitch y:yaw z:roll
	GLuint matRotation = glGetUniformLocation(program, "matRotation");

	glm::mat4 rotat;// rotation Y

	rotat[0][0] = 1 - 2 * pow(r.y, 2) - 2 * pow(r.z, 2);  rotat[0][1] = 2 * r.x*r.y - 2 * r.w*r.z;					rotat[0][2] = 2 * r.x*r.z + 2 * r.w*r.y;							rotat[0][3] = 0.0f;
	rotat[1][0] = 2 * r.x*r.y + 2 * r.w*r.z;			   rotat[1][1] = 1 - 2 * pow(r.x, 2) - 2 * pow(r.z, 2);		rotat[1][2] = 2 * r.y*r.z - 2 * r.w*r.x;							rotat[1][3] = 0.0f;
	rotat[2][0] = 2 * r.x*r.z - 2 * r.w*r.y;			   rotat[2][1] = 2 * r.y*r.z + 2 * r.w*r.x;					rotat[2][2] = 1 - 2 * pow(r.x, 2) - 2 * pow(r.y, 2);			rotat[2][3] = 0.0f;
	rotat[3][0] = 0.0f;							   rotat[3][1] = 0.0f;									rotat[3][2] = 0.0f;											rotat[3][3] = 1.0f;

	glUniformMatrix4fv(matRotation, 1, GL_FALSE, &rotat[0][0]);
}

void AbstractObject::MatTranslation(const glm::vec3 &position) // matrice de translation
{
	translation = position;

	GLuint matTrans = glGetUniformLocation(program, "matTranslation");

	glm::mat4 trans;

	trans[0][0] = 1.0f; trans[0][1] = 0.0f; trans[0][2] = 0.0f; trans[0][3] = position.x;
	trans[1][0] = 0.0f; trans[1][1] = 1.0f; trans[1][2] = 0.0f; trans[1][3] = position.y;
	trans[2][0] = 0.0f; trans[2][1] = 0.0f; trans[2][2] = 1.0f; trans[2][3] = position.z;
	trans[3][0] = 0.0f; trans[3][1] = 0.0f; trans[3][2] = 0.0f; trans[3][3] = 1.0f;

	glUniformMatrix4fv(matTrans, 1, GL_FALSE, &trans[0][0]);
}

void AbstractObject::MatScale(const glm::vec3 &scale) // matrice de translation
{
	this->scale = scale;

	GLuint matScale = glGetUniformLocation(program, "matScale");

	glm::mat4 scl;

	scl[0][0] = scale.x;	  scl[0][1] = 0.0f;			scl[0][2] = 0.0f;			scl[0][3] = 0.0f;
	scl[1][0] = 0.0f;		  scl[1][1] = scale.y;		scl[1][2] = 0.0f;			scl[1][3] = 0.0f;
	scl[2][0] = 0.0f;		  scl[2][1] = 0.0f;			scl[2][2] = scale.z;		scl[2][3] = 0.0f;
	scl[3][0] = 0.0f;		  scl[3][1] = 0.0f;			scl[3][2] = 0.0f;			scl[3][3] = 1.0f;

	glUniformMatrix4fv(matScale, 1, GL_FALSE, &scl[0][0]);
}

glm::vec3 AbstractObject::getRotation()
{
	return rotation;
}

glm::vec3 AbstractObject::getRotationDegree()
{
	return glm::degrees(rotation);
}

glm::quat AbstractObject::getRotationQuaternion()
{
	return glm::quat(rotation);
}

glm::vec3 AbstractObject::getTranslation()
{
	return translation;
}

glm::vec3 AbstractObject::getScale()
{
	return scale;
}

bool AbstractObject::isSelected()
{
	return selected;
}

void AbstractObject::setSelected(bool value)
{
	selected = value;
}

string AbstractObject::getName()
{
	return name;
}
