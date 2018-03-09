#pragma once
#include"QuadObject.h"

void QuadObject::Create(GLuint &Program)
{
	if (!imageOK)
	{
		cout << "Erreur au chargement de l'image" << endl;
		return;
	}

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

	GLint channels;
	GLenum type;
	SDL_Surface *image = Model::loadImage(texturePath);
	Model::getImageProperties(image, channels, type);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, channels, image->w, image->h, 0, type, GL_UNSIGNED_BYTE, image->pixels);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	//Free image
	SDL_FreeSurface(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
}

void QuadObject::Draw(glm::mat4 &perspective, glm::mat4 &view)
{
	if (!imageOK)
		return;

	glUseProgram(program);

	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatPerspective = glGetUniformLocation(program, "matPerspective");
	glUniformMatrix4fv(MatPerspective, 1, GL_FALSE, &perspective[0][0]);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glm::vec2 vTexture[4] = { glm::vec2(0.0f, 1.0f)  , glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f) , glm::vec2(1.0f, 0.0f) };

	GLuint textureBuffer;
	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vTexture), vTexture, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(VertexArray);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(textureID, "text"), 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

}


QuadObject::QuadObject(std::string texturePath)
{
	name = "Image (" + texturePath + ")";

	this->texturePath = texturePath;
	SDL_Surface *image = Model::loadImage(texturePath);
	imageOK = image != nullptr;
	SDL_FreeSurface(image);
}

QuadObject::~QuadObject()
{
	glDeleteVertexArrays(1, &VertexArray);
}
