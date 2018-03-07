#pragma once
#include"QuadObject.h"

void QuadObject::Create(GLuint &Program)
{
	program = Program;
	GLfloat width, height, depth;
	width = 0.5;
	height = 0.5;
	depth = 0;

	glBindVertexArray(VertexArray);

	vertices[0] = glm::vec3(0 - width, 0 - height, 0 - depth);
	vertices[1] = glm::vec3(0 - width, 0 + height, 0 - depth);
	vertices[2] = glm::vec3(0 + width, 0 + height, 0 - depth);
	vertices[3] = glm::vec3(0 + width, 0 - height, 0 - depth);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void QuadObject::Draw(glm::mat4 &perspective, glm::mat4 &view)
{
	glUseProgram(program);
	glBindVertexArray(VertexArray);
	glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void QuadObject::Delete()
{
	glDeleteVertexArrays(1, &VertexArray);
}