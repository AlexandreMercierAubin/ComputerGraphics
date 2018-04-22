#pragma once
#include"TessellationQuad.h"


void TessellationQuad::Create(GLuint &Program)
{
	name = "SurfaceTessellation";;
	if (!imageOK)
	{
		cout << "Erreur au chargement de l'image" << endl;
		return;
	}

	program = Program;
	glUseProgram(program);
	GLfloat width, height, depth;
	width = 0.5f;
	height = 0.5f;
	depth = 0.001f;
	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	vertices[0] = glm::vec3(0 - width / 2, 0 - height / 2, 0 - depth / 2);
	vertices[1] = glm::vec3(0 + width / 2, 0 - height / 2, 0 - depth / 2);
	vertices[2] = glm::vec3(0 - width / 2, 0 + height / 2, 0 - depth / 2);
	vertices[3] = glm::vec3(0 + width / 2, 0 + height / 2, 0 - depth / 2);

	

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

void TessellationQuad::Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights)
{
	if (!imageOK)
		return;

	glUseProgram(program);
	glDisable(GL_CULL_FACE);
	AbstractObject::uniformColor(program, color);

	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatProjection = glGetUniformLocation(program, "matProjection");
	glUniformMatrix4fv(MatProjection, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 r;
	MatRotationDegree(program, r, rotationDegree);
	glm::mat4 t;
	MatTranslation(program, t, position);
	glm::mat4 s;
	MatScale(program, s, scale);

	glm::mat4 model = t * r* s;

	GLuint MatModel = glGetUniformLocation(program, "matModel");
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, &model[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(textureID, "text"), 0);
	glBindVertexArray(VertexArray);
	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_CULL_FACE);
	

}


TessellationQuad::TessellationQuad(std::string TexturePath)
{
	name = "Image (" + TexturePath + ")";

	texturePath = TexturePath;
	SDL_Surface *image = Model::loadImage(TexturePath);
	imageOK = image != nullptr;
	SDL_FreeSurface(image);
}
TessellationQuad::TessellationQuad(std::string TexturePath, std::string TexturePath2)
{
	name = "Image (" + TexturePath + ")";

	texturePath = TexturePath;
	texturePath2 = TexturePath2;
	SDL_Surface *image = Model::loadImage(TexturePath);
	imageOK = image != nullptr;
	SDL_FreeSurface(image);
}

TessellationQuad::~TessellationQuad()
{
	glDeleteVertexArrays(1, &VertexArray);
}
