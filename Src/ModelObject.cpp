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

void ModelObject::Draw(glm::mat4 &projection, glm::mat4 &view , glm::vec3 &camPos, const vector<Light*>& lights)
{

	glUseProgram(program);
	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatProjection = glGetUniformLocation(program, "matProjection");
	glUniformMatrix4fv(MatProjection, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 r;
	AbstractObject::MatRotationDegree(program, r, rotationDegree);
	glm::mat4 t;
	AbstractObject::MatTranslation(program, t, position);
	glm::mat4 s;
	AbstractObject::MatScale(program, s, scale);

	glm::mat4 mModel = t * r* s;

	GLuint MatModel = glGetUniformLocation(program, "matModel");
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, &mModel[0][0]);

	AbstractObject::uniformColor(program, color);
	AbstractObject::uniformLight(program, lights);
	AbstractObject::uniformCameraPosition(program, camPos);
	AbstractObject::uniformShininess(program,shininess);

	model.Draw(program);
}

void ModelObject::setModelToCreate(string path)
{
	modelPath = path;
	name = "Modele (" + path + ")";
}
