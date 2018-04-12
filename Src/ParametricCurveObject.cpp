#include "ParametricCurveObject.h"


void ParametricCurveObject::forNumLines(std::vector<glm::vec3> &vertices, int num, std::function<glm::vec3(vector<glm::vec3>, float)> func)
{
	lines.clear();

	glm::vec3 lastPoint= func(vertices, 0.0f);
	for (int i = 1; i<num; ++i) {
		glm::vec3 point = func(vertices, (float)i / num);
		
		lines.push_back(PrimitiveObject());
		lines[lines.size() - 1].Create(program);

		std::vector<glm::vec3> line;
		line.push_back(lastPoint);
		line.push_back(point);

		lines[lines.size() - 1].setVertices(line);

		lines[lines.size() - 1].setTypePrimitive(GL_LINES);

		lastPoint = point;
	}

}
glm::vec3 ParametricCurveObject::bezier_quadratic(std::vector<glm::vec3> pt, float t)//3 points needed
{
	float u = 1 - t;
	glm::vec3 returnPt;
	returnPt.x = u * (u * pt[0].x + t * pt[1].x) + t * (u * pt[1].x + t * pt[2].x);
	returnPt.y = u * (u * pt[0].y + t * pt[1].y) + t * (u * pt[1].y + t * pt[2].y);
	returnPt.z = u * (u * pt[0].z + t * pt[1].z) + t * (u * pt[1].z + t * pt[2].z);
	return returnPt;
}

glm::vec3 ParametricCurveObject::bezier_cubic(std::vector<glm::vec3> pt, float t)//4 points needed
{
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	glm::vec3 returnPt;
	returnPt.x = uuu * pt[0].x + 3 * uu * t * pt[1].x + 3 * u * tt * pt[2].x + ttt * pt[3].x;
	returnPt.y = uuu * pt[0].y + 3 * uu * t * pt[1].y + 3 * u * tt * pt[2].y + ttt * pt[3].y;
	returnPt.z = uuu * pt[0].z + 3 * uu * t * pt[1].z + 3 * u * tt * pt[2].z + ttt * pt[3].z;
	return returnPt;
}

glm::vec3 ParametricCurveObject::hermite(std::vector<glm::vec3> pt, float t)//4 pts needed
{
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	glm::vec3 returnPt;
	returnPt.x = (2 * ttt - 3 * tt + 1) * pt[0].x + (ttt - 2 * tt + t) * pt[1].x + (ttt - tt) * pt[2].x + (-2 * ttt + 3 * tt) * pt[3].x;
	returnPt.y = (2 * ttt - 3 * tt + 1) * pt[0].y + (ttt - 2 * tt + t) * pt[1].y + (ttt - tt) * pt[2].y + (-2 * ttt + 3 * tt) * pt[3].y;
	returnPt.z = (2 * ttt - 3 * tt + 1) * pt[0].z + (ttt - 2 * tt + t) * pt[1].z + (ttt - tt) * pt[2].z + (-2 * ttt + 3 * tt) * pt[3].z;
	return returnPt;
}

unsigned int ParametricCurveObject::factorial(unsigned int n)
{
	unsigned int value = 1;
	for (int i = n; i > 1; --i)
		value = value * i;
	return value;
}

glm::vec3 ParametricCurveObject::bezier(std::vector<glm::vec3> pt, float t)// would work approximately up to 20 before running out of memory
{
	unsigned int n = pt.size();
	glm::vec3 point=glm::vec3(0,0,0);
	for (unsigned int i = 0; i < n; ++i) 
	{
		double a = (factorial(n) / (factorial(i)*factorial(n - i)))*pow(t, i)*pow(1 - t, n - i);
		point.x += (float)(a * pt[i].x);
		point.y += (float)(a * pt[i].y);
		point.z += (float)(a * pt[i].z);
	}
	return point;
}

glm::vec3 ParametricCurveObject::catmullRom(std::vector<glm::vec3> pt, float t) // 4 pts needed
{


	glm::mat3x4 pMatrix;
	pMatrix[0][0] = pt[0].x;	    pMatrix[0][1] = pt[1].x;			pMatrix[0][2] = pt[2].x;		pMatrix[0][3] = pt[3].x;
	pMatrix[1][0] = pt[0].y;		pMatrix[1][1] = pt[1].y;			pMatrix[1][2] = pt[2].y;		pMatrix[1][3] = pt[3].y;
	pMatrix[2][0] = pt[0].z;		pMatrix[2][1] = pt[1].z;			pMatrix[2][2] = pt[2].z;		pMatrix[2][3] = pt[3].z;

	glm::mat4 matrix;
	matrix[0][0] = 0.0f;	    matrix[0][1] = -1.0f;			matrix[0][2] = 2.0f;		matrix[0][3] = -1.0f;
	matrix[1][0] = 2.0f;		matrix[1][1] = 0.0f;			matrix[1][2] = -5.0f;		matrix[1][3] = 3.0f;
	matrix[2][0] = 0.0f;		matrix[2][1] = 1.0f;			matrix[2][2] = 4.0f;		matrix[2][3] = -3.0f;
	matrix[3][0] = 0.0f;		matrix[3][1] = 0.0f;			matrix[3][2] = -1.0f;		matrix[3][3] = 1.0f;

	glm::mat4 sc= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));;

	glm::vec4 ts=glm::vec4(1, t, pow(t, 2), pow(t, 3));

	glm::vec3 point = transpose(pMatrix)*sc *matrix*ts;

	return point;
}

void ParametricCurveObject::Create(GLuint & in_program)
{
	program = in_program;
	for (unsigned int i = 0; i < lines.size(); ++i)
	{
		lines[i].Create(program);
	}
}

ParametricCurveObject::ParametricCurveObject()
{
	name = "parametric curve";
	numLines = 16;
	type = BezierCubic;
}

void ParametricCurveObject::setNumLines(int num)
{
	numLines = num;
}

void ParametricCurveObject::setVertices(std::vector<glm::vec3> &vertices)//must be executed first
{
	switch (type) 
	{
	case BezierQuadratic:
		if (vertices.size() >= 3)
		{
			forNumLines(vertices, numLines+1,bezier_quadratic);
			return;
		}
		break;
	case BezierCubic:
		if (vertices.size() >= 4)
		{
			forNumLines(vertices, numLines+1, bezier_cubic);
			return;
		}
		break;
	case CatmullRom:
		if (vertices.size() >= 4)
		{
			forNumLines(vertices, numLines + 1, catmullRom);
			return;
		}
		break;
	case Hermite:
		if (vertices.size() >= 4)
		{
			forNumLines(vertices, numLines+1, hermite);
			return;
		}
		break;
	case Bezier:
		if (vertices.size() >= 2)
		{
			forNumLines(vertices, numLines + 1, bezier);
			return;
		}
		break;
	}
	cout << "not enough vertices, cancelling the draw call" << endl;
}

void ParametricCurveObject::setFillColor(glm::vec4 color) 
{
	for (unsigned int i = 0; i < lines.size(); ++i)
	{
		lines[i].setCouleurRemplissage(color);
	}
}
void ParametricCurveObject::setEdgeColor(glm::vec4 color) 
{
	for (unsigned int i = 0; i < lines.size(); ++i)
	{
		lines[i].setCouleurBordure(color);
	}
}
void ParametricCurveObject::setEdgeSize(int value) 
{
	for (unsigned int i = 0; i < lines.size(); ++i)
	{
		lines[i].setEpaisseurBordure(value);
	}
}
void ParametricCurveObject::setParametricType(PARAMETRICTYPE in_type) 
{
	type = in_type;
	//must setup  or add validation not to crash
}

void ParametricCurveObject::Draw(glm::mat4 & projection, glm::mat4 & view, glm::vec3 & camPos, const vector<Light*>& lights)
{
	for (unsigned int i = 0; i < lines.size(); ++i) 
	{
		lines[i].Draw(projection,view,camPos,lights);
	}
}




