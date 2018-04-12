#pragma once
#include "ParametricCurveObject.h"
#include <functional>

class ParametricSurfaceObject : public AbstractObject
{
public:
	ParametricSurfaceObject();

	enum PARAMETRICTYPE
	{
		BezierBicubic
	};

	void setNumLines(int numN,int numM);
	void setVertices(std::vector<glm::vec3> &vN, std::vector<glm::vec3> &vM);
	void setParametricType(PARAMETRICTYPE in_type);
	virtual void Create(GLuint &program);
	virtual void Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights);
private:
	PARAMETRICTYPE type;
	int numLines;
	static unsigned int factorial(unsigned int n);
	static  glm::vec3 bezierBicubic(glm::vec3 position, std::vector<glm::vec3> verticesM, std::vector<glm::vec3> verticesN, float tn, float tm);


	std::vector<glm::vec3> vertices;
	
	void forNumLines(std::vector<glm::vec3>&verticesN, std::vector<glm::vec3>&verticesM, int num, std::function< glm::vec3(glm::vec3,vector<glm::vec3>, vector<glm::vec3>, float,float)> func);


};
