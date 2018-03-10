#pragma once
#include "AbstractObject.h"
#include "Model.h"

class QuadObject: public AbstractObject
{
private:
	GLuint program;
	GLuint VertexArray;
	GLuint textureID;
	glm::vec3 vertices[4];
	glm::vec4 color;
	int angle;
	const unsigned int indices[6] = {
		2,3,0,//face
		0,3,1
	};
	std::string texturePath;
	bool imageOK = false;
public:
	virtual void Create(GLuint &Program);
	virtual void Create(GLuint &Program, string typeSurFace);
	virtual void Draw(glm::mat4 &perspective, glm::mat4 &view);
	QuadObject(std::string texturePath);
	~QuadObject();
};
