#include "PrimitiveObject.h"

void PrimitiveObject::uniformPrimitiveColor(const glm::vec4 &color)
{
	GLuint vecCouleur = glGetUniformLocation(program, "pColor");
	glUniform4fv(vecCouleur, 1, &color[0]);
}

void PrimitiveObject::Create(GLuint &program)
{
	Create(program, "Primitive");
}

void PrimitiveObject::Create(GLuint &Program, string Name)
{
	name = Name;
	program = Program;

	couleurRemplissage = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	couleurBordure = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	epaisseurBordure = 0;
	typePrimitive = GL_LINES;

	glGenVertexArrays(1, &vertexArray);
}

void PrimitiveObject::Draw(glm::mat4 &projection, glm::mat4 &view)
{
	glUseProgram(program);

	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatProjection = glGetUniformLocation(program, "matProjection");
	glUniformMatrix4fv(MatProjection, 1, GL_FALSE, &projection[0][0]);

	Draw();
}

void PrimitiveObject::Draw()
{
	glUseProgram(program);
	glBindVertexArray(vertexArray);

	uniformColor(program, color);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	MatRotationDegree(program, rotationDegree);
	MatTranslation(program, position);
	MatScale(program, scale);

	GLenum modeBordure = typePrimitive;

	// Dessiner intérieur
	if (typePrimitive != GL_POINTS && typePrimitive != GL_LINES)
	{
		uniformPrimitiveColor(couleurRemplissage);
		glDrawArrays(typePrimitive, 0, vertices.size());
		modeBordure = GL_LINE_LOOP;
	}

	// Dessiner bordure
	if (epaisseurBordure > 0)
	{
		if (typePrimitive == GL_POINTS)
			glPointSize((GLfloat)epaisseurBordure);
		else
			glLineWidth((GLfloat)epaisseurBordure);

		uniformPrimitiveColor(couleurBordure);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(modeBordure, 0, vertices.size());
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
	this->vertices = vertices;
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

void PrimitiveObject::setTypePrimitive(GLenum type)
{
	typePrimitive = type;
}
