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

	virtual void setVertices(const std::vector<glm::vec3> & vertices);
	virtual void setCouleurRemplissage(glm::vec4 couleur);
	virtual void setCouleurBordure(glm::vec4 couleur);
	virtual void setEpaisseurBordure(int valeur);
	virtual void setTypePrimitive(GLenum type);
};
