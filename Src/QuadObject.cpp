#pragma once
#include"QuadObject.h"

void QuadObject::Create(GLuint &Program)
{
	program = Program;
	GLfloat width, height, depth;
	width = 0.5f;
	height = 0.5f;
	depth = 0.001f;
	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	vertices[0] = glm::vec3(0 - width/2, 0 - height/2, 0 - depth/2);
	vertices[1] = glm::vec3(0 + width/2, 0 - height/2, 0 - depth/2);
	vertices[2] = glm::vec3(0 - width/2, 0 + height/2, 0 - depth/2);
	vertices[3] = glm::vec3(0 + width/2, 0 + height/2, 0 - depth/2);

	//temp normal
	glm::vec3 normal= glm::vec3(0,0,1);
	glm::vec3 normalBuffer[4] = { normal ,normal,normal ,normal };
	glm::vec2 vTexture[4] = { glm::vec2(0.0f, 0.0f)  , glm::vec2(width, 0.0f), glm::vec2(0.0f, height) , glm::vec2(width, height) };


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//color buffer could be remove if there is a texure
	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalBuffer), normalBuffer, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	GLuint textureBuffer;
	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vTexture), vTexture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLint channels;
	GLenum type;
	SDL_Surface *image = Model::loadImage("Resources/Skybox/HandMadeSky_up.tga");
	Model::getImageProperties(image, channels, type);

	// Assign texture to ID
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, channels, image->w, image->h, 0, type, GL_UNSIGNED_BYTE, image->pixels);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Free image
	SDL_FreeSurface(image);

	glBindVertexArray(0);
}

void QuadObject::Draw(glm::mat4 &perspective, glm::mat4 &view)
{
	glUseProgram(program);

	MatTranslation(program, glm::vec3(0.0f, 0.0f, 0.0f));
	MatScale(program, glm::vec3(1, 1, 1));
	uniformColor(program, color);

	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatPerspective = glGetUniformLocation(program, "matPerspective");
	glUniformMatrix4fv(MatPerspective, 1, GL_FALSE, &perspective[0][0]);

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(program, "text"), 0);

	glBindVertexArray(VertexArray);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void QuadObject::Delete()
{
	glDeleteVertexArrays(1, &VertexArray);
}