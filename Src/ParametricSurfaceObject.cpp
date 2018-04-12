#include "ParametricSurfaceObject.h"

void ParametricSurfaceObject::setNumLines(int numN, int numM)
{
}

void ParametricSurfaceObject::setVertices(std::vector<glm::vec3> &vN, std::vector<glm::vec3> &vM)
{
}

void ParametricSurfaceObject::setParametricType(PARAMETRICTYPE in_type)
{
}

void ParametricSurfaceObject::Create(GLuint & program)
{
}

void ParametricSurfaceObject::Draw(glm::mat4 & projection, glm::mat4 & view, glm::vec3 & camPos, const vector<Light*>& lights)
{
}

unsigned int ParametricSurfaceObject::factorial(unsigned int n)
{
	return 0;
}

glm::vec3 ParametricSurfaceObject::bezierBicubic(glm::vec3 position,std::vector<glm::vec3> verticesM, std::vector<glm::vec3> verticesN, float tn, float tm)
{
	unsigned int n = verticesN.size();
	unsigned int m = verticesM.size();
	glm::vec3 point=position;
	for (unsigned int i = 0; i < n; ++i)
	{
		for (unsigned int j = 0; j < m; ++j)
		{
			
			point+=ParametricCurveObject::bezier_cubic(verticesN,(float) i/n)* ParametricCurveObject::bezier_cubic(verticesM, (float)j/m)*position;
			//bezier*bezier*Pij

		}
	}
	return point;
}

void ParametricSurfaceObject::forNumLines(std::vector<glm::vec3>&verticesN, std::vector<glm::vec3>&verticesM, int num, std::function< glm::vec3(glm::vec3, vector<glm::vec3>, vector<glm::vec3>, float, float)> func)
{
	for (int i = 0; i < num; ++i) 
	{
		for (int j = 0; j < num; ++j) 
		{
			vertices.push_back(func(position, verticesN, verticesM, (float)i / num, (float)j / num));
		}
	}
}
