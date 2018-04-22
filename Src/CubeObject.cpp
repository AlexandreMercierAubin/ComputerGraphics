#pragma once
#include"CubeObject.h"

void CubeObject::Create(GLuint &Program)
{
	name = "Cube";
	program = Program;

	GLfloat width, height, depth;
	width = 0.5;
	height = 0.5;
	depth = 0.5;

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	createForm(width, height, depth);

	glBindVertexArray(0);
}

void CubeObject::createForm(GLfloat width, GLfloat height, GLfloat depth)
{
	createVertices(width / 2, height / 2, depth / 2);
	//creeTextureCube();

	//Création et remplissage du buffer d'indices de la base:
	GLuint IBOBase;
	glGenBuffers(1, &IBOBase);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOBase);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void CubeObject::createVertices(GLfloat width, GLfloat height, GLfloat depth)
{
	//Remplir la variable m_sommetsBase
	vertices[0] = glm::vec3(0 - width, 0 - height, 0 - depth);
	vertices[1] = glm::vec3(0 - width, 0 + height, 0 - depth);
	vertices[2] = glm::vec3(0 - width, 0 - height, 0 + depth);
	vertices[3] = glm::vec3(0 - width, 0 + height, 0 + depth);
	vertices[4] = glm::vec3(0 + width, 0 - height, 0 - depth);
	vertices[5] = glm::vec3(0 + width, 0 + height, 0 - depth);
	vertices[6] = glm::vec3(0 + width, 0 - height, 0 + depth);
	vertices[7] = glm::vec3(0 + width, 0 + height, 0 + depth);

	//Créer, remplir et activer le buffer de sommets
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}


void CubeObject::Draw(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &camPos, const vector<Light*>& lights)
{
	glUseProgram(program);
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
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

bool CubeObject::raycast(const Ray &ray, double &distance, glm::vec3 &normal, std::shared_ptr<AbstractObject> &object)
{
	bool intersect = false;

	// Boucle sur les différentes faces
	for (int i = 0; i < 6; ++i)
	{
		int i1 = indices[i * 6];
		int i2 = indices[i * 6 + 1];
		int i3 = indices[i * 6 + 2];

		glm::vec3 v[4];
		v[0] = vertices[i1];
		v[1] = vertices[i2];
		v[2] = vertices[i3];
		
		// Si la face n'est pas visible, on ne teste pas l'intersection
		glm::vec3 n = glm::cross(v[1] - v[0], v[2] - v[0]);
		if (glm::dot(n, ray.direction) >= 0)
			continue;

		for (int j = 3; j <= 5; ++j)
		{
			int temp = indices[i * 6 + j];
			if (temp != i1 && temp != i2)
			{
				v[3] = vertices[temp];
				break;
			}
		}

		double dist = glm::dot(n, ray.origin) + glm::dot(-n, v[3]);
		if (dist < 0.0)
			break; // Derrière la caméra

		glm::vec3 intersection = ray.origin + glm::vec3(ray.direction.x * dist, ray.direction.y * dist, ray.direction.z * dist);

		// Vérification que l'intersection est à l'intérieur des bornes de la face
		float minX = v[0].x;
		float maxX = v[0].x;
		float minY = v[0].y;
		float maxY = v[0].y;
		float minZ = v[0].z;
		float maxZ = v[0].z;

		for (int j = 1; j <= 3; ++j)
		{
			if (v[j].x < minX)
				minX = v[j].x;
			else if (v[j].x > maxX)
				maxX = v[j].x;

			if (v[j].y < minY)
				minY = v[j].y;
			else if (v[j].y > maxY)
				maxY = v[j].y;

			if (v[j].z < minZ)
				minZ = v[j].z;
			else if (v[j].z > maxZ)
				maxZ = v[j].z;
		}

		if (intersection.x >= minX && intersection.x <= maxX &&
			intersection.y >= minY && intersection.y <= maxY &&
			intersection.z >= minZ && intersection.z <= maxZ)
		{
			distance = dist;
			normal = n;
			intersect = true;
			break;
		}
	}

	return intersect;
}

CubeObject::~CubeObject()
{
	glDeleteVertexArrays(1, &VertexArray);
}

