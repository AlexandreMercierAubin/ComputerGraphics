#pragma once
#include "ModelObject.h"

void ModelObject::Create(GLuint &Program)
{
	program = Program;
	color = glm::vec4(1, 1, 1, 1);
}

void ModelObject::Draw(glm::mat4 &perspective, glm::mat4 &view)
{
	glUseProgram(program);
	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatPerspective = glGetUniformLocation(program, "matPerspective");
	glUniformMatrix4fv(MatPerspective, 1, GL_FALSE, &perspective[0][0]);


	//AbstractObject::MatRotation(program, 0);
	//AbstractObject::uniformColor(program, color);
	//AbstractObject::uniformLight(program, glm::vec3(1.0, 1.0, 1.0), glm::vec3(0,0,1), 0.3f, lightIntensity);


	////model
	//AbstractObject::MatTranslation(program, (matTranslation).x, (matTranslation).y, (matTranslation).z);
	//AbstractObject::MatScale(program, (matScale).x, (matScale).y, (matScale).z);
	//model.Draw(program);
}

void ModelObject::Delete() 
{

}