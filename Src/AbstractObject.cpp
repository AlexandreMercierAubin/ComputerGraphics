#pragma once
#include "AbstractObject.h"


void AbstractObject::uniformColor(GLuint &program, glm::vec4 &uniformColor)
{
	GLuint vecCouleur = glGetUniformLocation(program, "Color");
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

void AbstractObject::MatRotation(GLuint program, float angle) // matrice de rotation
{
	GLuint MatRotation = glGetUniformLocation(program, "matRotation");

	glm::mat4 rotat;// rotation Y

	rotat[0][0] = cosf(angle); rotat[0][1] = 0.0f; rotat[0][2] = sinf(angle); rotat[0][3] = 0.0f;
	rotat[1][0] = 0.0f; rotat[1][1] = 1.0f; rotat[1][2] = 0.0f; rotat[1][3] = 0.0f;
	rotat[2][0] = -sinf(angle); rotat[2][1] = 0.0f; rotat[2][2] = cosf(angle); rotat[2][3] = 0.0f;
	rotat[3][0] = 0.0f; rotat[3][1] = 0.0f; rotat[3][2] = 0.0f; rotat[3][3] = 1.0f;

	glUniformMatrix4fv(MatRotation, 1, GL_TRUE, &rotat[0][0]);
}

void AbstractObject::MatTranslation(GLuint program, float x, float y, float z) // matrice de translation
{
	GLuint MatTrans = glGetUniformLocation(program, "matTranslation");

	glm::mat4 trans;

	trans = glm::translate(trans, glm::vec3(x, y, z));

	glUniformMatrix4fv(MatTrans, 1, GL_FALSE, &trans[0][0]);
}

void AbstractObject::MatScale(GLuint program, float x, float y, float z) // matrice de translation
{
	GLuint MatScale = glGetUniformLocation(program, "matScale");

	glm::mat4 scl;

	scl = glm::scale(scl, glm::vec3(x, y, z));

	glUniformMatrix4fv(MatScale, 1, GL_FALSE, &scl[0][0]);
}