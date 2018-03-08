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

void PrimitiveObject::Draw(glm::mat4 &perspective, glm::mat4 &view)
{
	glUseProgram(program);

	GLuint MatView = glGetUniformLocation(program, "matView");
	glUniformMatrix4fv(MatView, 1, GL_FALSE, &view[0][0]);
	GLuint MatPerspective = glGetUniformLocation(program, "matPerspective");
	glUniformMatrix4fv(MatPerspective, 1, GL_FALSE, &perspective[0][0]);

	Draw();
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
			glPointSize((GLfloat)epaisseurBordure);
		else
			glLineWidth((GLfloat)epaisseurBordure);

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
