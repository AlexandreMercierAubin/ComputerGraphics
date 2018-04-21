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
	int angle;
	const unsigned int indices[6] = {
		2,3,0,//face
		0,3,1
	};
	std::string texturePath;
	std::string texturePath2;
	bool imageOK = false;
public:
	virtual void Create(GLuint &Program);
	virtual void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights);
	QuadObject(std::string texturePath);
	QuadObject(std::string texturePath,std::string texturePath2);
	~QuadObject();
};
