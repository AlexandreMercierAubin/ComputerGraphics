#pragma once
#include"SkyboxObject.h"

void SkyboxObject::Create(GLuint &Program)
{
	program = Program;

	faces =  //default skybox bad because I made it myself and I hate copyright strikes
	{ "Resources/Skybox/HandMadeSky_bk.tga" ,//rt
		"Resources/Skybox/HandMadeSky_bk.tga" ,//lf
		"Resources/Skybox/HandMadeSky_up.tga" ,//up
		"Resources/Skybox/HandMadeSky_dn.tga",//dn
		"Resources/Skybox/HandMadeSky_bk.tga" ,//bk
		"Resources/Skybox/HandMadeSky_bk.tga" };//ft

	GLfloat width, height, depth;
	width = 100;
	height = 100;
	depth = 100;

	color=glm::vec4(1, 1, 1, 1);

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	createCube(width, height, depth);

	textureSkybox =  loadCubemap(faces, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER);

	glBindVertexArray(0);
}

void SkyboxObject::createCube(GLfloat width, GLfloat height, GLfloat depth)
{
	createVerticesCube(width / 2, height / 2,depth/2);
	//creeTextureCube();

	//Création et remplissage du buffer d'indices de la base:
	GLuint IBOBase;
	glGenBuffers(1, &IBOBase);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOBase);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void SkyboxObject::createVerticesCube(GLfloat width, GLfloat height, GLfloat depth)
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

GLuint SkyboxObject::loadCubemap(std::vector<char*> faces, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter)
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


void SkyboxObject::Draw(glm::mat4 &perspective, glm::mat4 &view)
{
	glUseProgram(program);
	uniformColor(program, color);
	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatPerspective = glGetUniformLocation(program, "matPerspective");
	glUniformMatrix4fv(MatPerspective, 1, GL_FALSE, &perspective[0][0]);

	//Changer le test de profondeur
	glDepthMask(GL_FALSE);

	//Passer la texture
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkybox);

	//Dessiner le cube
	glBindVertexArray(VertexArray);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
}