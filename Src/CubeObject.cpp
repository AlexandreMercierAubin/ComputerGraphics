#pragma once
#include"CubeObject.h"

void CubeObject::Create(GLuint &Program)
{
	program = Program;
	GLfloat width, height, depth;
	width = 0.5;
	height = 0.5;
	depth = 0.5;

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	createVerticesCube(width / 2, height / 2,depth/2);

	//Création et remplissage du buffer d'indices de la base:
	GLuint IBOBase;
	glGenBuffers(1, &IBOBase);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOBase);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void CubeObject::createVerticesCube(GLfloat width, GLfloat height, GLfloat depth)
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

void CubeObject::createTextureCube() // not used yet, might be useful tho
{
	//remplissage de la variable m_texture
	textureCube[0] = glm::vec2(0.0f, 0.0f);
	textureCube[1] = glm::vec2(0.0f, 1.0f);
	textureCube[2] = glm::vec2(1.0f, 0.0f);
	textureCube[3] = glm::vec2(1.0f, 1.0f);
	textureCube[4] = glm::vec2(1.0f, 0.0f);
	textureCube[5] = glm::vec2(1.0f, 1.0f);
	textureCube[6] = glm::vec2(0.0f, 0.0f);
	textureCube[7] = glm::vec2(0.0f, 1.0f);

	//Créer, remplir et activer le buffer de texture
	GLuint textureBuffer;
	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCube), textureCube, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
}

void CubeObject::Draw(glm::mat4 &perspective, glm::mat4 &view)
{
	glUseProgram(program);
	glBindVertexArray(VertexArray);
	glDrawElements(GL_TRIANGLES, 46 , GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

GLuint CubeObject::loadCubemap(std::vector<char*> faces, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter)
{
	GLuint textureID;
	SDL_Surface* image;
	GLint channels;
	GLenum type;

	//Générer la texture
	glGenTextures(1, &textureID);

	//Lier la texture à la cible texture 2D
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	//wrapping 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//filtering 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	for (GLuint i = 0; i < faces.size(); i++)
	{

		image = Model::loadImage(string(faces[i]));
		Model::getImageProperties(image, channels, type);

		//Associer l'information de l'image à la texture
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, channels, image->w, image->h, 0, type, GL_UNSIGNED_BYTE, image->pixels);
		//Libérer l'image
		SDL_FreeSurface(image);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0); //Délie la texture pour éviter d'être corrompue avec une autre

	return textureID; //retourne l'objet texture créé
}