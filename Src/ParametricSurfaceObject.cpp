#include "ParametricSurfaceObject.h"

ParametricSurfaceObject::ParametricSurfaceObject()
{
	name = "Surface";
	type= BezierBicubic;

	matrix[0][0] = 0.1f;	    matrix[0][1] = 0.0f;			matrix[0][2] = 0.0f;		matrix[0][3] = 0.0f;
	matrix[1][0] = 0.0f;		matrix[1][1] = 0.1f;			matrix[1][2] = 0.0f;		matrix[1][3] = 0.0f;
	matrix[2][0] = 0.0f;		matrix[2][1] = 0.0f;			matrix[2][2] = 0.1f;		matrix[2][3] = 0.0f;
	matrix[3][0] = 0.0f;		matrix[3][1] = 0.0f;			matrix[3][2] = 0.0f;		matrix[3][3] = 1.0f;

	setNumLines(10, 10);
	forNumLines(matrix, numN, numM, bezierBicubic);
	fillIndices(numM);
}

ParametricSurfaceObject::~ParametricSurfaceObject()
{
	glDeleteVertexArrays(1, &VertexArray);
}

void ParametricSurfaceObject::setNumLines(int in_numN, int in_numM)
{
	numN = in_numN;
	numM = in_numM;
}

void ParametricSurfaceObject::setMatrix(glm::mat4 G)
{
	matrix = G;
	forNumLines(matrix, numN, numM, bezierBicubic);
	fillIndices(numM);
	Create(program);
}

void ParametricSurfaceObject::setParametricType(PARAMETRICTYPE in_type)
{

}

void ParametricSurfaceObject::Create(GLuint & in_program)
{
	program = in_program;

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);


	GLuint vertexbuffer;//binding vertices
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//binding indices
	GLuint IBOBase;
	glGenBuffers(1, &IBOBase);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOBase);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void ParametricSurfaceObject::Draw(glm::mat4 & projection, glm::mat4 & view, glm::vec3 & camPos, const vector<Light*>& lights)
{
	

	glUseProgram(program);
	glDisable(GL_CULL_FACE);

	uniformColor(program, color);
	uniformFog(program, useFog);
	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatProjection = glGetUniformLocation(program, "matProjection");
	glUniformMatrix4fv(MatProjection, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 r;
	MatRotationDegree(program, r, rotationDegree);
	glm::mat4 t;
	MatTranslation(program, t, position);
	glm::mat4 s;
	MatScale(program, s, scale);

	glm::mat4 model = t * r* s;

	GLuint MatModel = glGetUniformLocation(program, "matModel");
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, &model[0][0]);

	//Dessiner le cube
	glBindVertexArray(VertexArray);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
}


glm::vec3 ParametricSurfaceObject::bezierBicubic( glm::mat4 G,float u, float v)
{

	glm::vec4 U = glm::vec4(1.0, u, pow(u, 2), pow(u, 3));

	glm::mat4 matrix;
	matrix[0][0] = 1.0f;	    matrix[0][1] = 0.0f;			matrix[0][2] = 0.0f;		matrix[0][3] = 0.0f;
	matrix[1][0] = -3.0f;		matrix[1][1] = 3.0f;			matrix[1][2] = 0.0f;		matrix[1][3] = 0.0f;
	matrix[2][0] = 3.0f;		matrix[2][1] = -6.0f;			matrix[2][2] = 3.0f;		matrix[2][3] = 0.0f;
	matrix[3][0] = -1.0f;		matrix[3][1] = 3.0f;			matrix[3][2] = -3.0f;		matrix[3][3] = 1.0f;


	glm::vec4 V = glm::vec4(1.0, v, pow(v, 2), pow(v, 3));

	return U* glm::transpose(matrix)*G*matrix*V;
}

void ParametricSurfaceObject::forNumLines(glm::mat4 G, int numN,int numM, std::function< glm::vec3(glm::mat4,float, float)> func)
{
	for (int i = 0; i < numN; ++i) 
	{
		for (int j = 0; j < numM; ++j) 
		{
			vertices.push_back(func( G, (float)i / numN, (float)j / numM));
		}
	}
}

void ParametricSurfaceObject::fillIndices(unsigned int w)
{
	unsigned int h = vertices.size() / w;
	if (w *h != vertices.size()) 
	{
		cout << "w ise not set correctly"<<endl;
		return;
	}

	for (unsigned int i = 0; i < vertices.size(); ++i) 
	{
		if (i < w * (h - 1) && (i + 1) % w) 
		{
			indices.push_back(i);//adds the first triangle
			indices.push_back(i + 1);
			indices.push_back(i + w);

			indices.push_back(i+1);//adds the second triangle
			indices.push_back(i + 1 + w);
			indices.push_back(i + w);
		}
	}
}