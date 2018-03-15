#pragma once
#include"SBPyramidObject.h"

void SBPyramidObject::Create(GLuint &Program)
{
	name = "Pyramide";
	program = Program;

	GLfloat width, height, depth;
	width = 0.5;
	height = 0.5;
	depth = 0.5;

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	createForm(width, height, depth);

	glBindVertexArray(0);
}

void SBPyramidObject::createForm(GLfloat width, GLfloat height, GLfloat depth)
{
	createVertices(width / 2, height / 2, depth / 2);
	//creeTextureCube();

	//Création et remplissage du buffer d'indices de la base:
	GLuint IBOBase;
	glGenBuffers(1, &IBOBase);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOBase);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void SBPyramidObject::createVertices(GLfloat width, GLfloat height, GLfloat depth)
{
	//Remplir la variable m_sommetsBase
	vertices[0] = glm::vec3(0, 0 + height, 0);
	vertices[1] = glm::vec3(0 - width, 0 - height, 0 - depth);
	vertices[2] = glm::vec3(0 - width, 0 - height, 0 + depth);
	vertices[3] = glm::vec3(0 + width, 0 - height, 0 + depth);
	vertices[4] = glm::vec3(0 + width, 0 - height, 0 - depth);

	//Créer, remplir et activer le buffer de sommets
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}


void SBPyramidObject::Draw(glm::mat4 &projection, glm::mat4 &view)
{
	glUseProgram(program);
	uniformColor(program, color);
	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatProjection = glGetUniformLocation(program, "matProjection");
	glUniformMatrix4fv(MatProjection, 1, GL_FALSE, &projection[0][0]);
	
	MatRotationDegree(program, rotationDegree);
	MatTranslation(program, position);
	MatScale(program, scale);

	//Dessiner le cube
	glBindVertexArray(VertexArray);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

SBPyramidObject::~SBPyramidObject()
{
	glDeleteVertexArrays(1, &VertexArray);
}

