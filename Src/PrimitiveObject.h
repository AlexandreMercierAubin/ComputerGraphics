#pragma once
#include "AbstractObject.h"

class PrimitiveObject : public AbstractObject
{
private:
	GLuint vertexArray;
	glm::vec4 couleurRemplissage;
	glm::vec4 couleurBordure;
	int epaisseurBordure;
	GLenum typePrimitive;
	std::vector<glm::vec3> vertices;

	void uniformPrimitiveColor(const glm::vec4 &color);

public:
	virtual void Create(GLuint &program);
	virtual void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights);
	~PrimitiveObject();

	void Create(GLuint &program, string name);
	void Draw();

	void setVertices(std::vector<glm::vec3> vertices);
	void setCouleurRemplissage(glm::vec4 couleur);
	void setCouleurBordure(glm::vec4 couleur);
	void setEpaisseurBordure(int valeur);
	void setTypePrimitive(GLenum type);
};
