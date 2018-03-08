#include "PrimitiveObject.h"

void PrimitiveObject::Create(GLuint &program)
{
	this->program = program;

	couleurRemplissage = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	couleurBordure = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	epaisseurBordure = 0;
	typePrimitive = GL_LINES;

	glGenVertexArrays(1, &vertexArray);
}

void PrimitiveObject::Draw()
{
	glUseProgram(program);
	glBindVertexArray(vertexArray);

	GLenum modeBordure = typePrimitive;

	// Dessiner intérieur
	if (typePrimitive != GL_POINTS && typePrimitive != GL_LINES)
	{
		uniformColor(program, couleurRemplissage);
		glDrawArrays(typePrimitive, 0, nbVertex);
		modeBordure = GL_LINE_LOOP;
	}

	// Dessiner bordure
	if (epaisseurBordure > 0)
	{
		if (typePrimitive == GL_POINTS)
			glPointSize(epaisseurBordure);
		else
			glLineWidth(epaisseurBordure);

		uniformColor(program, couleurBordure);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(modeBordure, 0, nbVertex);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glBindVertexArray(0);
}

PrimitiveObject::~PrimitiveObject()
{
	glDeleteVertexArrays(1, &vertexArray);
}

void PrimitiveObject::setVertices(std::vector<glm::vec3> vertices)
{
	nbVertex = vertices.size();

	//Créer, remplir et activer le buffer de sommets
	glBindVertexArray(vertexArray);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, nbVertex * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void PrimitiveObject::setCouleurRemplissage(glm::vec4 couleur)
{
	couleurRemplissage = couleur;
}

void PrimitiveObject::setCouleurBordure(glm::vec4 couleur)
{
	couleurBordure = couleur;
}

void PrimitiveObject::setEpaisseurBordure(int valeur)
{
	epaisseurBordure = valeur;
}

void PrimitiveObject::setTypePrimitive(GLenum valeur)
{
	typePrimitive = valeur;
}
