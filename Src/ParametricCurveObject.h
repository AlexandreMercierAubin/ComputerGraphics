#pragma once
#include "PrimitiveObject.h"
#include <functional>

class ParametricCurveObject : public AbstractObject
{
public:
	ParametricCurveObject();

	enum PARAMETRICTYPE
	{
		BezierQuadratic,
		BezierCubic,
		CatmullRom,
		Hermite,
		Bezier
	};
	
	std::vector<PrimitiveObject> lines;

	void setNumLines(int num);
	void setVertices(std::vector<glm::vec3> vertices);
	void setFillColor(glm::vec4 color);
	void setEdgeColor(glm::vec4 color);
	void setEdgeSize(int value);
	void setParametricType(PARAMETRICTYPE in_type);
	virtual void Create(GLuint &program);
	virtual void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights);
private:
	PARAMETRICTYPE type;
	int numLines;
	static unsigned int factorial(unsigned int n);
	static  glm::vec3 bezier_quadratic(std::vector<glm::vec3> pt, float t);
	static  glm::vec3 bezier_cubic(std::vector<glm::vec3> pt, float t);
	static glm::vec3 hermite(std::vector<glm::vec3> pt, float t);
	static glm::vec3 bezier(std::vector<glm::vec3> pt, float t);
	static glm::vec3 catmullRom(std::vector<glm::vec3> pt, float t);


	
	void forNumLines(std::vector<glm::vec3>vertices, int num, std::function<glm::vec3(vector<glm::vec3>, float)> func);


};
