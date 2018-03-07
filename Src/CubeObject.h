#pragma once
#include "AbstractObject.h"

class CubeObject : public AbstractObject
{
private:
	GLuint VertexArray;
	GLuint program;
	glm::vec3 vertices[8];
	glm::vec2 textureCube[8];

	
	

protected:
	void createVerticesCube(GLfloat width, GLfloat height, GLfloat depth);
	void createTextureCube();
	const unsigned int indices[36] = {
		0,1,3,//left
		0,2,3,
		5,7,6,//right
		5,4,6,
		0,1,5,//face
		0,4,5,
		2,3,7,//back
		2,6,7,
		1,3,7,//top
		1,5,7,
		0,2,6,//bottom
		0,4,6
	};
public:
	GLuint loadCubemap(std::vector<char*> faces, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
	virtual void Create(GLuint &Program);
	virtual void Draw(glm::mat4 &perspective, glm::mat4 &view);
	virtual void Delete();
	//virtual void SetColor();
};
