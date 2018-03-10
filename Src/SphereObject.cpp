#pragma once
#include"SphereObject.h"

void SphereObject::Create(GLuint &Program)
{
	name = "Cube";
	program = Program;

	GLfloat width, height, depth;
	width = 0.5;
	height = 0.5;
	depth = 0.5;

	color = glm::vec4(1, 0, 1, 1);

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	createCube(width, height, depth);

	glBindVertexArray(0);
}

void SphereObject::createCube(GLfloat width, GLfloat height, GLfloat depth)
{
	createVerticesCube(width / 2, height / 2, depth / 2);
	//creeTextureCube();

	//Création et remplissage du buffer d'indices de la base:
	GLuint IBOBase;
	glGenBuffers(1, &IBOBase);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOBase);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void SphereObject::createVerticesCube(GLfloat width, GLfloat height, GLfloat depth)
{
	//Remplir la variable m_sommetsBase
	vertices[0] = glm::vec3(0 - width, 0 - height, 0 - depth);
	vertices[1] = glm::vec3(0 - width, 0 + height, 0 - depth);
	vertices[2] = glm::vec3(0 - width, 0 - height, 0 + depth);
	vertices[3] = glm::vec3(0 - width, 0 + height, 0 + depth);
	vertices[4] = glm::vec3(0 + width, 0 - height, 0 - depth);
	vertices[5] = glm::vec3(0 + width, 0 + height, 0 - depth);
	vertices[6] = glm::vec3(0 + width, 0 - height, 0 + depth);
	vertices[7] = glm::vec3(0 + width, 0 + height, 0 + depth);

	//Créer, remplir et activer le buffer de sommets
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}


void SphereObject::Draw(glm::mat4 &perspective, glm::mat4 &view)
{
	glUseProgram(program);
	uniformColor(program, color);
	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatPerspective = glGetUniformLocation(program, "matPerspective");
	glUniformMatrix4fv(MatPerspective, 1, GL_FALSE, &perspective[0][0]);
	
	MatRotation(program, glm::vec3(0, 0, 0));
	MatTranslation(program, glm::vec3(0, 0, 0));
	MatScale(program, glm::vec3(1, 1, 1));

	//Dessiner le cube
	glBindVertexArray(VertexArray);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

SphereObject::~SphereObject()
{
	glDeleteVertexArrays(1, &VertexArray);
}

