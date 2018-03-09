#pragma once
#include "AbstractObject.h"

class PrimitiveObject : public AbstractObject
{
private:
	GLuint program;
	GLuint vertexArray;
	glm::vec4 couleurRemplissage;
	glm::vec4 couleurBordure;
	int epaisseurBordure;
	GLenum typePrimitive;
	int nbVertex = 0;

public:
	virtual void Create(GLuint &program);
	virtual void Draw(glm::mat4 &perspective, glm::mat4 &view);
	~PrimitiveObject();

	void Create(GLuint &program, string name);
	void Draw();

	void setVertices(std::vector<glm::vec3> vertices);
	void setCouleurRemplissage(glm::vec4 couleur);
	void setCouleurBordure(glm::vec4 couleur);
	void setEpaisseurBordure(int valeur);
	void setTypePrimitive(GLenum type);
};
