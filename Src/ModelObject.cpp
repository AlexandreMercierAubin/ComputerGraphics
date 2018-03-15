#pragma once
#include "ModelObject.h"



void ModelObject::Create(GLuint &Program)
{
	name = "Modele (" + modelPath + ")";

	program = Program;

	//default settings
	scale = glm::vec3(0.0028f, 0.0028f, 0.0028f);
	model = Model(modelPath);

}

void ModelObject::Draw(glm::mat4 &projection, glm::mat4 &view)
{

	glUseProgram(program);
	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatProjection = glGetUniformLocation(program, "matProjection");
	glUniformMatrix4fv(MatProjection, 1, GL_FALSE, &projection[0][0]);

	MatRotationDegree(program,rotationDegree);//roll*pitch*yaw matrix with angles x,y,z
	MatTranslation(program,position);
	MatScale(program,scale);

	AbstractObject::uniformColor(program, color);
	AbstractObject::uniformLight(program, glm::vec3(1.0, 1.0, 1.0), glm::vec3(0,0,1), ambientIntensity, diffuseIntensity);

	model.Draw(program);
}

void ModelObject::setModelToCreate(string path)
{
	modelPath = path;
	name = "Modele (" + path + ")";
}
