#pragma once
#include "ParametricCurveObject.h"
#include <functional>

class ParametricSurfaceObject : public AbstractObject
{
public:
	ParametricSurfaceObject();
	~ParametricSurfaceObject();

	enum PARAMETRICTYPE
	{
		BezierBicubic
	};

	void setNumLines(int numN,int numM);
	void setMatrix(glm::mat4 G);
	void setParametricType(PARAMETRICTYPE in_type);
	virtual void Create(GLuint &program);
	virtual void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights);
private:
	GLuint VertexArray;

	PARAMETRICTYPE type;
	int numLines;
	static  glm::vec3 bezierBicubic(glm::mat4 G, float u, float v);

	glm::mat4 matrix;
	std::vector<glm::vec3> vertices;
	int numN;
	int numM;

	std::vector<int> indices;
	
	void forNumLines(glm::mat4 G, int numN, int numM, std::function< glm::vec3(glm::mat4, float, float)> func);
	void fillIndices(unsigned int w);

};
